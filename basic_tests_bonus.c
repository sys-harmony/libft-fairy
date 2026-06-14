#include "libft_fairy.h"
#include <stdlib.h>

#ifndef VERBOSE
# define VERBOSE 0
#endif

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
	const char		*tests[] = {
		"basic",
		"with NULL content"
	};
	const size_t	num_tests = ARRAY_SIZE(tests);
	int				passed[] = {
		!forked_test(ft_lstnew_basic_test),
		!forked_test(ft_lstnew_null_content_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_lstnew (bonus)", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                           ft_lstadd_front tests                            */
/* ************************************************************************** */

static void	ft_lstadd_front_basic_test(void) {
	int		c1 = 1, c2 = 2;
	t_list	*n1 = xmalloc(sizeof(t_list));
	t_list	*n2 = malloc(sizeof(t_list));
	t_list	*lst = n1;

	if (!n2) {
		free(n1);
		error_exit("libft-fairy: malloc failed");
	}
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
	const char		*tests[] = {
		"basic",
		"add to empty list",
		"NULL list",
		"NULL new",
		"NULL both"
	};
	const size_t	num_tests = ARRAY_SIZE(tests);
	int				passed[] = {
		!forked_test(ft_lstadd_front_basic_test),
		!forked_test(ft_lstadd_front_empty_list_test),
		!forked_test(ft_lstadd_front_null_list_test),
		!forked_test(ft_lstadd_front_null_new_test),
		!forked_test(ft_lstadd_front_null_both_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_lstadd_front (bonus)", num_tests, tests, passed);
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
	ft_lstsize(NULL);
}

static void	test_ft_lstsize(void) {
	const char		*tests[] = {
		"basic",
		"NULL"
	};
	const size_t	num_tests = ARRAY_SIZE(tests);
	const int		passed[] = {
		!forked_test(ft_lstsize_basic_test),
		!forked_test(ft_lstsize_null_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_lstsize (bonus)", num_tests, tests, passed);
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
	ft_lstlast(NULL);
}

static void	test_ft_lstlast(void) {
	const char		*tests[] = {
		"basic",
		"NULL"
	};
	const size_t	num_tests = ARRAY_SIZE(tests);
	const int		passed[] = {
		!forked_test(ft_lstlast_basic_test),
		!forked_test(ft_lstlast_null_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_lstlast (bonus)", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_lstadd_back tests                            */
/* ************************************************************************** */

static void	ft_lstadd_back_basic_test(void) {
	int				c1 = 1, c2 = 2;
	t_list			*lst = xmalloc(sizeof(t_list));
	t_list			*new = malloc(sizeof(t_list));

	if (!new) {
		free(lst);
		error_exit("libft-fairy: malloc failed");
	}
	lst->content = &c1;
	lst->next = NULL;
	new->content = &c2;
	new->next = NULL;
	ft_lstadd_back(&lst, new);
	if (lst->next != new || *(int *)new->content != 2)
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
	const char		*tests[] = {
		"basic",
		"add to empty list",
		"NULL list",
		"NULL new",
		"NULL both"
	};
	const size_t	num_tests = ARRAY_SIZE(tests);
	const int		passed[] = {
		!forked_test(ft_lstadd_back_basic_test),
		!forked_test(ft_lstadd_back_empty_list_test),
		!forked_test(ft_lstadd_back_null_list_test),
		!forked_test(ft_lstadd_back_null_new_test),
		!forked_test(ft_lstadd_back_null_both_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_lstadd_back (bonus)", num_tests, tests, passed);
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
	const char		*tests[] = {
		"free once",
		"correct remaining nodes",
		"NULL node",
		"NULL function",
		"NULL both"
	};
	const size_t	num_tests = ARRAY_SIZE(tests);
	const int		passed[] = {
		!forked_test(ft_lstdelone_free_once_test),
		!forked_test(ft_lstdelone_remaining_nodes_test),
		!forked_test(ft_lstdelone_null_node_test),
		!forked_test(ft_lstdelone_null_func_test),
		!forked_test(ft_lstdelone_null_both_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_lstdelone (bonus)", num_tests, tests, passed);
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
	int		*c1 = lst ? lst->content : NULL;
	int		*c2 = lst && lst->next ? lst->next->content : NULL;
	int		*c3 = lst && lst->next && lst->next->next ? lst->next->next->content : NULL;

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
	const char		*tests[] = {
		"basic",
		"free instead of del",
		"NULL list",
		"NULL function",
		"NULL both"
	};
	const size_t	num_tests = ARRAY_SIZE(tests);
	int				passed[] = {
		!forked_test(ft_lstclear_basic_test),
		!forked_test(ft_lstclear_del_no_free_test),
		!forked_test(ft_lstclear_null_list_test),
		!forked_test(ft_lstclear_null_func_test),
		!forked_test(ft_lstclear_null_both_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_lstclear (bonus)", num_tests, tests, passed);
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
	const char		*tests[] = {
		"basic",
		"NULL list",
		"NULL function",
		"NULL both"
	};
	const size_t	num_tests = ARRAY_SIZE(tests);
	int				passed[] = {
		!forked_test(ft_lstiter_basic_test),
		!forked_test(ft_lstiter_null_list_test),
		!forked_test(ft_lstiter_null_func_test),
		!forked_test(ft_lstiter_null_both_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_lstiter (bonus)", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                              ft_lstmap tests                               */
/* ************************************************************************** */

static void	ft_lstmap_basic_test(void) {
	t_list			*lst = create_test_list(1, 2, 3, 0);
	t_list			*new_lst = ft_lstmap(lst, map_func_dynamic_content, free);

	if (!new_lst || *(int *)new_lst->content != 2 || *(int *)new_lst->next->content != 4 || *(int *)new_lst->next->next->content != 6)
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

static void	ft_lstmap_malloc_fail_test(void) {
	t_list	*lst = create_test_list(1, 2, 3, 0);

	g_malloc_count = 0;
	++g_malloc_fail_at;
	g_malloc_wrap_enabled = 1;
	t_list *new_lst = ft_lstmap(lst, map_func_dynamic_content, free);
	g_malloc_wrap_enabled = 0;
	while (new_lst)
	{
		t_list *tmp = new_lst->next;
		free(new_lst->content);
		free(new_lst);
		new_lst = tmp;
	}
	while (lst)
	{
		t_list *tmp = lst->next;
		free(lst->content);
		free(lst);
		lst = tmp;
	}
}

static void	test_ft_lstmap(void) {
	const char		*tests[] = {
		"basic",
		"NULL list",
		"NULL function",
		"NULL both",
		"malloc fail #1 (node 1)",
		"malloc fail #2 (content 1)",
		"malloc fail #3 (node 2)",
		"malloc fail #4 (content 2)",
		"malloc fail #5 (node 3)",
		"malloc fail #6 (content 3)"
	};
	const size_t	num_tests = ARRAY_SIZE(tests);
	int				passed[num_tests];

	passed[0] = !forked_test(ft_lstmap_basic_test);
	passed[1] = !forked_test(ft_lstmap_null_list_test);
	passed[2] = !forked_test(ft_lstmap_null_func_test);
	passed[3] = !forked_test(ft_lstmap_null_both_test);
	g_malloc_fail_at = 0;
	for (int i = 4; i < 10; i++)
		passed[i] = !forked_test(ft_lstmap_malloc_fail_test);
	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_lstmap (bonus)", num_tests, tests, passed);
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
