#include "libft_fairy.h"

#ifndef VERBOSE
# define VERBOSE 0
#endif

/* ************************************************************************** */
/*                            ft_isalpha tests                                */
/* ************************************************************************** */

static void	test_ft_isalpha(void) {
	const char		*tests[] = {
		"EOF to '?'",
		"'A' to 'Z'",
		"'[' to '`'",
		"'a' to 'z'",
		"'{' to 255"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	int				passed[] = {1, 1, 1, 1, 1};
	int				c = -1;

	while (passed[0] && c < 'A')
		if (ft_isalpha(c++))
			passed[0] = 0;
	while (passed[1] && c <= 'Z')
		if (!ft_isalpha(c++))
			passed[1] = 0;
	while (passed[2] && c < 'a')
		if (ft_isalpha(c++))
			passed[2] = 0;
	while (passed[3] && c <= 'z')
		if (!ft_isalpha(c++))
			passed[3] = 0;
	while (passed[4] && c < 256)
		if (ft_isalpha(c++))
			passed[4] = 0;
	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_isalpha", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_isdigit tests                                */
/* ************************************************************************** */

static void	test_ft_isdigit(void) {
	const char		*tests[] = {
		"EOF to '/'",
		"'0' to '9'",
		"':' to 255"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	int				passed[] = {1, 1, 1};
	int				c = -1;

	while (passed[0] && c < '0')
		if (ft_isdigit(c++))
			passed[0] = 0;
	while (passed[1] && c <= '9')
		if (!ft_isdigit(c++))
			passed[1] = 0;
	while (passed[2] && c < 256)
		if (ft_isdigit(c++))
			passed[2] = 0;
	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_isdigit", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_isalnum tests                                */
/* ************************************************************************** */

static void	test_ft_isalnum(void) {
	const char		*tests[] = {
		"EOF to '/'",
		"'0' to '9'",
		"':' to '?'",
		"'A' to 'Z'",
		"'[' to '`'",
		"'a' to 'z'",
		"'{' to 255"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	int				passed[] = {1, 1, 1, 1, 1, 1, 1};
	int				c = -1;

	while (passed[0] && c < '0')
		if (ft_isalnum(c++))
			passed[0] = 0;
	while (passed[1] && c <= '9')
		if (!ft_isalnum(c++))
			passed[1] = 0;
	while (passed[2] && c < 'A')
		if (ft_isalnum(c++))
			passed[2] = 0;
	while (passed[3] && c <= 'Z')
		if (!ft_isalnum(c++))
			passed[3] = 0;
	while (passed[4] && c < 'a')
		if (ft_isalnum(c++))
			passed[4] = 0;
	while (passed[5] && c <= 'z')
		if (!ft_isalnum(c++))
			passed[5] = 0;
	while (passed[6] && c < 256)
		if (ft_isalnum(c++))
			passed[6] = 0;
	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_isalnum", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_isascii tests                                */
/* ************************************************************************** */

static void	test_ft_isascii(void) {
	const char		*tests[] = {
		"EOF",
		"'\\0' to DEL",
		"128 to 255"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	int				passed[] = {1, 1, 1};
	int				c = 0;

	if (ft_isascii(-1))
		passed[0] = 0;
	while (passed[1] && c < 128)
		if (!ft_isascii(c++))
			passed[1] = 0;
	while (passed[2] && c < 256)
		if (ft_isascii(c++))
			passed[2] = 0;
	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_isascii", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_isprint tests                                */
/* ************************************************************************** */

static void	test_ft_isprint(void) {
	const char		*tests[] = {
		"EOF to US",
		"' ' to '~'",
		"DEL to 255"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	int				passed[] = {1, 1, 1};
	int				c = -1;

	while (passed[0] && c < ' ')
		if (ft_isprint(c++))
			passed[0] = 0;
	while (passed[1] && c <= '~')
		if (!ft_isprint(c++))
			passed[1] = 0;
	while (passed[2] && c < 256)
		if (ft_isprint(c++))
			passed[2] = 0;
	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_isprint", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_strlen tests                                 */
/* ************************************************************************** */

#include <stdlib.h>

static void	ft_strlen_empty_test(void) {
	if (ft_strlen(""))
		abort();
}

static void	ft_strlen_hello_test(void) {
	if (ft_strlen("hello") != 5)
		abort();
}

static void	ft_strlen_42_test(void) {
	if (ft_strlen("42") != 2)
		abort();
}

static void	ft_strlen_long_test(void) {
	if (ft_strlen("Hello, World!") != 13)
		abort();
}

static void	ft_strlen_null_test(void) {
	ft_strlen(NULL);
}

static void	test_ft_strlen(void) {
	const char		*tests[] = {
		"empty string",
		"'hello'",
		"'42'",
		"long string",
		"NULL (should crash)"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_strlen_empty_test),
		!forked_test(ft_strlen_hello_test),
		!forked_test(ft_strlen_42_test),
		!forked_test(ft_strlen_long_test),
		forked_test(ft_strlen_null_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_strlen", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_memset tests                                 */
/* ************************************************************************** */

#include <string.h>

static void	ft_memset_basic_test(void) {
	char str1[10] = {0};
	char str2[10] = {0};
	void *ret = ft_memset(str1, 'A', 5);
	memset(str2, 'A', 5);
	if (memcmp(str1, str2, 10) || ret != str1)
		abort();
}

static void	ft_memset_zero_test(void) {
	char str1[10] = {0};
	char str2[10] = {0};
	memset(str1, 'X', 10);
	memset(str2, 'X', 10);
	void *ret = ft_memset(str1, 0, 10);
	memset(str2, 0, 10);
	if (memcmp(str1, str2, 10) || ret != str1)
		abort();
}

static void	ft_memset_uchar_test(void) {
	char str1[10];
	char str2[10];
	memset(str1, 200, 10);
	ft_memset(str2, 200, 10);
	if (memcmp(str1, str2, 10))
		abort();
}

static void	ft_memset_null_test(void) {
	ft_memset(NULL, 0, 5);
}

static void	test_ft_memset(void) {
	const char		*tests[] = {
		"basic memset",
		"with 0",
		"unsigned char (200)",
		"NULL (should crash)"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_memset_basic_test),
		!forked_test(ft_memset_zero_test),
		!forked_test(ft_memset_uchar_test),
		forked_test(ft_memset_null_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_memset", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_bzero tests                                  */
/* ************************************************************************** */

static void	ft_bzero_5_bytes_test(void) {
	char str[10];
	memset(str, 'A', 10);
	ft_bzero(str, 5);
	for (int i = 0; i < 5; i++)
		if (str[i])
			abort();
	for (int i = 5; i < 10; i++)
		if (str[i] != 'A')
			abort();
}

static void	ft_bzero_all_test(void) {
	char str[10];
	memset(str, 'A', 10);
	ft_bzero(str, 10);
	for (int i = 0; i < 10; i++)
		if (str[i])
			abort();
}

static void	test_ft_bzero(void) {
	const char		*tests[] = {
		"bzero 5 bytes",
		"bzero all"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_bzero_5_bytes_test),
		!forked_test(ft_bzero_all_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_bzero", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_memcpy tests                                 */
/* ************************************************************************** */

static void	ft_memcpy_basic_test(void) {
	char src[] = "Hello, World!";
	char dst1[20] = {0};
	char dst2[20] = {0};
	ft_memcpy(dst1, src, strlen(src) + 1);
	memcpy(dst2, src, strlen(src) + 1);
	if (strcmp(dst1, dst2))
		abort();
}

static void	ft_memcpy_partial_test(void) {
	char dst[20] = {0};
	ft_memcpy(dst, "42", 2);
	if (dst[0] != '4' || dst[1] != '2')
		abort();
}

static void	ft_memcpy_return_test(void) {
	char dst[20] = {0};
	if (ft_memcpy(dst, "42", 2) != dst)
		abort();
}

static void	ft_memcpy_null_n_zero_test(void) {
	ft_memcpy(NULL, NULL, 0);
}

static void	ft_memcpy_null_test(void) {
	char buffer[10];
	ft_memcpy((void *)buffer, NULL, 5);
}

static void	test_ft_memcpy(void) {
	const char		*tests[] = {
		"basic memcpy",
		"partial copy",
		"return value",
		"NULL src, n = 0 (no crash)",
		"NULL (should crash)"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_memcpy_basic_test),
		!forked_test(ft_memcpy_partial_test),
		!forked_test(ft_memcpy_return_test),
		!forked_test(ft_memcpy_null_n_zero_test),
		forked_test(ft_memcpy_null_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_memcpy", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_memmove tests                                */
/* ************************************************************************** */

static void	ft_memmove_forward_test(void) {
	char str1[] = "Hello, World!";
	char str2[] = "Hello, World!";
	ft_memmove(str1 + 2, str1, 5);
	memmove(str2 + 2, str2, 5);
	if (strcmp(str1, str2))
		abort();
}

static void	ft_memmove_backward_test(void) {
	char str1[] = "Hello, World!";
	char str2[] = "Hello, World!";
	ft_memmove(str1, str1 + 2, 5);
	memmove(str2, str2 + 2, 5);
	if (strcmp(str1, str2))
		abort();
}

static void	ft_memmove_no_overlap_test(void) {
	char dst1[20] = {0};
	char dst2[20] = {0};
	ft_memmove(dst1, "Hello", 5);
	memmove(dst2, "Hello", 5);
	if (memcmp(dst1, dst2, 5))
		abort();
}

static void	ft_memmove_same_ptr_test(void) {
	char str1[] = "Hello";
	char str2[] = "Hello";
	ft_memmove(str1, str1, 5);
	memmove(str2, str2, 5);
	if (strcmp(str1, str2))
		abort();
}

static void	ft_memmove_n_zero_test(void) {
	char str1[] = "Hello";
	char str2[] = "Hello";
	ft_memmove(str1 + 2, str1, 0);
	memmove(str2 + 2, str2, 0);
	if (strcmp(str1, str2))
		abort();
}

static void	ft_memmove_minimal_overlap_test(void) {
	char str1[] = "abcdef";
	char str2[] = "abcdef";
	ft_memmove(str1 + 1, str1, 5);
	memmove(str2 + 1, str2, 5);
	if (strcmp(str1, str2))
		abort();
}

static void	ft_memmove_return_test(void) {
	char str[] = "Hello";
	if (ft_memmove(str + 1, str, 3) != str + 1)
		abort();
}

static void	ft_memmove_null_n_zero_test(void) {
	ft_memmove(NULL, NULL, 0);
}

static void	ft_memmove_null_test(void) {
	char buffer[10];
	ft_memmove((void *)buffer, NULL, 5);
}

static void	test_ft_memmove(void) {
	const char		*tests[] = {
		"overlapping forward",
		"overlapping backward",
		"no overlap",
		"dst == src",
		"n = 0",
		"minimal overlap",
		"return value",
		"NULL src, n = 0 (no crash)",
		"NULL (should crash)"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_memmove_forward_test),
		!forked_test(ft_memmove_backward_test),
		!forked_test(ft_memmove_no_overlap_test),
		!forked_test(ft_memmove_same_ptr_test),
		!forked_test(ft_memmove_n_zero_test),
		!forked_test(ft_memmove_minimal_overlap_test),
		!forked_test(ft_memmove_return_test),
		!forked_test(ft_memmove_null_n_zero_test),
		forked_test(ft_memmove_null_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_memmove", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_strlcpy tests                                */
/* ************************************************************************** */

static void	ft_strlcpy_basic_test(void) {
	char dst[10];
	size_t len = ft_strlcpy(dst, "Hello", 20);
	if (strcmp(dst, "Hello") || len != 5)
		abort();
}

static void	ft_strlcpy_truncation_test(void) {
	char dst[10];
	size_t len = ft_strlcpy(dst, "Hello, World!", 6);
	if (strcmp(dst, "Hello") || len != 13)
		abort();
}

static void	ft_strlcpy_size_zero_test(void) {
	char dst[10] = "test";
	size_t len = ft_strlcpy(dst, "42", 0);
	if (len != 2)
		abort();
}

static void	test_ft_strlcpy(void) {
	const char		*tests[] = {
		"basic copy",
		"truncation",
		"size 0"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_strlcpy_basic_test),
		!forked_test(ft_strlcpy_truncation_test),
		!forked_test(ft_strlcpy_size_zero_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_strlcpy", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_strlcat tests                                */
/* ************************************************************************** */

static void	ft_strlcat_basic_test(void) {
	char dst[20];
	strcpy(dst, "Hello");
	size_t len = ft_strlcat(dst, " World", 20);
	if (strcmp(dst, "Hello World") || len != 11)
		abort();
}

static void	ft_strlcat_truncation_test(void) {
	char dst[20];
	strcpy(dst, "Hello");
	size_t len = ft_strlcat(dst, " World!", 10);
	if (strcmp(dst, "Hello Wor") || len != 12)
		abort();
}

static void	ft_strlcat_dstsize_eq_dstlen_test(void) {
	char dst[20];
	strcpy(dst, "Hello");
	size_t len = ft_strlcat(dst, " World", 5);
	if (strcmp(dst, "Hello") || len != 11)
		abort();
}

static void	ft_strlcat_dstsize_lt_dstlen_test(void) {
	char dst[20];
	strcpy(dst, "Hello");
	size_t len = ft_strlcat(dst, " World", 3);
	if (strcmp(dst, "Hello") || len != 9)
		abort();
}

static void	ft_strlcat_dstsize_zero_test(void) {
	char dst[20];
	strcpy(dst, "Hello");
	size_t len = ft_strlcat(dst, " World", 0);
	if (strcmp(dst, "Hello") || len != 6)
		abort();
}

static void	ft_strlcat_empty_src_test(void) {
	char dst[20];
	strcpy(dst, "Hello");
	size_t len = ft_strlcat(dst, "", 20);
	if (strcmp(dst, "Hello") || len != 5)
		abort();
}

static void	ft_strlcat_empty_dst_test(void) {
	char dst[20];
	dst[0] = '\0';
	size_t len = ft_strlcat(dst, "Hello", 20);
	if (strcmp(dst, "Hello") || len != 5)
		abort();
}

static void	ft_strlcat_no_room_test(void) {
	char dst[20];
	strcpy(dst, "Hello");
	size_t len = ft_strlcat(dst, "X", 6);
	if (strcmp(dst, "Hello") || len != 6)
		abort();
}

static void	test_ft_strlcat(void) {
	const char		*tests[] = {
		"basic concat",
		"truncation",
		"dstsize = strlen(dst)",
		"dstsize < strlen(dst)",
		"dstsize = 0",
		"empty src",
		"empty dst",
		"no room for concat"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_strlcat_basic_test),
		!forked_test(ft_strlcat_truncation_test),
		!forked_test(ft_strlcat_dstsize_eq_dstlen_test),
		!forked_test(ft_strlcat_dstsize_lt_dstlen_test),
		!forked_test(ft_strlcat_dstsize_zero_test),
		!forked_test(ft_strlcat_empty_src_test),
		!forked_test(ft_strlcat_empty_dst_test),
		!forked_test(ft_strlcat_no_room_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_strlcat", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_toupper tests                                */
/* ************************************************************************** */

static void	test_ft_toupper(void) {
	const char		*tests[] = {
		"EOF to '`'",
		"'a' to 'z'",
		"'{' to 255"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	int				passed[] = {1, 1, 1};
	int				c = -2;

	while (passed[0] && ++c < 'a')
		if (ft_toupper(c) != c)
			passed[0] = 0;
	while (passed[1] && ++c <= 'z')
		if (ft_toupper(c) != c - 32)
			passed[1] = 0;
	while (passed[2] && ++c < 256)
		if (ft_toupper(c) != c)
			passed[2] = 0;
	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_toupper", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_tolower tests                                */
/* ************************************************************************** */

static void	test_ft_tolower(void) {
	const char		*tests[] = {
		"EOF to '@'",
		"'A' to 'Z'",
		"'[' to 255"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	int				passed[] = {1, 1, 1};
	int				c = -2;

	while (passed[0] && ++c < 'A')
		if (ft_tolower(c) != c)
			passed[0] = 0;
	while (passed[1] && ++c <= 'Z')
		if (ft_tolower(c) != c + 32)
			passed[1] = 0;
	while (passed[2] && ++c < 256)
		if (ft_tolower(c) != c)
			passed[2] = 0;
	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_tolower", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_strchr tests                                 */
/* ************************************************************************** */

static void	ft_strchr_find_o_test(void) {
	const char str[] = "Hello\xC8World";
	if (ft_strchr(str, 'o') != strchr(str, 'o'))
		abort();
}

static void	ft_strchr_find_W_test(void) {
	const char str[] = "Hello\xC8World";
	if (ft_strchr(str, 'W') != strchr(str, 'W'))
		abort();
}

static void	ft_strchr_find_null_test(void) {
	const char str[] = "Hello\xC8World";
	if (ft_strchr(str, '\0') != strchr(str, '\0'))
		abort();
}

static void	ft_strchr_find_first_char_test(void) {
	const char str[] = "Hello\xC8World\xC8!";
	if (ft_strchr(str, 'H') != strchr(str, 'H'))
		abort();
}

static void	ft_strchr_find_last_char_test(void) {
	const char str[] = "Hello\xC8World\xC8!";
	if (ft_strchr(str, '!') != strchr(str, '!'))
		abort();
}

static void	ft_strchr_not_found_test(void) {
	const char str[] = "Hello\xC8World";
	if (ft_strchr(str, 'x'))
		abort();
}

static void	ft_strchr_uchar_test(void) {
	const char str[] = "Hello\xC8World";
	if (ft_strchr(str, 200) != strchr(str, 200))
		abort();
}

static void	ft_strchr_over_255_test(void) {
	const char str[] = "Hello\xC8World";
	if (ft_strchr(str, 'o' + 256) != strchr(str, 'o' + 256))
		abort();
}

static void	ft_strchr_null_test(void) {
	ft_strchr(NULL, 'a');
}

static void	test_ft_strchr(void) {
	const char		*tests[] = {
		"find 'o'",
		"find 'W'",
		"find '\\0'",
		"find first char",
		"find last char",
		"not found 'x'",
		"find unsigned char (200)",
		"find c > 255 (wraps to char)",
		"NULL (should crash)"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_strchr_find_o_test),
		!forked_test(ft_strchr_find_W_test),
		!forked_test(ft_strchr_find_null_test),
		!forked_test(ft_strchr_find_first_char_test),
		!forked_test(ft_strchr_find_last_char_test),
		!forked_test(ft_strchr_not_found_test),
		!forked_test(ft_strchr_uchar_test),
		!forked_test(ft_strchr_over_255_test),
		forked_test(ft_strchr_null_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_strchr", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_strrchr tests                                */
/* ************************************************************************** */

static void	ft_strrchr_find_last_o_test(void) {
	const char str[] = "Hello\xC8World\xC8!";
	if (ft_strrchr(str, 'o') != strrchr(str, 'o'))
		abort();
}

static void	ft_strrchr_find_W_test(void) {
	const char str[] = "Hello\xC8World\xC8!";
	if (ft_strrchr(str, 'W') != strrchr(str, 'W'))
		abort();
}

static void	ft_strrchr_find_null_test(void) {
	const char str[] = "Hello\xC8World\xC8!";
	if (ft_strrchr(str, '\0') != strrchr(str, '\0'))
		abort();
}

static void	ft_strrchr_find_first_char_test(void) {
	const char str[] = "Hello\xC8World\xC8!";
	if (ft_strrchr(str, 'H') != strrchr(str, 'H'))
		abort();
}

static void	ft_strrchr_find_last_char_test(void) {
	const char str[] = "Hello\xC8World\xC8!";
	if (ft_strrchr(str, '!') != strrchr(str, '!'))
		abort();
}

static void	ft_strrchr_not_found_test(void) {
	const char str[] = "Hello\xC8World\xC8!";
	if (ft_strrchr(str, 'x'))
		abort();
}

static void	ft_strrchr_uchar_test(void) {
	const char str[] = "Hello\xC8World\xC8!";
	if (ft_strrchr(str, 200) != strrchr(str, 200))
		abort();
}

static void	ft_strrchr_over_255_test(void) {
	const char str[] = "Hello\xC8World\xC8!";
	if (ft_strrchr(str, 'o' + 256) != strrchr(str, 'o' + 256))
		abort();
}

static void	ft_strrchr_null_test(void) {
	ft_strrchr(NULL, 'a');
}

static void	test_ft_strrchr(void) {
	const char		*tests[] = {
		"find last 'o'",
		"find 'W'",
		"find '\\0'",
		"find first char",
		"find last char",
		"not found 'x'",
		"find unsigned char (200)",
		"find c > 255 (wraps to char)",
		"NULL (should crash)"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_strrchr_find_last_o_test),
		!forked_test(ft_strrchr_find_W_test),
		!forked_test(ft_strrchr_find_null_test),
		!forked_test(ft_strrchr_find_first_char_test),
		!forked_test(ft_strrchr_find_last_char_test),
		!forked_test(ft_strrchr_not_found_test),
		!forked_test(ft_strrchr_uchar_test),
		!forked_test(ft_strrchr_over_255_test),
		forked_test(ft_strrchr_null_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_strrchr", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_strncmp tests                                */
/* ************************************************************************** */

static void	ft_strncmp_equal_test(void) {
	if (ft_strncmp("Hello", "Hello", 5))
		abort();
}

static void	ft_strncmp_different_test(void) {
	if (!ft_strncmp("Hello", "World", 5))
		abort();
}

static void	ft_strncmp_partial_test(void) {
	if (ft_strncmp("Hello", "Help", 3))
		abort();
}

static void	ft_strncmp_n_zero_test(void) {
	if (ft_strncmp("Hello", "World", 0))
		abort();
}

static void	ft_strncmp_with_null_test(void) {
	if (ft_strncmp("Hello", "Hello\0test", 10))
		abort();
}

static void	ft_strncmp_n_gt_strlen_test(void) {
	if (ft_strncmp("abc", "abcd", 5) >= 0)
		abort();
}

static void	ft_strncmp_uchar_test(void) {
	if (ft_strncmp("test\200", "test\0", 6) <= 0)
		abort();
}

static void	ft_strncmp_uchar_255_test(void) {
	if (ft_strncmp("test\xff", "test\x7f", 6) <= 0)
		abort();
}

static void	ft_strncmp_size_max_test(void) {
	const size_t	size_max = ~(size_t)0;
	if (ft_strncmp("abc", "abc", size_max))
		abort();
}

static void	ft_strncmp_null_test(void) {
	ft_strncmp(NULL, "abc", 3);
}

static void	test_ft_strncmp(void) {
	const char		*tests[] = {
		"equal strings",
		"different strings",
		"partial compare",
		"n = 0",
		"with \\0",
		"n > strlen",
		"unsigned char comparison",
		"unsigned char 255 vs 127",
		"SIZE_MAX",
		"NULL (should crash)"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_strncmp_equal_test),
		!forked_test(ft_strncmp_different_test),
		!forked_test(ft_strncmp_partial_test),
		!forked_test(ft_strncmp_n_zero_test),
		!forked_test(ft_strncmp_with_null_test),
		!forked_test(ft_strncmp_n_gt_strlen_test),
		!forked_test(ft_strncmp_uchar_test),
		!forked_test(ft_strncmp_uchar_255_test),
		!forked_test(ft_strncmp_size_max_test),
		forked_test(ft_strncmp_null_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_strncmp", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_memchr tests                                 */
/* ************************************************************************** */

static void	ft_memchr_find_o_test(void) {
	const char *str = "Hello, World!";
	if (ft_memchr(str, 'o', 13) != memchr(str, 'o', 13))
		abort();
}

static void	ft_memchr_find_W_test(void) {
	const char *str = "Hello, World!";
	if (ft_memchr(str, 'W', 13) != memchr(str, 'W', 13))
		abort();
}

static void	ft_memchr_not_found_test(void) {
	const char *str = "Hello, World!";
	if (ft_memchr(str, 'x', 13))
		abort();
}

static void	ft_memchr_n_zero_test(void) {
	const char *str = "Hello, World!";
	if (ft_memchr(str, 'H', 0))
		abort();
}

static void	ft_memchr_nullchar_test(void) {
	if (ft_memchr("Hello, World!", '\0', 14) != memchr("Hello, World!", '\0', 14))
		abort();
}

static void	ft_memchr_uchar_test(void) {
	const unsigned char bin[] = {0, 128, 255, 42};
	if (ft_memchr(bin, 255, sizeof(bin)) != memchr(bin, 255, sizeof(bin)))
		abort();
}

static void	ft_memchr_over_255_test(void) {
	const char *str = "Hello, World!";
	if (ft_memchr(str, 'o' + 256, 13) != memchr(str, 'o' + 256, 13))
		abort();
}

static void	test_ft_memchr(void) {
	const char		*tests[] = {
		"find last 'o'",
		"find 'W'",
		"find '\\0'",
		"not found 'x'",
		"find unsigned char (200)",
		"find c > 255 (wraps to char)",
		"NULL"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_memchr_find_o_test),
		!forked_test(ft_memchr_find_W_test),
		!forked_test(ft_memchr_not_found_test),
		!forked_test(ft_memchr_n_zero_test),
		!forked_test(ft_memchr_nullchar_test),
		!forked_test(ft_memchr_uchar_test),
		!forked_test(ft_memchr_over_255_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_memchr", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_memcmp tests                                 */
/* ************************************************************************** */

static void	ft_memcmp_equal_test(void) {
	if (ft_memcmp("Hello", "Hello", 5))
		abort();
}

static void	ft_memcmp_different_test(void) {
	if (!ft_memcmp("Hello", "World", 5))
		abort();
}

static void	ft_memcmp_n_zero_test(void) {
	if (ft_memcmp("Hello", "World", 0))
		abort();
}

static void	ft_memcmp_binary_test(void) {
	char buf1[] = {1, 2, 3, 4, 5};
	char buf2[] = {1, 2, 3, 4, 6};
	if (ft_memcmp(buf1, buf2, 5) >= 0)
		abort();
}

static void	ft_memcmp_uchar_gt_127_test(void) {
	char high1[] = {'A', '\200', 0};
	char high2[] = {'A', '\0', 0};
	if (ft_memcmp(high1, high2, 2) <= 0)
		abort();
}

static void	ft_memcmp_symmetry_test(void) {
	char high1[] = {'A', '\200', 0};
	char high2[] = {'A', '\0', 0};
	if (ft_memcmp(high2, high1, 2) >= 0)
		abort();
}

static void	test_ft_memcmp(void) {
	const char		*tests[] = {
		"equal",
		"different",
		"n = 0",
		"binary data",
		"unsigned char > 127 (\\200 vs \\0)",
		"unsigned char symmetry"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_memcmp_equal_test),
		!forked_test(ft_memcmp_different_test),
		!forked_test(ft_memcmp_n_zero_test),
		!forked_test(ft_memcmp_binary_test),
		!forked_test(ft_memcmp_uchar_gt_127_test),
		!forked_test(ft_memcmp_symmetry_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_memcmp", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_strnstr tests                                */
/* ************************************************************************** */

static void	ft_strnstr_find_world_test(void) {
	const char *haystack = "Hello, World!";
	if (!ft_strnstr(haystack, "World", 13))
		abort();
}

static void	ft_strnstr_find_o_test(void) {
	const char *haystack = "Hello, World!";
	if (!ft_strnstr(haystack, "o", 13))
		abort();
}

static void	ft_strnstr_not_found_test(void) {
	const char *haystack = "Hello, World!";
	if (ft_strnstr(haystack, "xyz", 13))
		abort();
}

static void	ft_strnstr_empty_needle_test(void) {
	const char *haystack = "Hello, World!";
	if (ft_strnstr(haystack, "", 13) != haystack)
		abort();
}

static void	ft_strnstr_len_too_short_test(void) {
	if (ft_strnstr("Hello, World!", "World", 5))
		abort();
}

static void	ft_strnstr_len_zero_test(void) {
	if (ft_strnstr("Hello, World!", "World", 0))
		abort();
}

static void	ft_strnstr_needle_cut_test(void) {
	if (ft_strnstr("Hello World", "World", 8))
		abort();
}

static void	ft_strnstr_needle_longer_test(void) {
	if (ft_strnstr("Hi", "Hello", 10))
		abort();
}

static void	test_ft_strnstr(void) {
	const char		*tests[] = {
		"find 'World'",
		"find 'o'",
		"not found",
		"empty needle",
		"len too short",
		"len = 0",
		"needle cut by len",
		"needle longer than haystack"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_strnstr_find_world_test),
		!forked_test(ft_strnstr_find_o_test),
		!forked_test(ft_strnstr_not_found_test),
		!forked_test(ft_strnstr_empty_needle_test),
		!forked_test(ft_strnstr_len_too_short_test),
		!forked_test(ft_strnstr_len_zero_test),
		!forked_test(ft_strnstr_needle_cut_test),
		!forked_test(ft_strnstr_needle_longer_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_strnstr", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_atoi tests                                   */
/* ************************************************************************** */

static void	ft_atoi_whitespace_plus_test(void) {
	if (ft_atoi("  \t\n\v\f\r +42   $") != 42)
		abort();
}

static void	ft_atoi_whitespace_minus_test(void) {
	if (ft_atoi("\v \t\r  \n    \f-42   *") != -42)
		abort();
}

static void	ft_atoi_zero_test(void) {
	if (ft_atoi("0"))
		abort();
}

static void	ft_atoi_double_plus_test(void) {
	if (ft_atoi("++2"))
		abort();
}

static void	ft_atoi_double_minus_test(void) {
	if (ft_atoi("--4"))
		abort();
}

static void	ft_atoi_minus_plus_test(void) {
	if (ft_atoi("-+9"))
		abort();
}

static void	ft_atoi_plus_minus_test(void) {
	if (ft_atoi("+-8"))
		abort();
}

static void	ft_atoi_minus_letter_test(void) {
	if (ft_atoi(" \t-R66"))
		abort();
}

static void	ft_atoi_int_max_test(void) {
	if (ft_atoi("2147483647") != 2147483647)
		abort();
}

static void	ft_atoi_int_min_test(void) {
	if (ft_atoi("-2147483648") != -2147483648)
		abort();
}

static void	ft_atoi_empty_test(void) {
	if (ft_atoi(""))
		abort();
}

static void	ft_atoi_lone_sign_test(void) {
	if (ft_atoi("+") || ft_atoi("-"))
		abort();
}

static void	ft_atoi_leading_zeros_test(void) {
	if (ft_atoi("0042") != 42)
		abort();
}

static void	ft_atoi_trailing_junk_test(void) {
	if (ft_atoi("42abc") != 42)
		abort();
}

static void	ft_atoi_null_test(void) {
	ft_atoi(NULL);
}

static void	test_ft_atoi(void) {
	const char		*tests[] = {
		"'  \\t\\n\\v\\f\\r +42   $'",
		"'\\v \\t\\r  \\n    \\f-42   *'",
		"'0'",
		"'++2'",
		"'--4'",
		"'-+9'",
		"'+-8'",
		"' \\t-R66'",
		"INT_MAX",
		"INT_MIN",
		"empty string",
		"lone sign ('+', '-')",
		"leading zeros ('0042')",
		"trailing junk ('42abc')",
		"NULL (should crash)"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_atoi_whitespace_plus_test),
		!forked_test(ft_atoi_whitespace_minus_test),
		!forked_test(ft_atoi_zero_test),
		!forked_test(ft_atoi_double_plus_test),
		!forked_test(ft_atoi_double_minus_test),
		!forked_test(ft_atoi_minus_plus_test),
		!forked_test(ft_atoi_plus_minus_test),
		!forked_test(ft_atoi_minus_letter_test),
		!forked_test(ft_atoi_int_max_test),
		!forked_test(ft_atoi_int_min_test),
		!forked_test(ft_atoi_empty_test),
		!forked_test(ft_atoi_lone_sign_test),
		!forked_test(ft_atoi_leading_zeros_test),
		!forked_test(ft_atoi_trailing_junk_test),
		forked_test(ft_atoi_null_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_atoi", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_calloc tests                                 */
/* ************************************************************************** */

static void	ft_calloc_all_zeros_test(void) {
	int *arr = ft_calloc(5, sizeof(int));
	for (int i = 0; i < 5; i++)
		if (arr[i] != 0)
			abort();
	free(arr);
}

static void	ft_calloc_allocated_test(void) {
	char *str = ft_calloc(10, sizeof(char));
	if (!str)
		abort();
	free(str);
}

static void	ft_calloc_overflow_test(void) {
	const size_t	size_max = ~(size_t)0;
	char			*ptr = ft_calloc(size_max >> 1, size_max >> 1);
	if (ptr)
		abort();
}

static void	ft_calloc_zero_count_test(void) {
	g_malloc_count = 0;
	g_malloc_fail_at = 0;
	g_malloc_wrap_enabled = 1;
	void *ptr = ft_calloc(0, 5);
	g_malloc_wrap_enabled = 0;
	if (ptr && !g_malloc_zero)
		abort();
	free(ptr);
}

static void	ft_calloc_zero_size_test(void) {
	g_malloc_count = 0;
	g_malloc_fail_at = 0;
	g_malloc_wrap_enabled = 1;
	void *ptr = ft_calloc(5, 0);
	g_malloc_wrap_enabled = 0;
	if (ptr && !g_malloc_zero)
		abort();
	free(ptr);
}

static void	test_ft_calloc(void) {
	const char		*tests[] = {
		"all zeros'",
		"allocated",
		"overflow",
		"count = 0",
		"size = 0"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_calloc_all_zeros_test),
		!forked_test(ft_calloc_allocated_test),
		!forked_test(ft_calloc_overflow_test),
		!forked_test(ft_calloc_zero_count_test),
		!forked_test(ft_calloc_zero_size_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_calloc", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_strdup tests                                 */
/* ************************************************************************** */

static void	ft_strdup_basic_test(void) {
	char *dup = ft_strdup("Hello");
	if (strcmp(dup, "Hello"))
		abort();
	free(dup);
}

static void	ft_strdup_short_test(void) {
	char *dup = ft_strdup("42");
	if (strcmp(dup, "42"))
		abort();
	free(dup);
}

static void	ft_strdup_empty_test(void) {
	char *dup = ft_strdup("");
	dup[0] = 'a';
	free(dup);
}

static void	test_ft_strdup(void) {
	const char		*tests[] = {
		"basic dup",
		"short string",
		"empty string"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_strdup_basic_test),
		!forked_test(ft_strdup_short_test),
		!forked_test(ft_strdup_empty_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_strdup", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_substr tests                                 */
/* ************************************************************************** */

static void	ft_substr_basic_test(void) {
	char *sub = ft_substr("Hello, World!", 7, 5);
	if (strcmp(sub, "World"))
		abort();
	free(sub);
}

static void	ft_substr_from_start_test(void) {
	char *sub = ft_substr("Hello", 0, 3);
	if (strcmp(sub, "Hel"))
		abort();
	free(sub);
}

static void	ft_substr_start_gt_len_test(void) {
	char *sub = ft_substr("Hello", 10, 5);
	if (strcmp(sub, ""))
		abort();
	free(sub);
}

static void	ft_substr_len_too_long_test(void) {
	char *sub = ft_substr("Hello", 2, 100);
	if (strcmp(sub, "llo"))
		abort();
	free(sub);
}

static void	ft_substr_len_zero_test(void) {
	char *sub = ft_substr("test", 0, 0);
	if (strcmp(sub, ""))
		abort();
	free(sub);
}

static void	ft_substr_empty_test(void) {
	char *sub1 = ft_substr("Hello", 10, 5);
	char *sub2 = ft_substr("test", 0, 0);
	sub1[0] = 'a';
	free(sub1);
	sub2[0] = 'a';
	free(sub2);
}

static void	ft_substr_start_past_end_test(void) {
	char *res = ft_substr("", 1, 1);
	if (!res || res[0] != '\0')
		abort();
	free(res);
	res = ft_substr("abc", 5, 2);
	if (!res || res[0] != '\0')
		abort();
	free(res);
}

static void	ft_substr_start_max_test(void) {
	char *res = ft_substr("abc", ~(unsigned int)0, 1);
	if (!res || res[0] != '\0')
		abort();
	free(res);
}

static void	ft_substr_len_size_max_test(void) {
	const size_t size_max = ~(size_t)0;
	char *res = ft_substr("Hello", 0, size_max);
	if (!res || strcmp(res, "Hello"))
		abort();
	free(res);
}

static void	ft_substr_no_modify_test(void) {
	char s[] = "Hello, World!";
	char *sub = ft_substr(s, 7, 5);
	if (strcmp(s, "Hello, World!"))
		abort();
	free(sub);
}

static void	ft_substr_null_test(void) {
	ft_substr(NULL, 0, 5);
}

static void	test_ft_substr(void) {
	const char		*tests[] = {
		"basic substr",
		"from start",
		"start > len",
		"len too long",
		"len = 0",
		"empty string alloc",
		"start past end",
		"start = UINT_MAX",
		"len = SIZE_MAX",
		"source not modified",
		"NULL"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_substr_basic_test),
		!forked_test(ft_substr_from_start_test),
		!forked_test(ft_substr_start_gt_len_test),
		!forked_test(ft_substr_len_too_long_test),
		!forked_test(ft_substr_len_zero_test),
		!forked_test(ft_substr_empty_test),
		!forked_test(ft_substr_start_past_end_test),
		!forked_test(ft_substr_start_max_test),
		!forked_test(ft_substr_len_size_max_test),
		!forked_test(ft_substr_no_modify_test),
		!forked_test(ft_substr_null_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_substr", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_strjoin tests                                */
/* ************************************************************************** */

static void	ft_strjoin_basic_test(void) {
	char *joined = ft_strjoin("Hello", " World");
	if (strcmp(joined, "Hello World"))
		abort();
	free(joined);
}

static void	ft_strjoin_empty_s1_test(void) {
	char *joined = ft_strjoin("", "Hello");
	if (strcmp(joined, "Hello"))
		abort();
	free(joined);
}

static void	ft_strjoin_empty_s2_test(void) {
	char *joined = ft_strjoin("Hello", "");
	if (strcmp(joined, "Hello"))
		abort();
	free(joined);
}

static void	ft_strjoin_short_test(void) {
	char *joined = ft_strjoin("42", "!");
	if (strcmp(joined, "42!"))
		abort();
	free(joined);
}

static void	ft_strjoin_null_s1_test(void) {
	ft_strjoin(NULL, "test");
}

static void	ft_strjoin_null_s2_test(void) {
	ft_strjoin("test", NULL);
}

static void	ft_strjoin_null_both_test(void) {
	ft_strjoin(NULL, NULL);
}

static void	ft_strjoin_no_modify_test(void) {
	char s1[] = "Hello";
	char s2[] = " World";
	char *joined = ft_strjoin(s1, s2);
	if (strcmp(s1, "Hello") || strcmp(s2, " World"))
		abort();
	free(joined);
}

static void	test_ft_strjoin(void) {
	const char		*tests[] = {
		"basic join",
		"empty s1",
		"empty s2",
		"short strings",
		"sources not modified",
		"NULL s1",
		"NULL s2",
		"both NULL"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_strjoin_basic_test),
		!forked_test(ft_strjoin_empty_s1_test),
		!forked_test(ft_strjoin_empty_s2_test),
		!forked_test(ft_strjoin_short_test),
		!forked_test(ft_strjoin_no_modify_test),
		!forked_test(ft_strjoin_null_s1_test),
		!forked_test(ft_strjoin_null_s2_test),
		!forked_test(ft_strjoin_null_both_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_strjoin", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_strtrim tests                                */
/* ************************************************************************** */

static void	ft_strtrim_spaces_test(void) {
	char *trimmed = ft_strtrim("   Hello   ", " ");
	if (strcmp(trimmed, "Hello"))
		abort();
	free(trimmed);
}

static void	ft_strtrim_custom_set_test(void) {
	char *trimmed = ft_strtrim("xxxHelloxxx", "x");
	if (strcmp(trimmed, "Hello"))
		abort();
	free(trimmed);
}

static void	ft_strtrim_no_trim_test(void) {
	char *trimmed = ft_strtrim("Hello", "xyz");
	if (strcmp(trimmed, "Hello"))
		abort();
	free(trimmed);
}

static void	ft_strtrim_multiple_test(void) {
	char *trimmed = ft_strtrim("abcdcba", "abc");
	if (strcmp(trimmed, "d"))
		abort();
	free(trimmed);
}

static void	ft_strtrim_no_set_test(void) {
	char *trimmed = ft_strtrim("Hello", "");
	if (strcmp(trimmed, "Hello"))
		abort();
	free(trimmed);
}

static void	ft_strtrim_empty_string_test(void) {
	ft_strtrim("", " ");
}

static void	ft_strtrim_all_trim_test(void) {
	char *trimmed = ft_strtrim("xxxxx", "x");
	if (!trimmed || trimmed[0])
		abort();
	free(trimmed);
}

static void	ft_strtrim_null_input_test(void) {
	ft_strtrim(NULL, " ");
}

static void	ft_strtrim_null_set_test(void) {
	ft_strtrim("test", NULL);
}

static void	ft_strtrim_null_both_test(void) {
	ft_strtrim(NULL, NULL);
}

static void	ft_strtrim_no_modify_test(void) {
	char s1[] = "   Hello   ";
	char *trimmed = ft_strtrim(s1, " ");
	if (strcmp(s1, "   Hello   "))
		abort();
	free(trimmed);
}

static void	test_ft_strtrim(void) {
	const char		*tests[] = {
		"spaces",
		"custom set",
		"no trim",
		"multiple occurrences",
		"no set",
		"empty string",
		"all trim",
		"NULL input",
		"NULL set",
		"both NULL",
		"source not modified"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_strtrim_spaces_test),
		!forked_test(ft_strtrim_custom_set_test),
		!forked_test(ft_strtrim_no_trim_test),
		!forked_test(ft_strtrim_multiple_test),
		!forked_test(ft_strtrim_no_set_test),
		!forked_test(ft_strtrim_empty_string_test),
		!forked_test(ft_strtrim_all_trim_test),
		!forked_test(ft_strtrim_null_input_test),
		!forked_test(ft_strtrim_null_set_test),
		!forked_test(ft_strtrim_null_both_test),
		!forked_test(ft_strtrim_no_modify_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_strtrim", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_split tests                                  */
/* ************************************************************************** */

static void	ft_split_basic_test(void) {
	char **arr = ft_split("Hello World 42", ' ');
	if (!arr || strcmp(arr[0], "Hello") || strcmp(arr[1], "World") || strcmp(arr[2], "42") || arr[3])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_multiple_delim_test(void) {
	char **arr = ft_split("___Hello___World___", '_');
	if (!arr || strcmp(arr[0], "Hello") || strcmp(arr[1], "World") || arr[2])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_empty_string_test(void) {
	char **arr = ft_split("", ' ');
	if (!arr || arr[0])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_empty_with_x_test(void) {
	char **arr = ft_split("", 'x');
	if (!arr || arr[0])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_all_delim_test(void) {
	char **arr = ft_split("xxx", 'x');
	if (!arr || arr[0])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_delim_null_test(void) {
	char **arr = ft_split("Hello", '\0');
	if (!arr || strcmp(arr[0], "Hello") || arr[1])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_empty_with_null_test(void) {
	char **arr = ft_split("", '\0');
	if (!arr || arr[0])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_only_spaces_test(void) {
	char **arr = ft_split("   ", ' ');
	if (!arr || arr[0])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_no_delim_found_test(void) {
	char **arr = ft_split("HelloWorld", ' ');
	if (!arr || strcmp(arr[0], "HelloWorld") || arr[1])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_consecutive_delim_test(void) {
	char **arr = ft_split("Hello   World", ' ');
	if (!arr || strcmp(arr[0], "Hello") || strcmp(arr[1], "World") || arr[2])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_delim_255_test(void) {
	char **arr = ft_split("Hello\xFFWorld", (char)255);
	if (!arr || strcmp(arr[0], "Hello") || strcmp(arr[1], "World") || arr[2])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_delim_minus1_test(void) {
	char **arr = ft_split("Hello\xFFWorld", (char)-1);
	if (!arr || strcmp(arr[0], "Hello") || strcmp(arr[1], "World") || arr[2])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_null_test(void) {
	ft_split(NULL, ' ');
}

static void	ft_split_malloc_fail_test(void) {
	g_malloc_count = 0;
	++g_malloc_fail_at;
	char **result = ft_split("Hello World Test", ' ');
	(void)result;
}

static void	test_ft_split(void) {
	const char		*tests[] = {
		"basic split",
		"multiple delimiters",
		"empty string",
		"empty string with 'x'",
		"all delimiters",
		"delimiter = '\\0'",
		"empty string with '\\0'",
		"only spaces",
		"no delimiter found",
		"consecutive delimiters",
		"delimiter = 255 (\\xFF)",
		"delimiter = -1",
		"NULL",
		"malloc fail #1 (array)",
		"malloc fail #2 (word 1)",
		"malloc fail #3 (word 2)",
		"malloc fail #4 (word 3)",
		"malloc fail #5 (other)"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	int				passed[num_tests];

	passed[0] = !forked_test(ft_split_basic_test);
	passed[1] = !forked_test(ft_split_multiple_delim_test);
	passed[2] = !forked_test(ft_split_empty_string_test);
	passed[3] = !forked_test(ft_split_empty_with_x_test);
	passed[4] = !forked_test(ft_split_all_delim_test);
	passed[5] = !forked_test(ft_split_delim_null_test);
	passed[6] = !forked_test(ft_split_empty_with_null_test);
	passed[7] = !forked_test(ft_split_only_spaces_test);
	passed[8] = !forked_test(ft_split_no_delim_found_test);
	passed[9] = !forked_test(ft_split_consecutive_delim_test);
	passed[10] = !forked_test(ft_split_delim_255_test);
	passed[11] = !forked_test(ft_split_delim_minus1_test);
	passed[12] = !forked_test(ft_split_null_test);
	g_malloc_wrap_enabled = 1;
	g_malloc_fail_at = 0;
	for (int i = 13; i < 18; i++)
		passed[i] = !forked_test(ft_split_malloc_fail_test);
	g_malloc_wrap_enabled = 0;
	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_split", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_itoa tests                                   */
/* ************************************************************************** */

static void	ft_itoa_42_test(void) {
	char *str = ft_itoa(42);
	if (strcmp(str, "42"))
		abort();
	free(str);
}

static void	ft_itoa_minus_42_test(void) {
	char *str = ft_itoa(-42);
	if (strcmp(str, "-42"))
		abort();
	free(str);
}

static void	ft_itoa_zero_test(void) {
	char *str = ft_itoa(0);
	if (strcmp(str, "0"))
		abort();
	free(str);
}

static void	ft_itoa_int_min_test(void) {
	char *str = ft_itoa(-2147483648);
	if (strcmp(str, "-2147483648"))
		abort();
	free(str);
}

static void	ft_itoa_int_max_test(void) {
	char *str = ft_itoa(2147483647);
	if (strcmp(str, "2147483647"))
		abort();
	free(str);
}

static void	test_ft_itoa(void) {
	const char		*tests[] = {
		"42",
		"-42",
		"0",
		"INT_MIN",
		"INT_MAX"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_itoa_42_test),
		!forked_test(ft_itoa_minus_42_test),
		!forked_test(ft_itoa_zero_test),
		!forked_test(ft_itoa_int_min_test),
		!forked_test(ft_itoa_int_max_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_itoa", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_strmapi tests                                */
/* ************************************************************************** */

static char	mapi_func(unsigned int i, char c) {
	return c + i;
}

static void	ft_strmapi_basic_test(void) {
	char *result = ft_strmapi("abc", mapi_func);
	if (!result || result[0] != 'a' || result[1] != 'c' || result[2] != 'e')
		abort();
	free(result);
}

static void	ft_strmapi_empty_test(void) {
	char *result = ft_strmapi("", mapi_func);
	if (!result || strcmp(result, ""))
		abort();
	free(result);
}

static void	ft_strmapi_null_input_test(void) {
	ft_strmapi(NULL, mapi_func);
}

static void	ft_strmapi_null_func_test(void) {
	ft_strmapi("test", NULL);
}

static void	ft_strmapi_null_both_test(void) {
	ft_strmapi(NULL, NULL);
}

static void	ft_strmapi_no_modify_test(void) {
	char s[] = "abc";
	char *result = ft_strmapi(s, mapi_func);
	if (strcmp(s, "abc"))
		abort();
	free(result);
}

static void	test_ft_strmapi(void) {
	const char		*tests[] = {
		"basic",
		"empty string",
		"NULL input",
		"f = NULL",
		"both NULL",
		"source not modified"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_strmapi_basic_test),
		!forked_test(ft_strmapi_empty_test),
		!forked_test(ft_strmapi_null_input_test),
		!forked_test(ft_strmapi_null_func_test),
		!forked_test(ft_strmapi_null_both_test),
		!forked_test(ft_strmapi_no_modify_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_strmapi", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                            ft_striteri tests                               */
/* ************************************************************************** */

static void	iteri_func(unsigned int i, char *c) {
	*c = *c + i;
}

static void	ft_striteri_basic_test(void) {
	char str[] = "abc";
	ft_striteri(str, iteri_func);
	if (str[0] != 'a' || str[1] != 'c' || str[2] != 'e')
		abort();
}

static void	ft_striteri_null_input_test(void) {
	ft_striteri(NULL, iteri_func);
}

static void	ft_striteri_null_func_test(void) {
	ft_striteri("test", NULL);
}

static void	ft_striteri_null_both_test(void) {
	ft_striteri(NULL, NULL);
}

static void	test_ft_striteri(void) {
	const char		*tests[] = {
		"basic",
		"NULL input",
		"f = NULL",
		"both NULL"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_striteri_basic_test),
		!forked_test(ft_striteri_null_input_test),
		!forked_test(ft_striteri_null_func_test),
		!forked_test(ft_striteri_null_both_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_striteri", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                        ft_putchar_fd tests                                 */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>

static int	test_fd_output(void (*func)(void *, int), void *input, 
							const char *expected, size_t read_len) {
	char	buf[50] = {0};
	char	tmp[] = "/tmp/test_fd_XXXXXX";
	int		fd = mkstemp(tmp);
	int		result;

	if (fd == -1)
		error_exit("libft-fairy: mkstemp failed");
	func(input, fd);
	close(fd);
	fd = open(tmp, O_RDONLY);
	if (fd == -1)
		error_exit("libft-fairy: open failed");
	read(fd, buf, read_len);
	buf[read_len] = '\0';
	close(fd);
	unlink(tmp);
	result = !strcmp(buf, expected);
	return (result);
}

static void	wrapper_putchar(void *c, int fd) {
	ft_putchar_fd(*(char *)c, fd);
}

static void	wrapper_putchar_str(void *s, int fd) {
	char *str = (char *)s;
	for (int i = 0; str[i]; i++)
		ft_putchar_fd(str[i], fd);
}

static void	ft_putchar_AB_test(void) {
	if (!test_fd_output(wrapper_putchar_str, "AB", "AB", 2))
		abort();
}

static void	ft_putchar_0_test(void) {
	char c = '0';
	if (!test_fd_output(wrapper_putchar, &c, "0", 1))
		abort();
}

static void	test_ft_putchar_fd(void) {
	const char		*tests[] = {
		"'A' and 'B'",
		"'0'"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_putchar_AB_test),
		!forked_test(ft_putchar_0_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_putchar_fd", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                        ft_putstr_fd tests                                  */
/* ************************************************************************** */

static void	wrapper_putstr(void *s, int fd) {
	ft_putstr_fd((char *)s, fd);
}

static void	ft_putstr_hello_test(void) {
	if (!test_fd_output(wrapper_putstr, "Hello, World!", "Hello, World!", 13))
		abort();
}

static void	ft_putstr_42_test(void) {
	if (!test_fd_output(wrapper_putstr, "42", "42", 2))
		abort();
}

static void	ft_putstr_null_test(void) {
	char	tmp[] = "/tmp/putstr_fd_null_test_XXXXXX";
	int		fd = mkstemp(tmp);
	ft_putstr_fd(NULL, fd);
	close(fd);
	unlink(tmp);
}

static void	test_ft_putstr_fd(void) {
	const char		*tests[] = {
		"'Hello, World!'",
		"'42'",
		"NULL"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_putstr_hello_test),
		!forked_test(ft_putstr_42_test),
		!forked_test(ft_putstr_null_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_putstr_fd", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                        ft_putendl_fd tests                                 */
/* ************************************************************************** */

static void	wrapper_putendl(void *s, int fd) {
	ft_putendl_fd((char *)s, fd);
}

static void	ft_putendl_hello_test(void) {
	if (!test_fd_output(wrapper_putendl, "Hello", "Hello\n", 6))
		abort();
}

static void	ft_putendl_empty_test(void) {
	if (!test_fd_output(wrapper_putendl, "", "\n", 1))
		abort();
}

static void	ft_putendl_null_test(void) {
	char	tmp[] = "/tmp/putendl_fd_null_test_XXXXXX";
	int		fd = mkstemp(tmp);
	ft_putendl_fd(NULL, fd);
	close(fd);
	unlink(tmp);
}

static void	test_ft_putendl_fd(void) {
	const char		*tests[] = {
		"'Hello' with newline",
		"empty string with newline",
		"NULL"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_putendl_hello_test),
		!forked_test(ft_putendl_empty_test),
		!forked_test(ft_putendl_null_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_putendl_fd", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                        ft_putnbr_fd tests                                  */
/* ************************************************************************** */

static void	wrapper_putnbr(void *n, int fd) {
	ft_putnbr_fd(*(int *)n, fd);
}

static void	ft_putnbr_42_test(void) {
	if (!test_fd_output(wrapper_putnbr, &(int){42}, "42", 2))
		abort();
}

static void	ft_putnbr_minus_42_test(void) {
	if (!test_fd_output(wrapper_putnbr, &(int){-42}, "-42", 3))
		abort();
}

static void	ft_putnbr_zero_test(void) {
	if (!test_fd_output(wrapper_putnbr, &(int){0}, "0", 1))
		abort();
}

static void	ft_putnbr_int_min_test(void) {
	if (!test_fd_output(wrapper_putnbr, &(int){-2147483648}, "-2147483648", 11))
		abort();
}

static void	ft_putnbr_int_max_test(void) {
	if (!test_fd_output(wrapper_putnbr, &(int){2147483647}, "2147483647", 10))
		abort();
}

static void	test_ft_putnbr_fd(void) {
	const char		*tests[] = {
		"42",
		"-42",
		"0",
		"INT_MIN",
		"INT_MAX"
	};
	const size_t	num_tests = sizeof(tests) / sizeof(*tests);
	const int		passed[] = {
		!forked_test(ft_putnbr_42_test),
		!forked_test(ft_putnbr_minus_42_test),
		!forked_test(ft_putnbr_zero_test),
		!forked_test(ft_putnbr_int_min_test),
		!forked_test(ft_putnbr_int_max_test)
	};

	if (!all_tests_passed(passed, num_tests) || VERBOSE)
		print_test_results("ft_putnbr_fd", num_tests, tests, passed);
}

/* ************************************************************************** */
/*                                 MAIN                                       */
/* ************************************************************************** */

int	main(void) {
	test_ft_isalpha();
	test_ft_isdigit();
	test_ft_isalnum();
	test_ft_isascii();
	test_ft_isprint();
	test_ft_strlen();
	test_ft_memset();
	test_ft_bzero();
	test_ft_memcpy();
	test_ft_memmove();
	test_ft_strlcpy();
	test_ft_strlcat();
	test_ft_toupper();
	test_ft_tolower();
	test_ft_strchr();
	test_ft_strrchr();
	test_ft_strncmp();
	test_ft_memchr();
	test_ft_memcmp();
	test_ft_strnstr();
	test_ft_atoi();
	test_ft_calloc();
	test_ft_strdup();
	test_ft_substr();
	test_ft_strjoin();
	test_ft_strtrim();
	test_ft_split();
	test_ft_itoa();
	test_ft_strmapi();
	test_ft_striteri();
	test_ft_putchar_fd();
	test_ft_putstr_fd();
	test_ft_putendl_fd();
	test_ft_putnbr_fd();
	return (g_tests_failed ? 1 : 0);
}
