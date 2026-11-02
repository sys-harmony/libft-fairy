#include "libft_fairy.h"

/*
 * Part 1 and 2. Wherever the libc has the function, ft_ is compared with it
 * over a sweep of inputs; strlcpy, strlcat and strnstr, missing from glibc,
 * with a reference written after the BSD man pages. Buffers are compared
 * whole, so a byte written past the end is caught.
 */

/* ************************************************************************** */
/*                     character classes, toupper, tolower                    */
/* ************************************************************************** */

/* Every value from EOF to 255; a class function must return 1 (subject). */
static void	char_test(const char *name, int (*ft)(int), int (*ref)(int), int is_class) {
	for (int c = EOF; c <= 255; c++) {
		int	expected = is_class ? !!ref(c) : ref(c);

		EXPECT(ft(c) == expected, "%s(%d): got %d, expected %d", name, c, ft(c), expected);
	}
}

#define CHAR_TEST(ft, ref, is_class) \
	static void	ft##_test(void) { char_test(#ft, ft, ref, is_class); }

CHAR_TEST(ft_isalpha, isalpha, 1)
CHAR_TEST(ft_isdigit, isdigit, 1)
CHAR_TEST(ft_isalnum, isalnum, 1)
CHAR_TEST(ft_isascii, isascii, 1)
CHAR_TEST(ft_isprint, isprint, 1)
CHAR_TEST(ft_toupper, toupper, 0)
CHAR_TEST(ft_tolower, tolower, 0)

static const t_test	g_char_tests[] = {
	{"ft_isalpha, EOF to 255", ft_isalpha_test},
	{"ft_isdigit, EOF to 255", ft_isdigit_test},
	{"ft_isalnum, EOF to 255", ft_isalnum_test},
	{"ft_isascii, EOF to 255", ft_isascii_test},
	{"ft_isprint, EOF to 255", ft_isprint_test},
	{"ft_toupper, EOF to 255", ft_toupper_test},
	{"ft_tolower, EOF to 255", ft_tolower_test},
};

/* ************************************************************************** */
/*                                   memory                                   */
/* ************************************************************************** */

#define BUF 64

/* Fills a buffer with a pattern that no tested operation writes. */
static void	pattern(unsigned char *b) {
	for (int i = 0; i < BUF; i++)
		b[i] = 0x80 | (i * 7);
}

static void	memset_test(void) {
	const int		values[] = {0, 'A', 200, -1, 'A' + 256};
	unsigned char	a[BUF];
	unsigned char	b[BUF];
	char			call[64];

	for (size_t v = 0; v < ARRAY_SIZE(values); v++)
		for (int off = 0; off < 8; off++)
			for (size_t n = 0; n <= 40; n++) {
				pattern(a);
				pattern(b);
				memset(b + off, values[v], n);
				EXPECT(ft_memset(a + off, values[v], n) == a + off, "ft_memset: wrong return value");
				snprintf(call, sizeof(call), "ft_memset(buf + %d, %d, %zu)", off, values[v], n);
				expect_same(call, a, b, BUF);
			}
}

static void	bzero_test(void) {
	unsigned char	a[BUF];
	unsigned char	b[BUF];
	char			call[64];

	for (int off = 0; off < 8; off++)
		for (size_t n = 0; n <= 40; n++) {
			pattern(a);
			pattern(b);
			memset(b + off, 0, n);
			ft_bzero(a + off, n);
			snprintf(call, sizeof(call), "ft_bzero(buf + %d, %zu)", off, n);
			expect_same(call, a, b, BUF);
		}
}

static void	memcpy_test(void) {
	unsigned char	src[BUF];
	unsigned char	a[BUF];
	unsigned char	b[BUF];
	char			call[64];

	for (int i = 0; i < BUF; i++)
		src[i] = i * 13;
	for (int off = 0; off < 8; off++)
		for (size_t n = 0; n <= 40; n++) {
			pattern(a);
			pattern(b);
			memcpy(b + off, src + 7 - off, n);
			EXPECT(ft_memcpy(a + off, src + 7 - off, n) == a + off, "ft_memcpy: wrong return value");
			snprintf(call, sizeof(call), "ft_memcpy(buf + %d, src + %d, %zu)", off, 7 - off, n);
			expect_same(call, a, b, BUF);
		}
}

