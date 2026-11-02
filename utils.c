#include "libft_fairy.h"

int	g_malloc_wrap_enabled = 0;
int	g_malloc_count;
int	g_malloc_fail_at;

/*
 * The latest allocations, so that a test can check the size of a result. The
 * addresses are kept inverted: Valgrind would otherwise find them here and
 * call a leaked block "still reachable" instead of "definitely lost".
 */
static struct {
	uintptr_t	inverted;
	size_t		size;
}				g_allocs[256];
static size_t	g_nallocs;

/*
 * With -Wl,--wrap=malloc, every malloc of the library goes through here: the
 * n-th one (g_malloc_fail_at) can return NULL, and each size is recorded.
 */
void	*__wrap_malloc(size_t size) {
	void	*ptr;

	if (g_malloc_wrap_enabled && ++g_malloc_count == g_malloc_fail_at)
		return NULL;
	ptr = __real_malloc(size);
	g_allocs[g_nallocs % ARRAY_SIZE(g_allocs)].inverted = ~(uintptr_t)ptr;
	g_allocs[g_nallocs++ % ARRAY_SIZE(g_allocs)].size = size;
	return ptr;
}

/* The size malloc'd for ptr, 0 if it does not come from a recent malloc. */
size_t	alloc_size(void *ptr) {
	for (size_t i = 1; i <= ARRAY_SIZE(g_allocs) && i <= g_nallocs; i++)
		if (g_allocs[(g_nallocs - i) % ARRAY_SIZE(g_allocs)].inverted == ~(uintptr_t)ptr)
			return g_allocs[(g_nallocs - i) % ARRAY_SIZE(g_allocs)].size;
	return 0;
}

/* s quoted, non-printable bytes as \xHH, cut after 40 characters. Four
 * buffers in turn: up to four show() in one message. */
const char	*show(const char *s) {
	static char	bufs[4][200];
	static int	turn;
	char		*out = bufs[turn++ % 4];
	size_t		n = 1;
	size_t		i;

	if (!s)
		return "NULL";
	out[0] = '"';
	for (i = 0; s[i] && i < 40; i++) {
		unsigned char	c = s[i];

		if (c == '\n' || c == '\t' || c == '"' || c == '\\')
			n += sprintf(out + n, "\\%c", c == '\n' ? 'n' : c == '\t' ? 't' : c);
		else if (isprint(c))
			out[n++] = c;
		else
			n += sprintf(out + n, "\\x%02x", c);
	}
	strcpy(out + n, s[i] ? "\"…" : "\"");
	return out;
}

/* A malloc'd copy of expected, '\0' included, else fails. */
void	check_str(const char *call, char *got, const char *expected) {
	EXPECT(got, "%s: got NULL, expected %s", call, show(expected));
	EXPECT(!strcmp(got, expected), "%s: got %s, expected %s", call, show(got), show(expected));
	EXPECT(alloc_size(got) > strlen(expected), "%s: %s fits in %zu malloc'd bytes, it needs %zu",
		call, show(got), alloc_size(got), strlen(expected) + 1);
}

/* The n bytes of got must be those of expected, else fails on the first. */
void	expect_same(const char *call, const void *got, const void *expected, size_t n) {
	const unsigned char	*g = got;
	const unsigned char	*e = expected;

	for (size_t i = 0; i < n; i++)
		EXPECT(g[i] == e[i], "%s: byte %zu is 0x%02x, expected 0x%02x", call, i, g[i], e[i]);
}

void	*xmalloc(size_t size) {
	void	*ptr = malloc(size);

	if (!ptr)
		die("libft-fairy: malloc");
	return ptr;
}

/* A list of n nodes whose contents are malloc'd ints 1, 2... n. */
t_list	*make_list(int n) {
	t_list	*head = NULL;

	while (n > 0) {
		t_list	*node = xmalloc(sizeof(t_list));

		node->content = xmalloc(sizeof(int));
		*(int *)node->content = n--;
		node->next = head;
		head = node;
	}
	return head;
}

void	free_list(t_list *lst) {
	while (lst) {
		t_list	*next = lst->next;

		free(lst->content);
		free(lst);
		lst = next;
	}
}

/* ************************************************************************** */
/*                     undefined behaviour: crash or not                      */
/* ************************************************************************** */

static enum e_ub	g_ub;

static void	on_crash(int sig) {
	static const char	msg[] = "⚠ crashes: undefined behaviour, but evaluators often test it\n";

	(void)sig;
	if (g_ub == UB_CRASH)
		_exit(0);
	if (write(STDERR_FILENO, msg, sizeof(msg) - 1) < 0)
		_exit(1);
	_exit(2);
}

static void	run_test(const t_test *t) {
	g_ub = t->ub;
	if (t->ub) {
		signal(SIGSEGV, on_crash);
		signal(SIGBUS, on_crash);
	}
	t->fn();
	if (t->ub == UB_CRASH)
		warning("does not crash where the libc does: undefined behaviour, either is fine");
}

/* ═══════════════════════════════ fairy core ═══════════════════════════════ */
/* Identical in libft-fairy, printf-fairy and gnl-fairy: every test runs in a */
/* child process, so a crash or an infinite loop only fails that test. What   */
/* the child writes (fail and warning lines, sanitizer reports, glibc's own   */
/* "double free" messages) is shown under the test when it did not pass.      */

