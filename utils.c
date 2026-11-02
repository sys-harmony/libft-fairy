#include "libft_fairy.h"

int	g_malloc_wrap_enabled = 0;
int	g_malloc_count;
int	g_malloc_fail_at;
int	g_tests_failed = 0;

// Most recent allocations, so tests can check the size of a returned buffer
static struct {
	void	*ptr;
	size_t	size;
}				g_allocs[128];
static size_t	g_allocs_count;

void	*__wrap_malloc(size_t size) {
	if (g_malloc_wrap_enabled && ++g_malloc_count == g_malloc_fail_at)
		return NULL;
	void	*ptr = __real_malloc(size);
	size_t	i = g_allocs_count++ % ARRAY_SIZE(g_allocs);

	g_allocs[i].ptr = ptr;
	g_allocs[i].size = size;
	return ptr;
}

// Size requested by the latest malloc that returned ptr, 0 if ptr was not malloc'd
size_t	alloc_size(void *ptr) {
	for (size_t i = 1; i <= ARRAY_SIZE(g_allocs) && i <= g_allocs_count; i++)
		if (g_allocs[(g_allocs_count - i) % ARRAY_SIZE(g_allocs)].ptr == ptr)
			return g_allocs[(g_allocs_count - i) % ARRAY_SIZE(g_allocs)].size;
	return 0;
}

int	all_tests_passed(const int *passed, const size_t num_tests) {
	for (size_t i = 0; i < num_tests; i++) {
		if (!passed[i])
			return 0;
	}
	return 1;
}

void	del_func_dummy(void *content) {
	(void)content;
}

#include <stdlib.h>

void	*map_func_dynamic_content(void *content) {
	int	*new = malloc(sizeof(int));

	if (!new)
		return NULL;
	*new = *(int *)content * 2;
	return new;
}

void	*map_func_shared_content(void *content) {
	return content;
}

void	safe_free_arr(char ***arr) {
	if (*arr) {
		for (int i = 0; (*arr)[i]; i++) {
			free((*arr)[i]);
			(*arr)[i] = NULL;
		}
		free(*arr);
		*arr = NULL;
	}
}

#include <stdio.h>

void	print_test_results(char *function_name, const size_t num_tests, const char *tests[], const int passed[]) {
	printf("\n========================================\n");
	printf("%s\n", function_name);
	printf("========================================\n");
	for (size_t i = 0; i < num_tests; i++) {
		printf("%s" RESET "  Test %s\n", passed[i] ? GREEN "[OK]" : RED "[KO]", tests[i]);
		if (!passed[i])
			g_tests_failed++;
	}
}

void	error_exit(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

void	*xmalloc(size_t size) {
	void	*ptr = malloc(size);

	if (ptr)
		return ptr;
	error_exit("libft-fairy: malloc failed");
	return NULL;
}

#include <string.h>

// Aborts unless s is a malloc'd copy of expected, big enough for its '\0', then frees s
void	check_str(char *s, const char *expected) {
	if (!s || strcmp(s, expected) || alloc_size(s) < strlen(expected) + 1)
		abort();
	free(s);
}

// Builds the list c1 -> c2 -> c3, with malloc'd contents (or static ones if use_static)
t_list	*create_test_list(int c1, int c2, int c3, int use_static) {
	static int	statics[3];
	const int	values[3] = {c1, c2, c3};
	t_list		*head = NULL;

	for (int i = 2; i >= 0; i--) {
		t_list	*node = xmalloc(sizeof(t_list));

		node->content = use_static ? &statics[i] : xmalloc(sizeof(int));
		*(int *)node->content = values[i];
		node->next = head;
		head = node;
	}
	return head;
}

#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static pid_t	xwaitpid(pid_t pid, int *status, int options)
{
	pid_t    ret = waitpid(pid, status, options);
	if (ret == -1)
		error_exit("libft-fairy: waitpid failed");
	return ret;
}

// Runs test_func in a child process, returns 0 if it returned normally, else the signal that killed it
static int	forked_test(void (*test_func)(void)) {
	pid_t	pid;
	int		status;

	fflush(stdout);
	pid = fork();
	if (pid == -1)
		error_exit("libft-fairy: fork failed");
	if (!pid) {
		alarm(TEST_TIMEOUT);
		test_func();
		exit(EXIT_SUCCESS);
	}
	xwaitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		return WTERMSIG(status);
	return 0;
}

static const char	*failure_reason(int sig) {
	if (!sig)
		return "did not crash";
	if (sig == SIGALRM)
		return "timeout";
	if (sig == SIGABRT)
		return "wrong result";
	return strsignal(sig);
}

// Forks every test (the malloc wrapper state is set before forking, so each child inherits it)
// and prints the results if a test failed or in verbose mode
void	run_tests(char *function_name, const t_test *tests, const size_t num_tests) {
	const char	*names[num_tests];
	char		labels[num_tests][128];
	int			passed[num_tests];

	for (size_t i = 0; i < num_tests; i++) {
		g_malloc_count = 0;
		g_malloc_fail_at = tests[i].fail_at;
		int sig = forked_test(tests[i].func);
		passed[i] = tests[i].crash ? sig && sig != SIGALRM : !sig;
		names[i] = tests[i].name;
		if (!passed[i]) {
			snprintf(labels[i], sizeof(labels[i]), "%s  (%s)", tests[i].name, failure_reason(sig));
			names[i] = labels[i];
		}
	}
	g_malloc_fail_at = 0;
	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results(function_name, num_tests, names, passed);
}
