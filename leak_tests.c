#include "libft_fairy.h"

/*
 * Run under Valgrind: any leak, invalid access or use of an uninitialised
 * value fails. Every allocating function of Part 1 and 2 runs once normally,
 * then with each of its mallocs failing in turn: what it allocated before
 * must be freed. The lists are in leak_tests_lists.c, so that a function
 * missing in one part does not stop the other from being checked.
 */

static void	free_split(void *p) {
	char	**words = p;

	for (size_t i = 0; words && words[i]; i++)
		free(words[i]);
	free(words);
}

/* Not ft_toupper: this file must not depend on another function. */
static char	upper(unsigned int i, char c) {
	(void)i;
	return c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c;
}

int	main(void) {
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
	return 0;
}