/* Every overlap: source and destination 0 to 15 bytes apart, both ways. */
static void	memmove_test(void) {
	unsigned char	a[BUF];
	unsigned char	b[BUF];
	char			call[64];

	for (int src = 0; src < 16; src++)
		for (int dst = 0; dst < 16; dst++)
			for (size_t n = 0; n <= 32; n++) {
				pattern(a);
				pattern(b);
				memmove(b + dst, b + src, n);
				EXPECT(ft_memmove(a + dst, a + src, n) == a + dst, "ft_memmove: wrong return value");
				snprintf(call, sizeof(call), "ft_memmove(buf + %d, buf + %d, %zu)", dst, src, n);
				expect_same(call, a, b, BUF);
			}
}

static long	offset(const void *p, const void *base) {
	return p ? (const char *)p - (const char *)base : -1;
}

static void	memchr_test(void) {
	unsigned char	s[256];

	for (int i = 0; i < 256; i++)
		s[i] = 255 - i;
	for (int c = -256; c < 512; c++)
		for (size_t n = 0; n <= 256; n += 15)
			EXPECT(ft_memchr(s, c, n) == memchr(s, c, n), "ft_memchr(s, %d, %zu): got s + %ld, expected s + %ld"
				" (s - 1: NULL)", c, n, offset(ft_memchr(s, c, n), s), offset(memchr(s, c, n), s));
}

static void	memcmp_test(void) {
	const char	s[][6] = {"", "a", "abc", "abd", "ab\0c", "ab\0d", "\200", "\0", "abc\200", "abc\001"};

	for (size_t i = 0; i < ARRAY_SIZE(s); i++)
		for (size_t j = 0; j < ARRAY_SIZE(s); j++)
			for (size_t n = 0; n <= sizeof(*s); n++)
				EXPECT(SIGN(ft_memcmp(s[i], s[j], n)) == SIGN(memcmp(s[i], s[j], n)), "ft_memcmp(%s, %s, %zu): "
					"got %d, expected a %s value", show(s[i]), show(s[j]), n, ft_memcmp(s[i], s[j], n),
					memcmp(s[i], s[j], n) ? memcmp(s[i], s[j], n) > 0 ? "positive" : "negative" : "zero");
}

static void	memset_null_test(void) {
	ft_memset(NULL, 'a', 5);
}

static void	memcpy_null_test(void) {
	char	buf[8];

	ft_memcpy(buf, NULL, 5);
}

static void	memmove_null_test(void) {
	char	buf[8];

	ft_memmove(buf, NULL, 5);
}

static void	null_n_zero_test(void) {
	ft_memcpy(NULL, NULL, 0);
	ft_memmove(NULL, NULL, 0);
}

static const t_test	g_memory_tests[] = {
	{"ft_memset: 5 values, 8 offsets, 0 to 40 bytes", memset_test},
	{"ft_bzero: 8 offsets, 0 to 40 bytes", bzero_test},
	{"ft_memcpy: 8 offsets, 0 to 40 bytes", memcpy_test},
	{"ft_memmove: every overlap, both ways", memmove_test},
	{"ft_memchr: every c from -256 to 511", memchr_test},
	{"ft_memcmp: unsigned bytes, past '\\0'", memcmp_test},
	{"ft_memset(NULL, ...)", memset_null_test, UB_CRASH},
	{"ft_memcpy(buf, NULL, 5)", memcpy_null_test, UB_CRASH},
	{"ft_memmove(buf, NULL, 5)", memmove_null_test, UB_CRASH},
	{"ft_memcpy, ft_memmove (NULL, NULL, 0)", null_n_zero_test, UB_SAFE},
};

/* ************************************************************************** */
/*                                  strings                                   */
/* ************************************************************************** */

