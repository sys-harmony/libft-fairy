#include "libft_fairy.h"
#include <stdlib.h>

static int	g_freed_count;

static void	free_count(void *content) {
	free(content);
	g_freed_count++;
}

/* ************************************************************************** */
/*                             ft_lstnew tests                                */
/* ************************************************************************** */

static void	ft_lstnew_basic_test(void) {
	int		content = 42;
	t_list	*node = ft_lstnew(&content);

	if (!node || node->content != &content || node->next)
		abort();
}

static void	ft_lstnew_null_content_test(void) {
	t_list	*node = ft_lstnew(NULL);

	if (!node || node->content || node->next)
		abort();
}

static void	test_ft_lstnew(void) {
	const t_test	tests[] = {
		{"basic", ft_lstnew_basic_test},
		{"with NULL content", ft_lstnew_null_content_test}
	};

	run_tests("ft_lstnew", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                           ft_lstadd_front tests                            */
/* ************************************************************************** */

static void	ft_lstadd_front_basic_test(void) {
	int		c1 = 1, c2 = 2;
	t_list	*n1 = xmalloc(sizeof(t_list));
	t_list	*n2 = xmalloc(sizeof(t_list));
	t_list	*lst = n1;

	n1->content = &c1;
	n1->next = NULL;
	n2->content = &c2;
	n2->next = NULL;
	ft_lstadd_front(&lst, n2);
	if (lst != n2 || *(int *)lst->content != 2 || *(int *)lst->next->content != 1)
		abort();
}

static void	ft_lstadd_front_empty_list_test(void) {
	int		content = 1;
	t_list	*empty = NULL;
	t_list	*node = xmalloc(sizeof(t_list));

	node->content = &content;
	node->next = NULL;
	ft_lstadd_front(&empty, node);
	if (empty != node || empty->next)
		abort();
}

static void	ft_lstadd_front_null_list_test(void) {
	t_list	*node = xmalloc(sizeof(t_list));

	node->content = &(int){42};
	node->next = NULL;
	ft_lstadd_front(NULL, node);
	free(node);
}

static void	ft_lstadd_front_null_new_test(void) {
	t_list	*lst = NULL;

	ft_lstadd_front(&lst, NULL);
}

static void	ft_lstadd_front_null_both_test(void) {
	ft_lstadd_front(NULL, NULL);
}

static void	test_ft_lstadd_front(void) {
	const t_test	tests[] = {
		{"basic", ft_lstadd_front_basic_test},
		{"add to empty list", ft_lstadd_front_empty_list_test},
		{"NULL list", ft_lstadd_front_null_list_test},
		{"NULL new", ft_lstadd_front_null_new_test},
		{"NULL both", ft_lstadd_front_null_both_test}
	};

	run_tests("ft_lstadd_front", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                              ft_lstsize tests                              */
/* ************************************************************************** */

static void	ft_lstsize_basic_test(void) {
	t_list	*lst = create_test_list(1, 2, 3, 0);

	if (ft_lstsize(lst) != 3)
		abort();
}

static void	ft_lstsize_null_test(void) {
	if (ft_lstsize(NULL) != 0)
		abort();
}

static void	test_ft_lstsize(void) {
	const t_test	tests[] = {
		{"basic", ft_lstsize_basic_test},
		{"NULL (empty list)", ft_lstsize_null_test}
	};

	run_tests("ft_lstsize", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                              ft_lstlast tests                              */
/* ************************************************************************** */

static void ft_lstlast_basic_test(void) {
	t_list	*lst = create_test_list(1, 2, 3, 0);

	if (ft_lstlast(lst) != lst->next->next)
		abort();
}

static void ft_lstlast_null_test(void) {
	if (ft_lstlast(NULL))
		abort();
}

static void	test_ft_lstlast(void) {
	const t_test	tests[] = {
		{"basic", ft_lstlast_basic_test},
		{"NULL (empty list)", ft_lstlast_null_test}
	};

	run_tests("ft_lstlast", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                            ft_lstadd_back tests                            */
/* ************************************************************************** */

static void	ft_lstadd_back_basic_test(void) {
	t_list	*lst = create_test_list(1, 2, 3, 0);
	t_list	*new = xmalloc(sizeof(t_list));

	new->content = &(int){4};
	new->next = NULL;
	ft_lstadd_back(&lst, new);
	if (lst->next->next->next != new || new->next || *(int *)lst->content != 1)
		abort();
}

static void	ft_lstadd_back_empty_list_test(void) {
	int		content = 1;
	t_list	*empty = NULL;
	t_list	*node = xmalloc(sizeof(t_list));

	node->content = &content;
	node->next = NULL;
	ft_lstadd_back(&empty, node);
	if (empty != node || empty->next)
		abort();
}

static void	ft_lstadd_back_null_list_test(void) {
	t_list	*node = xmalloc(sizeof(t_list));

	node->content = &(int){42};
	node->next = NULL;
	ft_lstadd_back(NULL, node);
	free(node);
}

static void	ft_lstadd_back_null_new_test(void) {
	t_list	*lst = NULL;

	ft_lstadd_back(&lst, NULL);
}

static void	ft_lstadd_back_null_both_test(void) {
	ft_lstadd_back(NULL, NULL);
}

static void	test_ft_lstadd_back(void) {
	const t_test	tests[] = {
		{"basic (3 nodes)", ft_lstadd_back_basic_test},
		{"add to empty list", ft_lstadd_back_empty_list_test},
		{"NULL list", ft_lstadd_back_null_list_test},
		{"NULL new", ft_lstadd_back_null_new_test},
		{"NULL both", ft_lstadd_back_null_both_test}
	};

	run_tests("ft_lstadd_back", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                             ft_lstdelone tests                             */
/* ************************************************************************** */

static void	ft_lstdelone_free_once_test(void) {
	t_list *lst = create_test_list(1, 2, 3, 0);
	t_list *to_del = lst->next;

	lst->next = lst->next->next;
	g_freed_count = 0;
	ft_lstdelone(to_del, free_count);
	if (g_freed_count != 1)
		abort();
}

static void	ft_lstdelone_remaining_nodes_test(void) {
	t_list *lst = create_test_list(1, 2, 3, 0);
	t_list *to_del = lst->next;

	lst->next = lst->next->next;
	ft_lstdelone(to_del, free);
	if ((*(int *)lst->content != 1) || (*(int *)lst->next->content != 3) || lst->next->next)
		abort();
}

static void	ft_lstdelone_null_node_test(void) {
	ft_lstdelone(NULL, free);
}

static void	ft_lstdelone_null_func_test(void) {
	t_list *lst = create_test_list(1, 2, 3, 0);

	ft_lstdelone(lst, NULL);
	while (lst)
	{
		t_list *tmp = lst->next;
		free(lst->content);
		free(lst);
		lst = tmp;
	}
}

static void	ft_lstdelone_null_both_test(void) {
	ft_lstdelone(NULL, NULL);
}

static void	test_ft_lstdelone(void)
{
	const t_test	tests[] = {
		{"free once", ft_lstdelone_free_once_test},
		{"correct remaining nodes", ft_lstdelone_remaining_nodes_test},
		{"NULL node", ft_lstdelone_null_node_test},
		{"NULL function", ft_lstdelone_null_func_test},
		{"NULL both", ft_lstdelone_null_both_test}
	};

	run_tests("ft_lstdelone", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                             ft_lstclear tests                              */
/* ************************************************************************** */

static void	ft_lstclear_basic_test(void) {
	t_list	*lst = create_test_list(1, 2, 3, 0);

	g_freed_count = 0;
	ft_lstclear(&lst, free_count);
	if (lst || g_freed_count != 3)
		abort();
}

static int	g_del_no_free_count;

static void	del_no_free(void *content) {
	if (content)
	{
		int *val = (int *)content;
		*val = 0;
		g_del_no_free_count++;
	}
}

static void	ft_lstclear_del_no_free_test(void) {
	t_list	*lst = create_test_list(42, 21, 84, 0);
	int		*c1 = lst->content;
	int		*c2 = lst->next->content;
	int		*c3 = lst->next->next->content;

	g_del_no_free_count = 0;
	ft_lstclear(&lst, del_no_free);
	if (g_del_no_free_count != 3 || *c1 || *c2 || *c3)
	    abort();
	free(c1);
	free(c2);
	free(c3);
}

static void	ft_lstclear_null_list_test(void) {
	ft_lstclear(NULL, free);
}

static void	ft_lstclear_null_func_test(void) {
	t_list	*lst = create_test_list(1, 2, 3, 0);

	ft_lstclear(&lst, NULL);
	while (lst)
	{
		t_list *tmp = lst->next;
		free(lst->content);
		free(lst);
		lst = tmp;
	}
}

static void	ft_lstclear_null_both_test(void) {
	ft_lstclear(NULL, NULL);
}

static void	test_ft_lstclear(void) {
	const t_test	tests[] = {
		{"basic", ft_lstclear_basic_test},
		{"content freed by del only", ft_lstclear_del_no_free_test},
		{"NULL list", ft_lstclear_null_list_test},
		{"NULL function", ft_lstclear_null_func_test},
		{"NULL both", ft_lstclear_null_both_test}
	};

	run_tests("ft_lstclear", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                              ft_lstiter tests                              */
/* ************************************************************************** */

static void	iter_func(void *content) {
	(*(int *)content)++;
}

static void	ft_lstiter_basic_test(void) {
	t_list	*lst = create_test_list(1, 2, 3, 0);

	ft_lstiter(lst, iter_func);
	if (*(int *)lst->content != 2 || *(int *)lst->next->content != 3 || *(int *)lst->next->next->content != 4)
		abort();
}

static void	ft_lstiter_null_list_test(void) {
	ft_lstiter(NULL, iter_func);
}

static void	ft_lstiter_null_func_test(void) {
	t_list	*lst = create_test_list(1, 2, 3, 0);

	ft_lstiter(lst, NULL);
	while (lst)
	{
		t_list *tmp = lst->next;
		free(lst->content);
		free(lst);
		lst = tmp;
	}
}

static void	ft_lstiter_null_both_test(void) {
	ft_lstiter(NULL, NULL);
}

static void	test_ft_lstiter(void) {
	const t_test	tests[] = {
		{"basic", ft_lstiter_basic_test},
		{"NULL list", ft_lstiter_null_list_test},
		{"NULL function", ft_lstiter_null_func_test},
		{"NULL both", ft_lstiter_null_both_test}
	};

	run_tests("ft_lstiter", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                              ft_lstmap tests                               */
/* ************************************************************************** */

static void	ft_lstmap_basic_test(void) {
	t_list			*lst = create_test_list(1, 2, 3, 0);
	t_list			*new_lst = ft_lstmap(lst, map_func_dynamic_content, free);

	if (!new_lst || *(int *)new_lst->content != 2 || *(int *)new_lst->next->content != 4
		|| *(int *)new_lst->next->next->content != 6 || new_lst->next->next->next)
		abort();
}

static void	ft_lstmap_null_list_test(void) {
	t_list *lst = ft_lstmap(NULL, map_func_dynamic_content, free);

	if (lst)
		abort();
}

static void	ft_lstmap_null_func_test(void) {
	t_list	*lst = create_test_list(1, 2, 3, 0);

	ft_lstmap(lst, NULL, free);
	while (lst)
	{
		t_list *tmp = lst->next;
		free(lst->content);
		free(lst);
		lst = tmp;
	}
}

static void	ft_lstmap_null_both_test(void) {
	ft_lstmap(NULL, NULL, free);
}

// f does not allocate, so every malloc is a node: lstmap must return NULL when one fails
static void	ft_lstmap_malloc_fail_test(void) {
	t_list	*lst = create_test_list(1, 2, 3, 0);

	g_malloc_wrap_enabled = 1;
	t_list *new_lst = ft_lstmap(lst, map_func_shared_content, del_func_dummy);
	g_malloc_wrap_enabled = 0;
	if (new_lst && g_malloc_count >= g_malloc_fail_at)
		abort();
}

static void	test_ft_lstmap(void) {
	const t_test	tests[] = {
		{"basic", ft_lstmap_basic_test},
		{"NULL list", ft_lstmap_null_list_test},
		{"NULL function", ft_lstmap_null_func_test},
		{"NULL both", ft_lstmap_null_both_test},
		{"malloc fail #1 (node 1)", ft_lstmap_malloc_fail_test, .fail_at = 1},
		{"malloc fail #2 (node 2)", ft_lstmap_malloc_fail_test, .fail_at = 2},
		{"malloc fail #3 (node 3)", ft_lstmap_malloc_fail_test, .fail_at = 3}
	};

	run_tests("ft_lstmap", tests, ARRAY_SIZE(tests));
}

int	main(void) {
	test_ft_lstnew();
	test_ft_lstadd_front();
	test_ft_lstsize();
	test_ft_lstlast();
	test_ft_lstadd_back();
	test_ft_lstdelone();
	test_ft_lstclear();
	test_ft_lstiter();
	test_ft_lstmap();
	return (g_tests_failed ? 1 : 0);
}
