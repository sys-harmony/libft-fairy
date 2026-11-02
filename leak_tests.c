#include "libft_fairy.h"

/*
 * Run under Valgrind: any leak, invalid access or use of an uninitialised
 * value fails. Every allocating function runs once normally, then with each
 * of its mallocs failing in turn: what it allocated before must be freed.
 *
 * "./leak_tests lstmap" runs only ft_lstmap with an f that allocates: when a
 * node's malloc fails, the content f just made must go through del. Most
 * evaluations and the Moulinette skip it, so run.sh only warns about it.
 */

#define EACH_FAILING(call, release) \
	for (int k_ = 1; ; k_++) \
	{ \
		void	*r_; \
		g_malloc_count = 0; \
		g_malloc_fail_at = k_; \
		g_malloc_wrap_enabled = 1; \
		r_ = (void *)(call); \
		g_malloc_wrap_enabled = 0; \
		release(r_); \
		if (g_malloc_count < k_) \
			break ; \
	}

static void	free_split(void *p) {
	char	**words = p;

	for (size_t i = 0; words && words[i]; i++)
		free(words[i]);
	free(words);
}

static void	free_lst(void *p) {
	free_list(p);
}

static void	*double_it(void *content) {
	int	*n = malloc(sizeof(int));

	if (n)
		*n = *(int *)content * 2;
	return n;
}

static void	*same(void *content) {
	return content;
}

static void	nothing(void *content) {
	(void)content;
}

static char	upper(unsigned int i, char c) {
	(void)i;
	return ft_toupper(c);
}

/* Frees the nodes of a list whose contents belong to another one. */
static void	free_nodes(void *p) {
	t_list	*lst = p;

	while (lst) {
		t_list	*next = lst->next;

		free(lst);
		lst = next;
	}
}

static void	strings(void) {
	EACH_FAILING(ft_calloc(10, sizeof(int)), free);
	EACH_FAILING(ft_strdup("Hello World"), free);
	EACH_FAILING(ft_substr("Hello World", 0, 5), free);
	EACH_FAILING(ft_substr("Hello", 10, 5), free);
	EACH_FAILING(ft_strjoin("Hello", " World"), free);
	EACH_FAILING(ft_strtrim("   Hello World   ", " "), free);
	EACH_FAILING(ft_strtrim("xxxx", "x"), free);
	EACH_FAILING(ft_split("  Hello World  Test 42 ", ' '), free_split);
	EACH_FAILING(ft_split("", ' '), free_split);
	EACH_FAILING(ft_itoa(INT_MIN), free);
	EACH_FAILING(ft_itoa(0), free);
	EACH_FAILING(ft_strmapi("abc", upper), free);
}

static void	lists(void) {
	t_list	*lst = make_list(3);
	t_list	*one = make_list(1);
	t_list	*two = make_list(2);

	EACH_FAILING(ft_lstnew(NULL), free);
	ft_lstdelone(one, free);
	ft_lstclear(&two, free);
	free_list(ft_lstmap(lst, double_it, free));
	EACH_FAILING(ft_lstmap(lst, same, nothing), free_nodes);
	free_list(lst);
}

static void	lstmap_dynamic(void) {
	t_list	*lst = make_list(3);

	EACH_FAILING(ft_lstmap(lst, double_it, free), free_lst);
	free_list(lst);
}

int	main(int argc, char **argv) {
	if (argc > 1 && !strcmp(argv[1], "lstmap")) {
		lstmap_dynamic();
		return 0;
	}
	strings();
	lists();
	return 0;
}