static const char	*g_strs[] = {"", "a", "hello", "Hello\xC8World\xC8!", "abcabcabd",
	" \t\n spaces \v\f\r", "\x7f\x80\xff"};

static size_t	ref_strlcpy(char *dst, const char *src, size_t size) {
	size_t	len = strlen(src);

	if (size) {
		size_t	n = len < size - 1 ? len : size - 1;

		memcpy(dst, src, n);
		dst[n] = '\0';
	}
	return len;
}

static size_t	ref_strlcat(char *dst, const char *src, size_t size) {
	size_t	dlen = strnlen(dst, size);

	if (dlen == size)
		return size + strlen(src);
	return dlen + ref_strlcpy(dst + dlen, src, size - dlen);
}

static char	*ref_strnstr(const char *big, const char *little, size_t len) {
	size_t	n = strlen(little);

	for (size_t i = 0; big[i] && n && i + n <= len; i++)
		if (!strncmp(big + i, little, n))
			return (char *)big + i;
	return n ? NULL : (char *)big;
}

static void	strlen_test(void) {
	char	big[10000];

	memset(big, 'z', sizeof(big) - 1);
	big[sizeof(big) - 1] = '\0';
	for (size_t i = 0; i < ARRAY_SIZE(g_strs); i++)
		EXPECT(ft_strlen(g_strs[i]) == strlen(g_strs[i]), "ft_strlen(%s): got %zu, expected %zu",
			show(g_strs[i]), ft_strlen(g_strs[i]), strlen(g_strs[i]));
	EXPECT(ft_strlen(big) == sizeof(big) - 1, "ft_strlen of 9999 chars: got %zu", ft_strlen(big));
}

/* Every size from 0 to past the end, the whole buffer compared. */
static void	strlcpy_test(void) {
	char	a[BUF];
	char	b[BUF];
	size_t	got;
	size_t	expected;

	for (size_t i = 0; i < ARRAY_SIZE(g_strs); i++)
		for (size_t size = 0; size <= strlen(g_strs[i]) + 2; size++) {
			pattern((unsigned char *)a);
			pattern((unsigned char *)b);
			got = ft_strlcpy(a, g_strs[i], size);
			expected = ref_strlcpy(b, g_strs[i], size);
			EXPECT(got == expected, "ft_strlcpy(dst, %s, %zu): returned %zu, expected %zu",
				show(g_strs[i]), size, got, expected);
			EXPECT(!memcmp(a, b, BUF), "ft_strlcpy(dst, %s, %zu): dst is %s, expected %s",
				show(g_strs[i]), size, size ? show(a) : "changed", size ? show(b) : "untouched");
		}
}

/* dst holding 0 to 5 chars, every size, including those shorter than dst. */
static void	strlcat_test(void) {
	const char	*dsts[] = {"", "a", "abc", "hello"};
	char		a[BUF];
	char		b[BUF];
	size_t		got;
	size_t		expected;

	for (size_t d = 0; d < ARRAY_SIZE(dsts); d++)
		for (size_t i = 0; i < ARRAY_SIZE(g_strs); i++)
			for (size_t size = 0; size <= strlen(dsts[d]) + strlen(g_strs[i]) + 2; size++) {
				pattern((unsigned char *)a);
				pattern((unsigned char *)b);
				strcpy(a, dsts[d]);
				strcpy(b, dsts[d]);
				got = ft_strlcat(a, g_strs[i], size);
				expected = ref_strlcat(b, g_strs[i], size);
				EXPECT(got == expected, "ft_strlcat(%s, %s, %zu): returned %zu, expected %zu",
					show(dsts[d]), show(g_strs[i]), size, got, expected);
				EXPECT(!memcmp(a, b, BUF), "ft_strlcat(%s, %s, %zu): dst is %s, expected %s",
					show(dsts[d]), show(g_strs[i]), size, show(a), show(b));
			}
}