static int	g_count[3];	/* tests passed, failed, passed with warnings */
static int	g_warned;	/* in the child: warning() was called */
static int	g_timeout = TEST_TIMEOUT;	/* 1 second once a test hung */
int			g_lenient = 0;	/* outside the subject: a failure only warns */

void	die(const char *what) {
	perror(what);
	exit(EXIT_FAILURE);
}

static void	vreport(const char *tag, const char *fmt, va_list ap) {
	fprintf(stderr, "%s ", tag);
	vfprintf(stderr, fmt, ap);
	fputc('\n', stderr);
}

/* Fails the running test: reason shown under it. */
void	fail(const char *fmt, ...) {
	va_list	ap;

	va_start(ap, fmt);
	vreport("↳", fmt, ap);
	va_end(ap);
	_exit(1);
}

/* Flags something debatable: the test still passes, the reason is shown. */
void	warning(const char *fmt, ...) {
	va_list	ap;

	va_start(ap, fmt);
	vreport("⚠", fmt, ap);
	va_end(ap);
	g_warned = 1;
}

/*
 * Runs one test in a child: 0 passed, 1 failed, 2 warned. Its output in log.
 * The child may not write files past OUTPUT_LIMIT: an endless output stops
 * there instead of filling the disk.
 */
static int	fork_test(const t_test *t, char *log, size_t size) {
	static int	fd = -1;
	pid_t		pid;
	int			status;
	ssize_t		n;
	size_t		len;

	if (fd < 0) {
		FILE	*f = tmpfile();

		if (!f)
			die("fairy: tmpfile");
		fd = fileno(f);
	}
	if (ftruncate(fd, 0) < 0 || lseek(fd, 0, SEEK_SET) < 0)
		die("fairy: tmpfile");
	fflush(NULL);
	pid = fork();
	if (pid < 0)
		die("fairy: fork");
	if (!pid) {
		setrlimit(RLIMIT_FSIZE, &(struct rlimit){OUTPUT_LIMIT, OUTPUT_LIMIT});
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		alarm(g_timeout);
		run_test(t);
		_exit(g_warned ? 2 : 0);
	}
	if (waitpid(pid, &status, 0) < 0)
		die("fairy: waitpid");
	n = pread(fd, log, size - 1, 0);
	len = n > 0 ? (size_t)n : 0;
	log[len] = '\0';
	if (WIFEXITED(status))
		return WEXITSTATUS(status) == 2 ? 2 : WEXITSTATUS(status) != 0;
	if (WTERMSIG(status) == SIGXFSZ)
		snprintf(log + len, size - len, "↳ wrote more than %d MB: endless output?\n", OUTPUT_LIMIT >> 20);
	else if (WTERMSIG(status) != SIGALRM)
		snprintf(log + len, size - len, "↳ crashed: %s\n", strsignal(WTERMSIG(status)));
	else {
		snprintf(log + len, size - len, "↳ timeout after %ds (infinite loop?)%s\n", g_timeout,
			g_timeout == TEST_TIMEOUT ? "" : ", cut short since a test hung");
		g_timeout = 1;
	}
	return 1;
}

void	section(const char *title) {
	int	n = WIDTH - 4 - (int)strlen(title);

	printf("\n" DIM "──" RESET " " BOLD "%s" RESET " " DIM, title);
	for (int i = 0; i < (n > 3 ? n : 3); i++)
		fputs("─", stdout);
	puts(RESET);
}

/* Prints a test's output indented, without the "==pid==" prefixes, the "====="
 * rulers and the shadow memory map of sanitizer reports. */
static void	print_log(const char *log) {
	const char	*end;
	const char	*p;

	for (; *log; log = *end ? end + 1 : end) {
		end = strchr(log, '\n');
		if (!end)
			end = log + strlen(log);
		p = log + strspn(log, "=");
		if (p == end)
			continue ;
		if (p - log == 2 && isdigit(*p) && *(p += strspn(p, "0123456789")) == '=' && p[1] == '=')
			log = p + 2 + (p[2] == ' ');
		if (!strncmp(log, "Shadow bytes around", 19))
			return ;
		printf("     %.*s\n", (int)(end - log), log);
	}
}

/* Runs a group of tests; prints those that did not pass (all in VERBOSE). */
void	run_group(const char *title, const t_test *tests, size_t n) {
	static const char	*tags[] = {GREEN "[OK]", RED "[KO]", YELLOW "[OK]", YELLOW "[KO]"};
	static char			log[16384];
	int					shown = 0;
	int					res;

	for (size_t i = 0; i < n; i++) {
		res = fork_test(&tests[i], log, sizeof(log));
		if (res == 1 && g_lenient)
			res = 3;
		g_count[res == 3 ? 2 : res]++;
		if (res == 0 && !VERBOSE)
			continue ;
		if (!shown++)
			section(title);
		printf("%s" RESET " %s\n", tags[res], tests[i].name);
		print_log(log);
	}
}

/* Hands the counts to run.sh; returns the binary's exit status: 0, 1 or 2. */
int	fairy_summary(void) {
	const char	*path = getenv("FAIRY_STATS");
	FILE		*f = path ? fopen(path, "a") : NULL;

	if (f) {
		fprintf(f, "%d %d %d\n", g_count[0] + g_count[2], g_count[1], g_count[2]);
		fclose(f);
	}
	return g_count[1] ? 1 : g_count[2] ? 2 : 0;
}

/* ══════════════════════════════ end of core ═══════════════════════════════ */
