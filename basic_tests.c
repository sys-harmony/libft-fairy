#include "libft_fairy.h"

/* ************************************************************************** */
/*                  ft_is* / ft_toupper / ft_tolower tests                    */
/* ************************************************************************** */

#include <ctype.h>
#include <stdio.h>

// Compares ft with the libc function ref for every value from EOF to 255.
// A character class function must return exactly 1 on a match (subject rule).
static void	test_char_func(char *function_name, int (*ft)(int), int (*ref)(int), int is_class) {
	char		label[64] = "EOF to 255";
	const char	*tests[] = {label};
	int			passed[] = {1};

	for (int c = -1; c < 256 && passed[0]; c++) {
		int	expected = is_class ? !!ref(c) : ref(c);
		int	got = ft(c);

		if (got != expected) {
			snprintf(label, sizeof(label), "EOF to 255  (c = %d: got %d, expected %d)", c, got, expected);
			passed[0] = 0;
		}
	}
	if (!passed[0] || VERBOSE)
		print_test_results(function_name, 1, tests, passed);
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

static void	ft_strlen_null_test(void) {
	ft_strlen(NULL);
}

static void	test_ft_strlen(void) {
	const t_test	tests[] = {
		{"empty string", ft_strlen_empty_test},
		{"'hello'", ft_strlen_hello_test},
		{"NULL (should crash)", ft_strlen_null_test, .crash = 1}
	};

	run_tests("ft_strlen", tests, ARRAY_SIZE(tests));
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
	const t_test	tests[] = {
		{"basic memset", ft_memset_basic_test},
		{"with 0", ft_memset_zero_test},
		{"unsigned char (200)", ft_memset_uchar_test},
		{"NULL (should crash)", ft_memset_null_test, .crash = 1}
	};

	run_tests("ft_memset", tests, ARRAY_SIZE(tests));
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
	const t_test	tests[] = {
		{"bzero 5 bytes", ft_bzero_5_bytes_test},
		{"bzero all", ft_bzero_all_test}
	};

	run_tests("ft_bzero", tests, ARRAY_SIZE(tests));
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
	const t_test	tests[] = {
		{"basic memcpy", ft_memcpy_basic_test},
		{"partial copy", ft_memcpy_partial_test},
		{"return value", ft_memcpy_return_test},
		{"NULL dst and src, n = 0 (no crash)", ft_memcpy_null_n_zero_test},
		{"NULL (should crash)", ft_memcpy_null_test, .crash = 1}
	};

	run_tests("ft_memcpy", tests, ARRAY_SIZE(tests));
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
	const t_test	tests[] = {
		{"overlapping forward", ft_memmove_forward_test},
		{"overlapping backward", ft_memmove_backward_test},
		{"no overlap", ft_memmove_no_overlap_test},
		{"dst == src", ft_memmove_same_ptr_test},
		{"n = 0", ft_memmove_n_zero_test},
		{"minimal overlap", ft_memmove_minimal_overlap_test},
		{"return value", ft_memmove_return_test},
		{"NULL dst and src, n = 0 (no crash)", ft_memmove_null_n_zero_test},
		{"NULL (should crash)", ft_memmove_null_test, .crash = 1}
	};

	run_tests("ft_memmove", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                            ft_strlcpy tests                                */
/* ************************************************************************** */

static void	ft_strlcpy_basic_test(void) {
	char dst[10];
	size_t len = ft_strlcpy(dst, "Hello", sizeof(dst));
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
	if (len != 2 || strcmp(dst, "test"))
		abort();
}

static void	test_ft_strlcpy(void) {
	const t_test	tests[] = {
		{"basic copy", ft_strlcpy_basic_test},
		{"truncation", ft_strlcpy_truncation_test},
		{"size 0 (dst untouched)", ft_strlcpy_size_zero_test}
	};

	run_tests("ft_strlcpy", tests, ARRAY_SIZE(tests));
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
	const t_test	tests[] = {
		{"basic concat", ft_strlcat_basic_test},
		{"truncation", ft_strlcat_truncation_test},
		{"dstsize = strlen(dst)", ft_strlcat_dstsize_eq_dstlen_test},
		{"dstsize < strlen(dst)", ft_strlcat_dstsize_lt_dstlen_test},
		{"dstsize = 0", ft_strlcat_dstsize_zero_test},
		{"empty src", ft_strlcat_empty_src_test},
		{"empty dst", ft_strlcat_empty_dst_test},
		{"no room for concat", ft_strlcat_no_room_test}
	};

	run_tests("ft_strlcat", tests, ARRAY_SIZE(tests));
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
	const t_test	tests[] = {
		{"find 'o'", ft_strchr_find_o_test},
		{"find 'W'", ft_strchr_find_W_test},
		{"find '\\0'", ft_strchr_find_null_test},
		{"find first char", ft_strchr_find_first_char_test},
		{"find last char", ft_strchr_find_last_char_test},
		{"not found 'x'", ft_strchr_not_found_test},
		{"find unsigned char (200)", ft_strchr_uchar_test},
		{"find c > 255 (wraps to char)", ft_strchr_over_255_test},
		{"NULL (should crash)", ft_strchr_null_test, .crash = 1}
	};

	run_tests("ft_strchr", tests, ARRAY_SIZE(tests));
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
	const t_test	tests[] = {
		{"find last 'o'", ft_strrchr_find_last_o_test},
		{"find 'W'", ft_strrchr_find_W_test},
		{"find '\\0'", ft_strrchr_find_null_test},
		{"find first char", ft_strrchr_find_first_char_test},
		{"find last char", ft_strrchr_find_last_char_test},
		{"not found 'x'", ft_strrchr_not_found_test},
		{"find unsigned char (200)", ft_strrchr_uchar_test},
		{"find c > 255 (wraps to char)", ft_strrchr_over_255_test},
		{"NULL (should crash)", ft_strrchr_null_test, .crash = 1}
	};

	run_tests("ft_strrchr", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                            ft_strncmp tests                                */
/* ************************************************************************** */

static void	ft_strncmp_equal_test(void) {
	if (ft_strncmp("Hello", "Hello", 5))
		abort();
}

static void	ft_strncmp_different_test(void) {
	if (ft_strncmp("Hello", "World", 5) >= 0)
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

static void	ft_strncmp_size_max_test(void) {
	const size_t	size_max = ~(size_t)0;
	if (ft_strncmp("abc", "abc", size_max))
		abort();
}

static void	ft_strncmp_null_test(void) {
	ft_strncmp(NULL, "abc", 3);
}

static void	test_ft_strncmp(void) {
	const t_test	tests[] = {
		{"equal strings", ft_strncmp_equal_test},
		{"different strings (sign)", ft_strncmp_different_test},
		{"partial compare", ft_strncmp_partial_test},
		{"n = 0", ft_strncmp_n_zero_test},
		{"with \\0", ft_strncmp_with_null_test},
		{"n > strlen", ft_strncmp_n_gt_strlen_test},
		{"unsigned char comparison", ft_strncmp_uchar_test},
		{"SIZE_MAX", ft_strncmp_size_max_test},
		{"NULL (should crash)", ft_strncmp_null_test, .crash = 1}
	};

	run_tests("ft_strncmp", tests, ARRAY_SIZE(tests));
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
	const t_test	tests[] = {
		{"find first 'o'", ft_memchr_find_o_test},
		{"find 'W'", ft_memchr_find_W_test},
		{"not found 'x'", ft_memchr_not_found_test},
		{"n = 0", ft_memchr_n_zero_test},
		{"find '\\0'", ft_memchr_nullchar_test},
		{"find unsigned char (255)", ft_memchr_uchar_test},
		{"find c > 255 (wraps to char)", ft_memchr_over_255_test}
	};

	run_tests("ft_memchr", tests, ARRAY_SIZE(tests));
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

static void	ft_memcmp_past_nul_test(void) {
	if (ft_memcmp("ab\0c", "ab\0d", 4) >= 0)
		abort();
}

static void	test_ft_memcmp(void) {
	const t_test	tests[] = {
		{"equal", ft_memcmp_equal_test},
		{"different", ft_memcmp_different_test},
		{"n = 0", ft_memcmp_n_zero_test},
		{"binary data", ft_memcmp_binary_test},
		{"unsigned char > 127 (\\200 vs \\0)", ft_memcmp_uchar_gt_127_test},
		{"unsigned char symmetry", ft_memcmp_symmetry_test},
		{"does not stop at '\\0'", ft_memcmp_past_nul_test}
	};

	run_tests("ft_memcmp", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                            ft_strnstr tests                                */
/* ************************************************************************** */

static void	ft_strnstr_find_world_test(void) {
	const char *haystack = "Hello, World!";
	if (ft_strnstr(haystack, "World", 13) != haystack + 7)
		abort();
}

static void	ft_strnstr_find_o_test(void) {
	const char *haystack = "Hello, World!";
	if (ft_strnstr(haystack, "o", 13) != haystack + 4)
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

static void	ft_strnstr_partial_match_test(void) {
	const char *haystack = "aaabc";
	if (ft_strnstr(haystack, "aab", 5) != haystack + 1)
		abort();
}

static void	ft_strnstr_len_exact_test(void) {
	const char *haystack = "Hello World";
	if (ft_strnstr(haystack, "World", 11) != haystack + 6)
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
	const t_test	tests[] = {
		{"find 'World'", ft_strnstr_find_world_test},
		{"find 'o'", ft_strnstr_find_o_test},
		{"not found", ft_strnstr_not_found_test},
		{"empty needle", ft_strnstr_empty_needle_test},
		{"match after a partial match", ft_strnstr_partial_match_test},
		{"needle ends exactly at len", ft_strnstr_len_exact_test},
		{"len too short", ft_strnstr_len_too_short_test},
		{"len = 0", ft_strnstr_len_zero_test},
		{"needle cut by len", ft_strnstr_needle_cut_test},
		{"needle longer than haystack", ft_strnstr_needle_longer_test}
	};

	run_tests("ft_strnstr", tests, ARRAY_SIZE(tests));
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
	const t_test	tests[] = {
		{"'  \\t\\n\\v\\f\\r +42   $'", ft_atoi_whitespace_plus_test},
		{"'\\v \\t\\r  \\n    \\f-42   *'", ft_atoi_whitespace_minus_test},
		{"'0'", ft_atoi_zero_test},
		{"'++2'", ft_atoi_double_plus_test},
		{"'--4'", ft_atoi_double_minus_test},
		{"'-+9'", ft_atoi_minus_plus_test},
		{"'+-8'", ft_atoi_plus_minus_test},
		{"' \\t-R66'", ft_atoi_minus_letter_test},
		{"INT_MAX", ft_atoi_int_max_test},
		{"INT_MIN", ft_atoi_int_min_test},
		{"empty string", ft_atoi_empty_test},
		{"lone sign ('+', '-')", ft_atoi_lone_sign_test},
		{"leading zeros ('0042')", ft_atoi_leading_zeros_test},
		{"trailing junk ('42abc')", ft_atoi_trailing_junk_test},
		{"NULL (should crash)", ft_atoi_null_test, .crash = 1}
	};

	run_tests("ft_atoi", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                            ft_calloc tests                                 */
/* ************************************************************************** */

static void	ft_calloc_all_zeros_test(void) {
	int *arr = ft_calloc(5, sizeof(int));
	if (!arr || alloc_size(arr) < 5 * sizeof(int))
		abort();
	for (int i = 0; i < 5; i++)
		if (arr[i] != 0)
			abort();
	free(arr);
}

static void	ft_calloc_overflow_test(void) {
	const size_t	size_max = ~(size_t)0;
	char			*ptr = ft_calloc(size_max >> 1, size_max >> 1);
	if (ptr)
		abort();
}

// Subject: if nmemb or size is 0, return a unique pointer that can be passed to free()
static void	ft_calloc_zero_count_test(void) {
	void *ptr = ft_calloc(0, 5);
	if (!ptr)
		abort();
	free(ptr);
}

static void	ft_calloc_zero_size_test(void) {
	void *ptr = ft_calloc(5, 0);
	if (!ptr)
		abort();
	free(ptr);
}

static void	test_ft_calloc(void) {
	const t_test	tests[] = {
		{"all zeros", ft_calloc_all_zeros_test},
		{"overflow", ft_calloc_overflow_test},
		{"count = 0 (freeable pointer)", ft_calloc_zero_count_test},
		{"size = 0 (freeable pointer)", ft_calloc_zero_size_test}
	};

	run_tests("ft_calloc", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                            ft_strdup tests                                 */
/* ************************************************************************** */

static void	ft_strdup_basic_test(void) {
	check_str(ft_strdup("Hello"), "Hello");
}

static void	ft_strdup_empty_test(void) {
	check_str(ft_strdup(""), "");
}

static void	test_ft_strdup(void) {
	const t_test	tests[] = {
		{"basic dup", ft_strdup_basic_test},
		{"empty string", ft_strdup_empty_test}
	};

	run_tests("ft_strdup", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                            ft_substr tests                                 */
/* ************************************************************************** */

static void	ft_substr_basic_test(void) {
	check_str(ft_substr("Hello, World!", 7, 5), "World");
}

static void	ft_substr_from_start_test(void) {
	check_str(ft_substr("Hello", 0, 3), "Hel");
}

static void	ft_substr_start_gt_len_test(void) {
	check_str(ft_substr("Hello", 10, 5), "");
}

static void	ft_substr_len_too_long_test(void) {
	check_str(ft_substr("Hello", 2, 100), "llo");
}

static void	ft_substr_len_zero_test(void) {
	check_str(ft_substr("test", 0, 0), "");
}

static void	ft_substr_start_past_end_test(void) {
	check_str(ft_substr("", 1, 1), "");
	check_str(ft_substr("abc", 5, 2), "");
}

static void	ft_substr_start_max_test(void) {
	check_str(ft_substr("abc", ~(unsigned int)0, 1), "");
}

static void	ft_substr_len_size_max_test(void) {
	check_str(ft_substr("Hello", 0, ~(size_t)0), "Hello");
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
	const t_test	tests[] = {
		{"basic substr", ft_substr_basic_test},
		{"from start", ft_substr_from_start_test},
		{"start > len", ft_substr_start_gt_len_test},
		{"len too long", ft_substr_len_too_long_test},
		{"len = 0", ft_substr_len_zero_test},
		{"start past end", ft_substr_start_past_end_test},
		{"start = UINT_MAX", ft_substr_start_max_test},
		{"len = SIZE_MAX", ft_substr_len_size_max_test},
		{"source not modified", ft_substr_no_modify_test},
		{"NULL", ft_substr_null_test}
	};

	run_tests("ft_substr", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                            ft_strjoin tests                                */
/* ************************************************************************** */

static void	ft_strjoin_basic_test(void) {
	check_str(ft_strjoin("Hello", " World"), "Hello World");
}

static void	ft_strjoin_empty_s1_test(void) {
	check_str(ft_strjoin("", "Hello"), "Hello");
}

static void	ft_strjoin_empty_s2_test(void) {
	check_str(ft_strjoin("Hello", ""), "Hello");
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
	const t_test	tests[] = {
		{"basic join", ft_strjoin_basic_test},
		{"empty s1", ft_strjoin_empty_s1_test},
		{"empty s2", ft_strjoin_empty_s2_test},
		{"sources not modified", ft_strjoin_no_modify_test},
		{"NULL s1", ft_strjoin_null_s1_test},
		{"NULL s2", ft_strjoin_null_s2_test},
		{"both NULL", ft_strjoin_null_both_test}
	};

	run_tests("ft_strjoin", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                            ft_strtrim tests                                */
/* ************************************************************************** */

static void	ft_strtrim_spaces_test(void) {
	check_str(ft_strtrim("   Hello   ", " "), "Hello");
}

static void	ft_strtrim_custom_set_test(void) {
	check_str(ft_strtrim("xxxHelloxxx", "x"), "Hello");
}

static void	ft_strtrim_no_trim_test(void) {
	check_str(ft_strtrim("Hello", "xyz"), "Hello");
}

static void	ft_strtrim_multiple_test(void) {
	check_str(ft_strtrim("abcdcba", "abc"), "d");
}

static void	ft_strtrim_inner_kept_test(void) {
	check_str(ft_strtrim("xxHelloxWorldxx", "x"), "HelloxWorld");
}

static void	ft_strtrim_no_set_test(void) {
	check_str(ft_strtrim("Hello", ""), "Hello");
}

static void	ft_strtrim_empty_string_test(void) {
	check_str(ft_strtrim("", " "), "");
}

static void	ft_strtrim_all_trim_test(void) {
	check_str(ft_strtrim("xxxxx", "x"), "");
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
	const t_test	tests[] = {
		{"spaces", ft_strtrim_spaces_test},
		{"custom set", ft_strtrim_custom_set_test},
		{"no trim", ft_strtrim_no_trim_test},
		{"multiple occurrences", ft_strtrim_multiple_test},
		{"set chars kept inside", ft_strtrim_inner_kept_test},
		{"no set", ft_strtrim_no_set_test},
		{"empty string", ft_strtrim_empty_string_test},
		{"all trim", ft_strtrim_all_trim_test},
		{"NULL input", ft_strtrim_null_input_test},
		{"NULL set", ft_strtrim_null_set_test},
		{"both NULL", ft_strtrim_null_both_test},
		{"source not modified", ft_strtrim_no_modify_test}
	};

	run_tests("ft_strtrim", tests, ARRAY_SIZE(tests));
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

static void	ft_split_no_delim_found_test(void) {
	char **arr = ft_split("HelloWorld", ' ');
	if (!arr || strcmp(arr[0], "HelloWorld") || arr[1])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_delim_255_test(void) {
	char **arr = ft_split("Hello\xFFWorld", (char)255);
	if (!arr || strcmp(arr[0], "Hello") || strcmp(arr[1], "World") || arr[2])
		abort();
	safe_free_arr(&arr);
}

static void	ft_split_null_test(void) {
	ft_split(NULL, ' ');
}

// Subject: NULL if any allocation fails
static void	ft_split_malloc_fail_test(void) {
	g_malloc_wrap_enabled = 1;
	char **arr = ft_split("Hello World Test", ' ');
	g_malloc_wrap_enabled = 0;
	if (arr && g_malloc_count >= g_malloc_fail_at)
		abort();
	safe_free_arr(&arr);
}

static void	test_ft_split(void) {
	const t_test	tests[] = {
		{"basic split", ft_split_basic_test},
		{"multiple delimiters", ft_split_multiple_delim_test},
		{"empty string", ft_split_empty_string_test},
		{"all delimiters", ft_split_all_delim_test},
		{"delimiter = '\\0'", ft_split_delim_null_test},
		{"no delimiter found", ft_split_no_delim_found_test},
		{"delimiter = 255 (\\xFF)", ft_split_delim_255_test},
		{"NULL", ft_split_null_test},
		{"malloc fail #1 (array)", ft_split_malloc_fail_test, .fail_at = 1},
		{"malloc fail #2 (word 1)", ft_split_malloc_fail_test, .fail_at = 2},
		{"malloc fail #3 (word 2)", ft_split_malloc_fail_test, .fail_at = 3},
		{"malloc fail #4 (word 3)", ft_split_malloc_fail_test, .fail_at = 4},
		{"malloc fail #5 (other)", ft_split_malloc_fail_test, .fail_at = 5}
	};

	run_tests("ft_split", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                            ft_itoa tests                                   */
/* ************************************************************************** */

static void	ft_itoa_42_test(void) {
	check_str(ft_itoa(42), "42");
}

static void	ft_itoa_minus_42_test(void) {
	check_str(ft_itoa(-42), "-42");
}

static void	ft_itoa_zero_test(void) {
	check_str(ft_itoa(0), "0");
}

static void	ft_itoa_int_min_test(void) {
	check_str(ft_itoa(-2147483648), "-2147483648");
}

static void	ft_itoa_int_max_test(void) {
	check_str(ft_itoa(2147483647), "2147483647");
}

static void	test_ft_itoa(void) {
	const t_test	tests[] = {
		{"42", ft_itoa_42_test},
		{"-42", ft_itoa_minus_42_test},
		{"0", ft_itoa_zero_test},
		{"INT_MIN", ft_itoa_int_min_test},
		{"INT_MAX", ft_itoa_int_max_test}
	};

	run_tests("ft_itoa", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                            ft_strmapi tests                                */
/* ************************************************************************** */

static char	mapi_func(unsigned int i, char c) {
	return c + i;
}

static void	ft_strmapi_basic_test(void) {
	check_str(ft_strmapi("abc", mapi_func), "ace");
}

static void	ft_strmapi_empty_test(void) {
	check_str(ft_strmapi("", mapi_func), "");
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
	const t_test	tests[] = {
		{"basic", ft_strmapi_basic_test},
		{"empty string", ft_strmapi_empty_test},
		{"NULL input", ft_strmapi_null_input_test},
		{"f = NULL", ft_strmapi_null_func_test},
		{"both NULL", ft_strmapi_null_both_test},
		{"source not modified", ft_strmapi_no_modify_test}
	};

	run_tests("ft_strmapi", tests, ARRAY_SIZE(tests));
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
	const t_test	tests[] = {
		{"basic", ft_striteri_basic_test},
		{"NULL input", ft_striteri_null_input_test},
		{"f = NULL", ft_striteri_null_func_test},
		{"both NULL", ft_striteri_null_both_test}
	};

	run_tests("ft_striteri", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                        ft_putchar_fd tests                                 */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>

// Calls put(arg, fd) on a pipe and aborts unless exactly `expected` was written
static void	check_fd_output(void (*put)(void *, int), void *arg, const char *expected) {
	char	buf[64];
	int		fds[2];
	ssize_t	len;

	if (pipe(fds) == -1)
		error_exit("libft-fairy: pipe failed");
	put(arg, fds[1]);
	close(fds[1]);
	len = read(fds[0], buf, sizeof(buf));
	close(fds[0]);
	if (len != (ssize_t)strlen(expected) || memcmp(buf, expected, len))
		abort();
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
	check_fd_output(wrapper_putchar_str, "AB", "AB");
}

static void	ft_putchar_0_test(void) {
	check_fd_output(wrapper_putchar, &(char){'0'}, "0");
}

static void	test_ft_putchar_fd(void) {
	const t_test	tests[] = {
		{"'A' and 'B'", ft_putchar_AB_test},
		{"'0'", ft_putchar_0_test}
	};

	run_tests("ft_putchar_fd", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                        ft_putstr_fd tests                                  */
/* ************************************************************************** */

static void	wrapper_putstr(void *s, int fd) {
	ft_putstr_fd((char *)s, fd);
}

static void	ft_putstr_hello_test(void) {
	check_fd_output(wrapper_putstr, "Hello, World!", "Hello, World!");
}

static void	ft_putstr_null_test(void) {
	ft_putstr_fd(NULL, open("/dev/null", O_WRONLY));
}

static void	test_ft_putstr_fd(void) {
	const t_test	tests[] = {
		{"'Hello, World!'", ft_putstr_hello_test},
		{"NULL", ft_putstr_null_test}
	};

	run_tests("ft_putstr_fd", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                        ft_putendl_fd tests                                 */
/* ************************************************************************** */

static void	wrapper_putendl(void *s, int fd) {
	ft_putendl_fd((char *)s, fd);
}

static void	ft_putendl_hello_test(void) {
	check_fd_output(wrapper_putendl, "Hello", "Hello\n");
}

static void	ft_putendl_empty_test(void) {
	check_fd_output(wrapper_putendl, "", "\n");
}

static void	ft_putendl_null_test(void) {
	ft_putendl_fd(NULL, open("/dev/null", O_WRONLY));
}

static void	test_ft_putendl_fd(void) {
	const t_test	tests[] = {
		{"'Hello' with newline", ft_putendl_hello_test},
		{"empty string with newline", ft_putendl_empty_test},
		{"NULL", ft_putendl_null_test}
	};

	run_tests("ft_putendl_fd", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                        ft_putnbr_fd tests                                  */
/* ************************************************************************** */

static void	wrapper_putnbr(void *n, int fd) {
	ft_putnbr_fd(*(int *)n, fd);
}

static void	ft_putnbr_42_test(void) {
	check_fd_output(wrapper_putnbr, &(int){42}, "42");
}

static void	ft_putnbr_minus_42_test(void) {
	check_fd_output(wrapper_putnbr, &(int){-42}, "-42");
}

static void	ft_putnbr_zero_test(void) {
	check_fd_output(wrapper_putnbr, &(int){0}, "0");
}

static void	ft_putnbr_int_min_test(void) {
	check_fd_output(wrapper_putnbr, &(int){-2147483648}, "-2147483648");
}

static void	ft_putnbr_int_max_test(void) {
	check_fd_output(wrapper_putnbr, &(int){2147483647}, "2147483647");
}

static void	test_ft_putnbr_fd(void) {
	const t_test	tests[] = {
		{"42", ft_putnbr_42_test},
		{"-42", ft_putnbr_minus_42_test},
		{"0", ft_putnbr_zero_test},
		{"INT_MIN", ft_putnbr_int_min_test},
		{"INT_MAX", ft_putnbr_int_max_test}
	};

	run_tests("ft_putnbr_fd", tests, ARRAY_SIZE(tests));
}

/* ************************************************************************** */
/*                                 MAIN                                       */
/* ************************************************************************** */

int	main(void) {
	test_char_func("ft_isalpha", ft_isalpha, isalpha, 1);
	test_char_func("ft_isdigit", ft_isdigit, isdigit, 1);
	test_char_func("ft_isalnum", ft_isalnum, isalnum, 1);
	test_char_func("ft_isascii", ft_isascii, isascii, 1);
	test_char_func("ft_isprint", ft_isprint, isprint, 1);
	test_ft_strlen();
	test_ft_memset();
	test_ft_bzero();
	test_ft_memcpy();
	test_ft_memmove();
	test_ft_strlcpy();
	test_ft_strlcat();
	test_char_func("ft_toupper", ft_toupper, toupper, 0);
	test_char_func("ft_tolower", ft_tolower, tolower, 0);
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