/* Every c from -300 to 600: the search is for (char)c, '\0' included. */
static void	strchr_test(void) {
	for (size_t i = 0; i < ARRAY_SIZE(g_strs); i++)
		for (int c = -300; c <= 600; c++) {
			EXPECT(ft_strchr(g_strs[i], c) == strchr(g_strs[i], c), "ft_strchr(%s, %d): got %s, expected %s",
				show(g_strs[i]), c, show(ft_strchr(g_strs[i], c)), show(strchr(g_strs[i], c)));
			EXPECT(ft_strrchr(g_strs[i], c) == strrchr(g_strs[i], c), "ft_strrchr(%s, %d): got %s, expected %s",
				show(g_strs[i]), c, show(ft_strrchr(g_strs[i], c)), show(strrchr(g_strs[i], c)));
		}
}

/* Each pair, n from 0 to past both ends and SIZE_MAX: the sign must match. */
static void	strncmp_test(void) {
	const char		*s[] = {"", "a", "ab", "abc", "abd", "abc\200", "\200", "ab\0c", "abc\0d"};
	const size_t	ns[] = {0, 1, 2, 3, 4, 6, SIZE_MAX};

	for (size_t i = 0; i < ARRAY_SIZE(s); i++)
		for (size_t j = 0; j < ARRAY_SIZE(s); j++)
			for (size_t n = 0; n < ARRAY_SIZE(ns); n++)
				EXPECT(SIGN(ft_strncmp(s[i], s[j], ns[n])) == SIGN(strncmp(s[i], s[j], ns[n])),
					"ft_strncmp(%s, %s, %zu): got %d, expected a %s value", show(s[i]), show(s[j]),
					ns[n], ft_strncmp(s[i], s[j], ns[n]), strncmp(s[i], s[j], ns[n])
					? strncmp(s[i], s[j], ns[n]) > 0 ? "positive" : "negative" : "zero");
}

/* Each haystack and needle, len from 0 to past the end and SIZE_MAX. */
static void	strnstr_test(void) {
	const char	*needles[] = {"", "a", "b", "aab", "abd", "abc", "World", "Hello\xC8World\xC8!!", "xyz"};

	for (size_t h = 0; h < ARRAY_SIZE(g_strs); h++)
		for (size_t n = 0; n < ARRAY_SIZE(needles); n++)
			for (size_t k = 0; k <= strlen(g_strs[h]) + 3; k++) {
				size_t	len = k > strlen(g_strs[h]) + 2 ? SIZE_MAX : k;

				EXPECT(ft_strnstr(g_strs[h], needles[n], len) == ref_strnstr(g_strs[h], needles[n], len),
					"ft_strnstr(%s, %s, %zu): got %s, expected %s", show(g_strs[h]), show(needles[n]), len,
					show(ft_strnstr(g_strs[h], needles[n], len)), show(ref_strnstr(g_strs[h], needles[n], len)));
			}
}

static void	atoi_test(void) {
	const char	*s[] = {"0", "42", "-42", "+42", "  \t\n\v\f\r 42", "\v \t\r  \n    \f-42   *", "0042",
		"42abc", "4 2", "", "+", "-", "++2", "--4", "-+9", "+-8", " \t-R66", "\x01 42", "2147483647",
		"-2147483648", "-0", "+0", "  +000123456789xyz", "1e3", "\200 5"};

	for (size_t i = 0; i < ARRAY_SIZE(s); i++)
		EXPECT(ft_atoi(s[i]) == atoi(s[i]), "ft_atoi(%s): got %d, expected %d", show(s[i]), ft_atoi(s[i]), atoi(s[i]));
}

static void	strlen_null_test(void) {
	ft_strlen(NULL);
}

static void	strchr_null_test(void) {
	ft_strchr(NULL, 'a');
}

static void	strrchr_null_test(void) {
	ft_strrchr(NULL, 'a');
}

static void	strncmp_null_test(void) {
	ft_strncmp(NULL, "abc", 3);
}

static void	atoi_null_test(void) {
	ft_atoi(NULL);
}

