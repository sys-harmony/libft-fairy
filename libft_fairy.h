#ifndef LIBFT_FAIRY_H
# define LIBFT_FAIRY_H

// =============================
// 📚 Libraries
// =============================
# include "libft.h"

// =============================
// 🎨 Global defines
// =============================
# define GREEN			"\033[0;32m"
# define RED			"\033[0;31m"
# define RESET			"\033[0m"
# ifndef VERBOSE
#  define VERBOSE		0
# endif
# define TEST_TIMEOUT	1	// seconds before a forked test is reported as hanging

// =============================
// 🧩 Types
// =============================
// A forked test: it passes if func returns normally (or crashes, when crash is set).
// fail_at is the number of the malloc call that will fail (0 = none).
typedef struct s_test
{
	const char	*name;
	void		(*func)(void);
	int			crash;
	int			fail_at;
}	t_test;

// =============================
// 🌍 Global variables
// =============================
extern int	g_malloc_wrap_enabled;
extern int	g_malloc_count;
extern int	g_malloc_fail_at;
extern int	g_tests_failed;

// =============================
// 🪄 Macro(s)
// =============================
# define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*(arr)))

// =============================
// 📋 Function prototypes
// =============================
void		*__real_malloc(size_t size);
void		*__wrap_malloc(size_t size);
size_t		alloc_size(void *ptr);
int			all_tests_passed(const int *passed, const size_t num_tests);
void		del_func_dummy(void *content);
void		*map_func_dynamic_content(void *content);
void		*map_func_shared_content(void *content);
void		safe_free_arr(char ***arr);
void		print_test_results(char *function_name, const size_t num_tests, const char *tests[], const int passed[]);
void		run_tests(char *function_name, const t_test *tests, const size_t num_tests);
void		check_str(char *s, const char *expected);
void		error_exit(char *msg);
void		*xmalloc(size_t size);
t_list		*create_test_list(int c1, int c2, int c3, int use_static);

#endif //LIBFT_FAIRY_H
