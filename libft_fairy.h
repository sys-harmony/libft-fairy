#ifndef LIBFT_FAIRY_H
# define LIBFT_FAIRY_H

// =============================
// 📚 Libraries
// =============================
# include "libft.h"
# include <ctype.h>
# include <limits.h>
# include <signal.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/resource.h>
# include <sys/wait.h>

// =============================
// 🎨 Global define(s)
// =============================
# define GREEN			"\033[32m"
# define RED			"\033[31m"
# define YELLOW			"\033[33m"
# define BOLD			"\033[1m"
# define DIM			"\033[2m"
# define RESET			"\033[0m"
# define WIDTH			46		// as in run.sh
# define TEST_TIMEOUT	2		// seconds before a test counts as hanging
# define OUTPUT_LIMIT	(8 << 20)	// bytes a test may write to files
# ifndef VERBOSE
#  define VERBOSE		0
# endif

// =============================
// 🧩 Type(s)
// =============================
/*
 * NULL where a string or a list is expected is undefined behaviour: either
 * way is fine, so a test marked ub never fails on it, it warns when the
 * function does not do what most expect. UB_CRASH: the libc crashes there.
 * UB_SAFE: evaluators often expect the function to survive it.
 */
enum e_ub { STRICT, UB_CRASH, UB_SAFE };

typedef struct s_test
{
	const char	*name;
	void		(*fn)(void);
	enum e_ub	ub;
}	t_test;

// =============================
// 🌍 Global variable(s)
// =============================
extern int	g_malloc_wrap_enabled;
extern int	g_malloc_count;
extern int	g_malloc_fail_at;

// =============================
// 🪄 Macro(s)
// =============================
# define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*(arr)))
# define RUN_GROUP(title, tests) run_group(title, tests, ARRAY_SIZE(tests))
# define EXPECT(cond, ...) do { if (!(cond)) fail(__VA_ARGS__); } while (0)
# define SIGN(n) (((n) > 0) - ((n) < 0))

/*
 * Makes the 1st, then the 2nd... malloc of call fail, until call no longer
 * reaches that many: whichever allocation fails, call must return NULL.
 */
# define EACH_MALLOC_FAILING(call) \
	for (int k_ = 1; ; k_++) \
	{ \
		void	*r_; \
		g_malloc_count = 0; \
		g_malloc_fail_at = k_; \
		g_malloc_wrap_enabled = 1; \
		r_ = (void *)(call); \
		g_malloc_wrap_enabled = 0; \
		if (g_malloc_count < k_) \
			break ; \
		EXPECT(!r_, "%s returned %p when malloc #%d failed, expected NULL", #call, r_, k_); \
	}

// =============================
// 📋 Function prototype(s)
// =============================
void		*__real_malloc(size_t size);
void		*__wrap_malloc(size_t size);
size_t		alloc_size(void *ptr);
const char	*show(const char *s);
void		expect_same(const char *call, const void *got, const void *expected, size_t n);
void		check_str(const char *call, char *got, const char *expected);
void		*xmalloc(size_t size);
t_list		*make_list(int n);
void		free_list(t_list *lst);

void		die(const char *what) __attribute__((noreturn));
void		fail(const char *fmt, ...)
			__attribute__((noreturn, format(printf, 1, 2)));
void		warning(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void		section(const char *title);
void		run_group(const char *title, const t_test *tests, size_t n);
int			fairy_summary(void);

#endif //LIBFT_FAIRY_H