static const t_test	g_string_tests[] = {
	{"ft_strlen", strlen_test},
	{"ft_strlcpy: every size, whole buffer", strlcpy_test},
	{"ft_strlcat: every size, dst longer than size", strlcat_test},
	{"ft_strchr, ft_strrchr: every c from -300 to 600", strchr_test},
	{"ft_strncmp: unsigned chars, past '\\0', SIZE_MAX", strncmp_test},
	{"ft_strnstr: every len, SIZE_MAX", strnstr_test},
	{"ft_atoi: spaces, signs, junk, INT_MIN, INT_MAX", atoi_test},
	{"ft_strlen(NULL)", strlen_null_test, UB_CRASH},
	{"ft_strchr(NULL, 'a')", strchr_null_test, UB_CRASH},
	{"ft_strrchr(NULL, 'a')", strrchr_null_test, UB_CRASH},
	{"ft_strncmp(NULL, \"abc\", 3)", strncmp_null_test, UB_CRASH},
	{"ft_atoi(NULL)", atoi_null_test, UB_CRASH},
};

/* ************************************************************************** */
/*                                allocations                                 */
/* ************************************************************************** */

static void	calloc_test(void) {
	const size_t	sizes[][2] = {{1, 1}, {5, sizeof(int)}, {3, 17}, {1000, 1}};

	for (size_t i = 0; i < ARRAY_SIZE(sizes); i++) {
		unsigned char	*p = ft_calloc(sizes[i][0], sizes[i][1]);
		size_t			n = sizes[i][0] * sizes[i][1];

		EXPECT(p && alloc_size(p) >= n, "ft_calloc(%zu, %zu): %zu bytes malloc'd", sizes[i][0], sizes[i][1],
			p ? alloc_size(p) : 0);
		for (size_t j = 0; j < n; j++)
			EXPECT(!p[j], "ft_calloc(%zu, %zu): byte %zu is not 0", sizes[i][0], sizes[i][1], j);
	}
}

/* Subject: with nmemb or size 0, a unique pointer that free() accepts. */
static void	calloc_zero_test(void) {
	void	*a = ft_calloc(0, 5);
	void	*b = ft_calloc(5, 0);

	EXPECT(a && b && a != b, "ft_calloc(0, 5), ft_calloc(5, 0): got %p and %p, the subject wants unique "
		"pointers that free() accepts", a, b);
	free(a);
	free(b);
}

static void	calloc_overflow_test(void) {
	EXPECT(!ft_calloc(SIZE_MAX / 2, 3), "ft_calloc(SIZE_MAX / 2, 3): nmemb * size overflows, expected NULL");
	EXPECT(!ft_calloc(SIZE_MAX, SIZE_MAX), "ft_calloc(SIZE_MAX, SIZE_MAX): expected NULL");
}

static void	strdup_test(void) {
	char	call[128];

	for (size_t i = 0; i < ARRAY_SIZE(g_strs); i++) {
		snprintf(call, sizeof(call), "ft_strdup(%s)", show(g_strs[i]));
		check_str(call, ft_strdup(g_strs[i]), g_strs[i]);
	}
}

static void	malloc_fail_test(void) {
	EACH_MALLOC_FAILING(ft_calloc(4, 4));
	EACH_MALLOC_FAILING(ft_strdup("hello"));
}

static const t_test	g_alloc_tests[] = {
	{"ft_calloc: zeroed, big enough", calloc_test},
	{"ft_calloc(0, 5), ft_calloc(5, 0)", calloc_zero_test},
	{"ft_calloc: nmemb * size overflowing", calloc_overflow_test},
	{"ft_strdup", strdup_test},
	{"ft_calloc, ft_strdup: NULL when malloc fails", malloc_fail_test},
};

/* ************************************************************************** */
/*                          Part 2: new strings                               */
/* ************************************************************************** */

