#include "libft_fairy.h"

/*
 * Part 3 under Valgrind: any leak, invalid access or use of an uninitialised
 * value fails. ft_lstnew and ft_lstmap run with each of their mallocs failing
 * in turn, ft_lstdelone and ft_lstclear on lists they must free whole.
 *
 * "./leak_tests_lists lstmap" runs only ft_lstmap with an f that allocates:
 * when a node's malloc fails, the content f just made must go through del.
 * Most evaluations and the Moulinette skip it, so run.sh only warns about it.
 */

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

/* Frees the nodes of a list whose contents belong to another one. */
static void	free_nodes(void *p) {
	t_list	*lst = p;

	while (lst) {
		t_list	*next = lst->next;

		free(lst);
		lst = next;
	}
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
	lists();
	return 0;
}
