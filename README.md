# 🧚 libft-fairy

A tester for 42's **libft** (subject v19.3, where the linked lists are mandatory). Every function swept against the libc (or a BSD reference where glibc has none), allocation sizes checked, each `malloc` failing in turn, Valgrind, plus every check the subject asks for, in about 2 seconds.

Part of a trio with [printf-fairy](https://github.com/sys-harmony/printf-fairy) and [gnl-fairy](https://github.com/sys-harmony/gnl-fairy): same display, same verdicts, same engine.

## 🚀 Usage

Clone it inside your libft repository, then run it:

```bash
git clone https://github.com/sys-harmony/libft-fairy.git
./libft-fairy/run.sh          # failing tests only
./libft-fairy/run.sh -v       # every test
```

The exit status is `0` when everything passed (warnings included), `1` otherwise, so it fits in a script or a CI.

## 📊 Output

```
╔════════════════════════════════════════════╗
║               LIBFT-FAIRY 🧚               ║
╚════════════════════════════════════════════╝

📂 Files ................................ Done
📖 README ............................... Done
📝 Norm ................................. Done
📋 Prototypes ........................... Done
🔧 Makefile ............................. Done
📦 Build ................................ Done
🔍 Symbols .............................. Done
🔨 Test build ........................... Done
🧪 Tests .............................. Failed

── memory ────────────────────────────────────
[KO] ft_memmove: every overlap, both ways
     ↳ ft_memmove(buf + 1, buf + 0, 2): byte 2 is 0x80, expected 0x87

── strings ───────────────────────────────────
[KO] ft_strncmp: unsigned chars, past '\0', SIZE_MAX
     ↳ ft_strncmp("", "\x80", 1): got 128, expected a negative value
[OK] ft_strlen(NULL)
     ⚠ does not crash where the libc does: undefined behaviour, either is fine

── new strings ───────────────────────────────
[KO] each malloc failing in turn: NULL, no crash
     ↳ ft_split("  Hello World  42 ", ' ') returned 0x29e7c4f0 when malloc #2 failed, expected NULL

── valgrind · leak_tests ─────────────────────
     Invalid free() / delete / delete[] / realloc()
        at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
        by 0x401CEF: free_split (leak_tests.c:31)
      Address 0x4a9f370 is 0 bytes inside a block of size 6 free'd
        at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
        by 0x404086: ft_free (ft_split.c:22)
        by 0x40400D: ft_fill_strs (ft_split.c:61)
     …
🧹 Cleanup .............................. Done

╔════════════════════════════════════════════╗
║            OH NO... YOU FAILED!            ║
╚════════════════════════════════════════════╝
   ✘ Tests · 3/69 tests failed · 1.90s
```

Only what did not pass is listed, each time with its reason: the call, what it gave and what was expected, a crash signal, a timeout, or the Valgrind report, which points to your file and line.

## ⚖️ Failures and warnings

A **failure** is a bug or a clear subject violation. A **warning** is something debatable: worth a look, maybe a question from your evaluator, never a failure. A working libft passes.

`NULL` where a string or a list is expected is undefined behaviour, so these tests never fail. They warn when the function does not do what most expect:

- Part 1, where the libc crashes (`ft_strlen(NULL)`, `ft_memcpy(buf, NULL, 5)`...): a warning if yours does not crash
- Part 2 and 3 (`ft_strjoin(NULL, "a")`, `ft_lstclear(NULL, del)`, `f` or `del` NULL...): a warning if yours crashes

An empty list, though, is a valid list: `ft_lstsize(NULL)`, `ft_lstiter(NULL, f)` or `ft_lstmap(NULL, f, del)` must work.

Other warnings:

- `ft_lstmap` leaking the content `f` made when a node's `malloc` fails (most evaluations skip it)
- a public `ft_` function that is not in the subject: `static`, if it is a helper
- a README that does not look like the subject's description, project files git does not track
- `norminette` or `valgrind` missing: that check did not run

## 🧪 What is checked

| Step | What |
|---|---|
| 📂 Files | `Makefile`, `libft.h` and `ft_*.c` at the root, nothing else among the graded files (a local, untracked `main.c` is fine); every `.c` compiled by `make` (unused files are forbidden); project files not committed (warning) |
| 📖 README | there (failure), with the italic first line, *Description*, *Instructions*, *Resources* and how AI was used (warnings) |
| 📝 Norm | `norminette` on the graded files |
| 📋 Prototypes | the compiler checks `libft.h` against the 43 prototypes of the subject; no `restrict` |
| 🔧 Makefile | `$(NAME)`, `all`, `clean`, `fclean`, `re`; `cc -Wall -Wextra -Werror` on every compilation `make -n` shows; no `-std=c99`; `ar`, no `libtool`; no wildcard |
| 📦 Build | a build from scratch, then `make` again must not relink |
| 🔍 Symbols | in every object of `libft.a`: the external functions each function may call (`malloc`, `free`, `write`, as the subject lists them), no global variable, no non-static helper, every function of the subject |
| 🧪 Tests | below |

**Part 1, against the libc:** `ft_is*`, `ft_toupper`, `ft_tolower` for every value from `EOF` to 255 (a class function must return 1); `ft_memset`, `ft_bzero`, `ft_memcpy` at 8 offsets and 0 to 40 bytes; `ft_memmove` for every overlap both ways; `ft_memchr`, `ft_strchr`, `ft_strrchr` for every `c` from -300 to 600; `ft_memcmp`, `ft_strncmp` on unsigned chars, past `'\0'`, up to `SIZE_MAX`; `ft_atoi` on spaces, signs, junk, `INT_MIN`, `INT_MAX`. `ft_strlcpy`, `ft_strlcat` and `ft_strnstr`, missing from glibc, against a reference written after the BSD man pages, for every size. Buffers are compared whole, so a byte written past the end fails.

**Allocations:** `ft_calloc` zeroed, big enough, `NULL` on overflow, a pointer `free()` takes for 0; every returned string is checked against the size actually `malloc`'d, which catches the missing `+ 1`.

**Part 2:** `ft_substr` past the end, with `UINT_MAX` and `SIZE_MAX`; `ft_strjoin`, `ft_strtrim`; `ft_split` with delimiters in a row, `'\0'` and `\xff`, words and array sized right; `ft_itoa` around every power of 10; `ft_strmapi`, `ft_striteri` (index and address of each char); `ft_put*_fd` through a pipe, every byte for `ft_putchar_fd`.

**Part 3:** every list function on empty, single and longer lists, `del` and `f` calls counted and ordered, the source of `ft_lstmap` untouched.

**`malloc` failing:** each allocating function runs with its 1st, then 2nd... `malloc` failing, until every allocation site has failed once: it must return `NULL` and, under Valgrind, leak nothing.

## 🔍 How it works

- Every test runs in its own process with a timeout: a crash or an infinite loop fails that test only, and the remaining timeouts shrink to 1 second. A test on undefined behaviour catches the crash itself.
- `-Wl,--wrap=malloc` routes every `malloc` of the library through the tester, which records the sizes and can make the n-th one fail.
- The sources are built again with `-g` for the tests, in parallel, so that reports point to their lines; the test binaries run in parallel too, norminette from the start.

## 📋 Requirements

Linux with `bash` 4.3+, `cc`, `make`, `nm`, `valgrind`, `norminette`, `perl` and coreutils.

## 📁 Structure

```
libft-fairy/
├── run.sh               # the steps; its "fairy core" is shared by the trio
├── utils.c              # malloc wrapper, allocation sizes, helpers; the C fairy core
├── libft_fairy.h        # EXPECT, EACH_MALLOC_FAILING
├── basic_tests.c        # Part 1 and 2
├── basic_tests_lists.c  # Part 3
└── leak_tests.c         # under Valgrind
```

## 💖 Credits

Created with ✨ by **sys-harmony**. Special thanks to the 42 community for feedback and suggestions.