static void	substr_test(void) {
	const struct {
		const char		*s;
		unsigned int	start;
		size_t			len;
		const char		*expected;
	}	cases[] = {
		{"Hello, World!", 7, 5, "World"}, {"Hello", 0, 3, "Hel"}, {"Hello", 2, 100, "llo"},
		{"Hello", 0, 0, ""}, {"Hello", 5, 1, ""}, {"Hello", 10, 5, ""}, {"", 0, 5, ""},
		{"", 1, 1, ""}, {"Hello", 4, 1, "o"}, {"Hello", UINT_MAX, 3, ""}, {"Hello", 1, SIZE_MAX, "ello"},
	};
	char	call[96];

	for (size_t i = 0; i < ARRAY_SIZE(cases); i++) {
		snprintf(call, sizeof(call), "ft_substr(%s, %u, %zu)", show(cases[i].s), cases[i].start, cases[i].len);
		check_str(call, ft_substr(cases[i].s, cases[i].start, cases[i].len), cases[i].expected);
	}
}

static void	strjoin_test(void) {
	const char	*cases[][3] = {{"Hello", " World", "Hello World"}, {"", "abc", "abc"},
		{"abc", "", "abc"}, {"", "", ""}, {"\xff", "\x01", "\xff\x01"}};
	char		call[96];

	for (size_t i = 0; i < ARRAY_SIZE(cases); i++) {
		snprintf(call, sizeof(call), "ft_strjoin(%s, %s)", show(cases[i][0]), show(cases[i][1]));
		check_str(call, ft_strjoin(cases[i][0], cases[i][1]), cases[i][2]);
	}
}

static void	strtrim_test(void) {
	const char	*cases[][3] = {{"   Hello   ", " ", "Hello"}, {"xxHelloxWorldxx", "x", "HelloxWorld"},
		{"abcdcba", "abc", "d"}, {"Hello", "xyz", "Hello"}, {"Hello", "", "Hello"}, {"", " ", ""},
		{"", "", ""}, {"xxxxx", "x", ""}, {" \t\n a b \n\t ", " \t\n", "a b"}, {"aXb", "ab", "X"}};
	char		call[96];

	for (size_t i = 0; i < ARRAY_SIZE(cases); i++) {
		snprintf(call, sizeof(call), "ft_strtrim(%s, %s)", show(cases[i][0]), show(cases[i][1]));
		check_str(call, ft_strtrim(cases[i][0], cases[i][1]), cases[i][2]);
	}
}

/* Each word a malloc'd string, the array malloc'd with room for its NULL. */
static void	split_case(const char *s, char c, const char *const *expected) {
	char	**got = ft_split(s, c);
	char	call[96];
	size_t	n = 0;

	snprintf(call, sizeof(call), "ft_split(%s, %s)", show(s), show((char []){c, 0}));
	EXPECT(got, "%s: got NULL", call);
	while (expected[n]) {
		EXPECT(got[n], "%s: %zu words, expected more", call, n);
		check_str(call, got[n], expected[n]);
		n++;
	}
	EXPECT(!got[n], "%s: word %zu is %s, expected NULL", call, n, show(got[n]));
	EXPECT(alloc_size(got) >= (n + 1) * sizeof(char *), "%s: %zu words + NULL in a %zu-byte array",
		call, n, alloc_size(got));
}

static void	split_test(void) {
	split_case("Hello World 42", ' ', (const char *[]){"Hello", "World", "42", NULL});
	split_case("___Hello___World___", '_', (const char *[]){"Hello", "World", NULL});
	split_case("", ' ', (const char *[]){NULL});
	split_case("xxx", 'x', (const char *[]){NULL});
	split_case("Hello", '\0', (const char *[]){"Hello", NULL});
	split_case("HelloWorld", ' ', (const char *[]){"HelloWorld", NULL});
	split_case("a", ' ', (const char *[]){"a", NULL});
	split_case("  a  b  ", ' ', (const char *[]){"a", "b", NULL});
	split_case("Hello\xffWorld", '\xff', (const char *[]){"Hello", "World", NULL});
	split_case("1,2,,3,", ',', (const char *[]){"1", "2", "3", NULL});
}

