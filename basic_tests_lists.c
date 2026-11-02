#include "libft_fairy.h"

/*
 * Part 3. An empty list is a NULL t_list *: a valid list, which every
 * function must take. A NULL t_list ** or a NULL function is undefined
 * behaviour: crashing there only warns.
 */

static int	g_calls;
static int	g_seen[8];

static void	count_del(void *content) {
	free(content);
	g_calls++;
}

/* Records the order in which f sees the contents. */
static void	record(void *content) {
	g_seen[g_calls++ % 8] = *(int *)content;
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

static t_list	*node(void *content) {
	t_list	*n = xmalloc(sizeof(t_list));

	n->content = content;
	n->next = NULL;
	return n;
}

/* ************************************************************************** */
/*                                building                                    */
/* ************************************************************************** */

static void	lstnew_test(void) {
	int		x;
	t_list	*a = ft_lstnew(&x);
	t_list	*b = ft_lstnew(NULL);

	EXPECT(a && a->content == &x && !a->next, "ft_lstnew(&x): content or next is wrong");
	EXPECT(b && !b->content && !b->next, "ft_lstnew(NULL): content or next is wrong");
	EXPECT(alloc_size(a) >= sizeof(t_list), "ft_lstnew: %zu bytes malloc'd for a t_list of %zu",
		alloc_size(a), sizeof(t_list));
	EACH_MALLOC_FAILING(ft_lstnew(&x));
}

static void	lstadd_front_test(void) {
	t_list	*lst = NULL;
	t_list	*a = node(NULL);
	t_list	*b = node(NULL);

	ft_lstadd_front(&lst, a);
	EXPECT(lst == a && !a->next, "ft_lstadd_front on an empty list: the list is not the new node alone");
	ft_lstadd_front(&lst, b);
	EXPECT(lst == b && b->next == a && !a->next, "ft_lstadd_front: the new node is not first, followed by the list");
}

static void	lstadd_back_test(void) {
	t_list	*lst = NULL;
	t_list	*a = node(NULL);
	t_list	*b = node(NULL);
	t_list	*c = node(NULL);

	ft_lstadd_back(&lst, a);
	EXPECT(lst == a && !a->next, "ft_lstadd_back on an empty list: the list is not the new node alone");
	ft_lstadd_back(&lst, b);
	ft_lstadd_back(&lst, c);
	EXPECT(lst == a && a->next == b && b->next == c && !c->next, "ft_lstadd_back: nodes not in the order added");
}

static void	lstsize_last_test(void) {
	t_list	*lst = make_list(5);

	EXPECT(ft_lstsize(NULL) == 0, "ft_lstsize(NULL): got %u, expected 0", ft_lstsize(NULL));
	EXPECT(ft_lstsize(lst) == 5, "ft_lstsize: got %u, expected 5", ft_lstsize(lst));
	EXPECT(ft_lstsize(lst->next->next->next->next) == 1, "ft_lstsize of a single node: got %u",
		ft_lstsize(lst->next->next->next->next));
	EXPECT(!ft_lstlast(NULL), "ft_lstlast(NULL): expected NULL");
	EXPECT(ft_lstlast(lst) == lst->next->next->next->next, "ft_lstlast: not the last node");
	EXPECT(ft_lstlast(lst->next->next->next->next) == lst->next->next->next->next,
		"ft_lstlast of a single node: not that node");
}

static void	add_null_list_test(void) {
	ft_lstadd_front(NULL, node(NULL));
	ft_lstadd_back(NULL, node(NULL));
}

static void	add_null_node_test(void) {
	t_list	*lst = make_list(2);

	ft_lstadd_front(&lst, NULL);
	ft_lstadd_back(&lst, NULL);
}

static const t_test	g_build_tests[] = {
	{"ft_lstnew: content, next, malloc failing", lstnew_test},
	{"ft_lstadd_front: empty list, then in front", lstadd_front_test},
	{"ft_lstadd_back: empty list, then in order", lstadd_back_test},
	{"ft_lstsize, ft_lstlast: empty, one, five nodes", lstsize_last_test},
	{"ft_lstadd_front, ft_lstadd_back (NULL, node)", add_null_list_test, UB_SAFE},
	{"ft_lstadd_front, ft_lstadd_back (&lst, NULL)", add_null_node_test, UB_SAFE},
};

/* ************************************************************************** */
/*                                 freeing                                    */
/* ************************************************************************** */

static void	lstdelone_test(void) {
	t_list	*lst = make_list(3);
	t_list	*second = lst->next;

	lst->next = second->next;
	g_calls = 0;
	ft_lstdelone(second, count_del);
	EXPECT(g_calls == 1, "ft_lstdelone: del called %d times, expected once", g_calls);
	EXPECT(*(int *)lst->content == 1 && *(int *)lst->next->content == 3 && !lst->next->next,
		"ft_lstdelone: the other nodes changed");
}

static void	lstclear_test(void) {
	t_list	*lst = make_list(4);
	t_list	*empty = NULL;

	g_calls = 0;
	ft_lstclear(&lst, count_del);
	EXPECT(!lst, "ft_lstclear: the list pointer is not set to NULL");
	EXPECT(g_calls == 4, "ft_lstclear on 4 nodes: del called %d times", g_calls);
	ft_lstclear(&empty, count_del);
	EXPECT(!empty && g_calls == 4, "ft_lstclear on an empty list: del called");
}

static void	free_null_test(void) {
	ft_lstdelone(NULL, free);
	ft_lstclear(NULL, free);
}

static void	free_null_del_test(void) {
	t_list	*lst = make_list(2);

	ft_lstdelone(lst->next, NULL);
	lst->next = NULL;
	ft_lstclear(&lst, NULL);
}

static const t_test	g_free_tests[] = {
	{"ft_lstdelone: del once, the rest untouched", lstdelone_test},
	{"ft_lstclear: del on each node, list set to NULL", lstclear_test},
	{"ft_lstdelone, ft_lstclear (NULL, free)", free_null_test, UB_SAFE},
	{"ft_lstdelone, ft_lstclear with del NULL", free_null_del_test, UB_SAFE},
};

/* ************************************************************************** */
/*                                iterating                                   */
/* ************************************************************************** */

static void	lstiter_test(void) {
	t_list	*lst = make_list(5);

	g_calls = 0;
	ft_lstiter(lst, record);
	EXPECT(g_calls == 5, "ft_lstiter on 5 nodes: f called %d times", g_calls);
	for (int i = 0; i < 5; i++)
		EXPECT(g_seen[i] == i + 1, "ft_lstiter: call %d got content %d, expected %d", i + 1, g_seen[i], i + 1);
	ft_lstiter(NULL, record);
	EXPECT(g_calls == 5, "ft_lstiter on an empty list: f called");
}

static void	lstmap_test(void) {
	t_list	*lst = make_list(4);
	t_list	*map = ft_lstmap(lst, double_it, free);
	t_list	*m = map;

	for (int i = 1; i <= 4; i++, m = m->next) {
		EXPECT(m, "ft_lstmap on 4 nodes: %d nodes", i - 1);
		EXPECT(*(int *)m->content == 2 * i, "ft_lstmap(f: n * 2): node %d holds %d, expected %d", i,
			*(int *)m->content, 2 * i);
		EXPECT(*(int *)lst->content == i, "ft_lstmap: the source list changed");
		lst = lst->next;
	}
	EXPECT(!m, "ft_lstmap on 4 nodes: more than 4 nodes");
	EXPECT(!ft_lstmap(NULL, double_it, free), "ft_lstmap on an empty list: expected NULL");
}

/* f allocates nothing, so every malloc is a node: one failing gives NULL. */
static void	lstmap_fail_test(void) {
	t_list	*lst = make_list(3);

	EACH_MALLOC_FAILING(ft_lstmap(lst, same, nothing));
}

static void	iter_null_test(void) {
	t_list	*lst = make_list(2);

	ft_lstiter(lst, NULL);
	ft_lstmap(lst, NULL, free);
}

static const t_test	g_iter_tests[] = {
	{"ft_lstiter: each content in order, empty list", lstiter_test},
	{"ft_lstmap: new contents, source untouched", lstmap_test},
	{"ft_lstmap: NULL when a node's malloc fails", lstmap_fail_test},
	{"ft_lstiter, ft_lstmap with f NULL", iter_null_test, UB_SAFE},
};

int	main(void) {
	RUN_GROUP("lists: building", g_build_tests);
	RUN_GROUP("lists: freeing", g_free_tests);
	RUN_GROUP("lists: iterating", g_iter_tests);
	return fairy_summary();
}