/* Each value next to a power of 10, compared with printf's %d. */
static void	itoa_test(void) {
	char	expected[16];
	char	call[32];

	for (long p = 1; p <= 1000000000; p *= 10)
		for (long v = p - 1; v <= p + 1; v++)
			for (int sign = -1; sign <= 1; sign += 2) {
				snprintf(expected, sizeof(expected), "%ld", sign * v);
				snprintf(call, sizeof(call), "ft_itoa(%ld)", sign * v);
				check_str(call, ft_itoa(sign * v), expected);
			}
	check_str("ft_itoa(INT_MAX)", ft_itoa(INT_MAX), "2147483647");
	check_str("ft_itoa(INT_MIN)", ft_itoa(INT_MIN), "-2147483648");
}

static char	map_index(unsigned int i, char c) {
	return c + i;
}

static void	strmapi_test(void) {
	check_str("ft_strmapi(\"abc\", c + i)", ft_strmapi("abc", map_index), "ace");
	check_str("ft_strmapi(\"\", c + i)", ft_strmapi("", map_index), "");
	check_str("ft_strmapi(\"aaaaaaaaaa\", c + i)", ft_strmapi("aaaaaaaaaa", map_index), "abcdefghij");
}

static char	*g_iteri_base;

static void	iteri_check(unsigned int i, char *c) {
	EXPECT(c == g_iteri_base + i, "ft_striteri: f got index %u with the address of char %td", i, c - g_iteri_base);
	*c += i;
}

static void	striteri_test(void) {
	char	s[] = "aaaaaaaaaa";

	g_iteri_base = s;
	ft_striteri(s, iteri_check);
	EXPECT(!strcmp(s, "abcdefghij"), "ft_striteri(\"aaaaaaaaaa\", c += i): got %s", show(s));
	ft_striteri(s + 10, iteri_check);
}

static void	malloc_fail2_test(void) {
	EACH_MALLOC_FAILING(ft_substr("Hello", 1, 3));
	EACH_MALLOC_FAILING(ft_strjoin("Hello", " World"));
	EACH_MALLOC_FAILING(ft_strtrim("  Hello  ", " "));
	EACH_MALLOC_FAILING(ft_split("  Hello World  42 ", ' '));
	EACH_MALLOC_FAILING(ft_itoa(-42));
	EACH_MALLOC_FAILING(ft_strmapi("abc", map_index));
}

static void	substr_null_test(void) {
	ft_substr(NULL, 0, 5);
}

static void	strjoin_null_test(void) {
	ft_strjoin(NULL, "a");
	ft_strjoin("a", NULL);
}

static void	strtrim_null_test(void) {
	ft_strtrim(NULL, " ");
	ft_strtrim("a", NULL);
}

static void	split_null_test(void) {
	ft_split(NULL, ' ');
}

static void	strmapi_null_test(void) {
	ft_strmapi(NULL, map_index);
	ft_strmapi("a", NULL);
}

static void	striteri_null_test(void) {
	ft_striteri(NULL, iteri_check);
	ft_striteri((char []){"a"}, NULL);
}

static const t_test	g_new_string_tests[] = {
	{"ft_substr: start past the end, UINT_MAX, SIZE_MAX", substr_test},
	{"ft_strjoin", strjoin_test},
	{"ft_strtrim: inner chars kept, all trimmed", strtrim_test},
	{"ft_split: delimiters in a row, '\\0', \\xff", split_test},
	{"ft_itoa: around powers of 10, INT_MIN, INT_MAX", itoa_test},
	{"ft_strmapi", strmapi_test},
	{"ft_striteri: index and address of each char", striteri_test},
	{"each malloc failing in turn: NULL, no crash", malloc_fail2_test},
	{"ft_substr(NULL, ...)", substr_null_test, UB_SAFE},
	{"ft_strjoin with NULL", strjoin_null_test, UB_SAFE},
	{"ft_strtrim with NULL", strtrim_null_test, UB_SAFE},
	{"ft_split(NULL, ' ')", split_null_test, UB_SAFE},
	{"ft_strmapi with NULL", strmapi_null_test, UB_SAFE},
	{"ft_striteri with NULL", striteri_null_test, UB_SAFE},
};

/* ************************************************************************** */
/*                           Part 2: file descriptors                         */
/* ************************************************************************** */

/* Opens a pipe: the function under test writes to p[1]. */
static void	open_pipe(int p[2]) {
	if (pipe(p) < 0)
		die("libft-fairy: pipe");
}

/* Reads back the pipe: exactly len bytes of expected, else fails. */
static void	expect_written(int p[2], const char *expected, size_t len, const char *call) {
	static char	got[16384];
	ssize_t		n;

	close(p[1]);
	n = read(p[0], got, sizeof(got) - 1);
	close(p[0]);
	got[n > 0 ? n : 0] = '\0';
	EXPECT(n == (ssize_t)len && !memcmp(got, expected, len), "%s: wrote %s (%zd bytes), expected %s (%zu)",
		call, show(got), n, show(expected), len);
}

static void	putchar_fd_test(void) {
	int		p[2];
	char	call[32];

	for (int c = 0; c < 256; c++) {
		open_pipe(p);
		ft_putchar_fd(c, p[1]);
		snprintf(call, sizeof(call), "ft_putchar_fd(%d, fd)", c);
		expect_written(p, (char []){c, 0}, 1, call);
	}
}

static void	putstr_fd_test(void) {
	static char	big[10000];
	int			p[2];

	memset(big, 'z', sizeof(big) - 1);
	open_pipe(p);
	ft_putstr_fd("Hello, World!", p[1]);
	expect_written(p, "Hello, World!", 13, "ft_putstr_fd(\"Hello, World!\", fd)");
	open_pipe(p);
	ft_putstr_fd("", p[1]);
	expect_written(p, "", 0, "ft_putstr_fd(\"\", fd)");
	open_pipe(p);
	ft_putstr_fd(big, p[1]);
	expect_written(p, big, sizeof(big) - 1, "ft_putstr_fd(9999 chars, fd)");
}

static void	putendl_fd_test(void) {
	int	p[2];

	open_pipe(p);
	ft_putendl_fd("Hello", p[1]);
	expect_written(p, "Hello\n", 6, "ft_putendl_fd(\"Hello\", fd)");
	open_pipe(p);
	ft_putendl_fd("", p[1]);
	expect_written(p, "\n", 1, "ft_putendl_fd(\"\", fd)");
}

static void	putnbr_fd_test(void) {
	const int	vals[] = {0, 1, -1, 9, -9, 10, -10, 42, -42, 100, 12345, -12345, INT_MAX, INT_MIN};
	char		expected[16];
	char		call[32];
	int			p[2];

	for (size_t i = 0; i < ARRAY_SIZE(vals); i++) {
		snprintf(expected, sizeof(expected), "%d", vals[i]);
		snprintf(call, sizeof(call), "ft_putnbr_fd(%d, fd)", vals[i]);
		open_pipe(p);
		ft_putnbr_fd(vals[i], p[1]);
		expect_written(p, expected, strlen(expected), call);
	}
}

static void	put_null_test(void) {
	ft_putstr_fd(NULL, 1);
	ft_putendl_fd(NULL, 1);
}

static const t_test	g_fd_tests[] = {
	{"ft_putchar_fd: every byte from 0 to 255", putchar_fd_test},
	{"ft_putstr_fd: empty, 9999 chars", putstr_fd_test},
	{"ft_putendl_fd", putendl_fd_test},
	{"ft_putnbr_fd: 0, ±1, INT_MIN, INT_MAX", putnbr_fd_test},
	{"ft_putstr_fd, ft_putendl_fd with NULL", put_null_test, UB_SAFE},
};

int	main(void) {
	RUN_GROUP("characters", g_char_tests);
	RUN_GROUP("memory", g_memory_tests);
	RUN_GROUP("strings", g_string_tests);
	RUN_GROUP("allocations", g_alloc_tests);
	RUN_GROUP("new strings", g_new_string_tests);
	RUN_GROUP("file descriptors", g_fd_tests);
	return fairy_summary();
}
