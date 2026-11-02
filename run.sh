#!/bin/bash

set -u

RED='\033[0;31m'
GREEN='\033[0;32m'
PINK='\033[0;95m'
YELLOW='\033[0;33m'
RESET='\033[0m'
TIMEOUT_SEC=10	# per test binary; each forked test also has its own TEST_TIMEOUT

VERBOSE=0
for arg in "$@"; do
    case $arg in
        -v|--verbose)
            VERBOSE=1
            ;;
    esac
done

cd "$(dirname "$0")" || exit 1
LIBFT_DIR=".."
LIBFT_A="$LIBFT_DIR/libft.a"
MAKEFILE_PATH="$LIBFT_DIR/Makefile"
BASIC_TESTER_NAME=".basic_tests"
LEAK_TESTER_NAME=".leak_tests"
LISTS_BASIC_TESTER_NAME=".basic_tests_lists"
LISTS_LEAK_TESTER_NAME=".leak_tests_lists"

for tool in gcc make valgrind norminette nm; do
    if ! command -v "$tool" > /dev/null; then
        echo "libft-fairy: '$tool' is required but not installed"
        exit 1
    fi
done
if [ ! -f "$LIBFT_DIR/libft.h" ] || [ ! -f "$MAKEFILE_PATH" ]; then
    echo "libft-fairy: no libft.h or Makefile in $(cd "$LIBFT_DIR" && pwd), clone me inside your libft repository"
    exit 1
fi
TMP_DIR=$(mktemp -d /tmp/libft_fairy_XXXXXX)

# ---- result accumulators (init up front so the EXIT trap is set -u safe) ----
NORM_TEST_RES=0
README_TEST_RES=0
PROTO_TEST_RES=0
MAKE_ERRORS=0
BUILD_FAIL=0
EXTERN_TEST_RES=0
BASIC_TESTS_RES=0
LISTS_BASIC_TESTS_RES=0
LEAK_TESTS_RES=0
LISTS_LEAK_TESTS_RES=0
INTERRUPTED=0

# ---- alignment helpers ------------------------------------------------------
# Status words are right-aligned with \033[<n>G (absolute cursor column), so
# emoji width and label length never shift them — no tab arithmetic.
COL=27
ok()       { printf "\033[${COL}G  Done\n"; }
fail()     { printf "\033[${COL}G${RED}Failed${RESET}\n"; }
timeout_() { printf "\033[26G${YELLOW}Timeout${RESET}\n"; }

tests_ran_ok() {
    [[ $BASIC_TESTS_RES -eq 0 && $LISTS_BASIC_TESTS_RES -eq 0 \
        && $LEAK_TESTS_RES -eq 0 && $LISTS_LEAK_TESTS_RES -eq 0 ]]
}

tests_passed() {
    [[ $NORM_TEST_RES -eq 0 && $README_TEST_RES -eq 0 && $PROTO_TEST_RES -eq 0 \
        && $MAKE_ERRORS -eq 0 && $BUILD_FAIL -eq 0 && $EXTERN_TEST_RES -eq 0 ]] && tests_ran_ok
}

print_result() {
    echo ""
    if [ $INTERRUPTED -eq 1 ]; then
        echo -e "${YELLOW}Interrupted${RESET}"
    elif tests_passed; then
        echo -e "${GREEN}╔══════════════════════════════╗${RESET}"
        echo -e "${GREEN}║      OH MY, YOU PASSED!      ║${RESET}"
        echo -e "${GREEN}╚══════════════════════════════╝${RESET}"
    else
        echo -e "${RED}╔══════════════════════════════╗${RESET}"
        echo -e "${RED}║     OH NO... YOU FAILED!     ║${RESET}"
        echo -e "${RED}╚══════════════════════════════╝${RESET}"
    fi
}

cleanup() {
    echo ""
    printf "🧹 Cleaning up..."
    rm -rf "$TMP_DIR" $BASIC_TESTER_NAME $LEAK_TESTER_NAME \
        $LISTS_BASIC_TESTER_NAME $LISTS_LEAK_TESTER_NAME .results
    make -C $LIBFT_DIR fclean > /dev/null 2>&1
    ok
    echo ""
}

# INT/TERM only exit: the EXIT trap then prints the result and cleans up, once
trap 'INTERRUPTED=1; exit 130' INT TERM
trap 'print_result; cleanup' EXIT

# runs a test binary (under Valgrind when a log file is given), appends its output to .results
run_tester() {
    local tester=$1
    local log=${2:-}
    local cmd=("./$tester")
    local rc
    [ -x "$tester" ] || return 0
    [ -n "$log" ] && cmd=(valgrind --leak-check=full --show-leak-kinds=all \
        --errors-for-leak-kinds=all --error-exitcode=1 --track-origins=yes --log-file="$log" "./$tester")
    # the subshell keeps bash's own "Segmentation fault ... <command>" message out of the output
    ( timeout "$TIMEOUT_SEC" "${cmd[@]}" > "$TMP_DIR/out.tmp" 2>&1; exit $? ) 2> /dev/null
    rc=$?
    cat "$TMP_DIR/out.tmp" >> .results
    if [ $rc -eq 124 ]; then
        echo -e "\n${YELLOW}$tester: timeout after ${TIMEOUT_SEC}s${RESET}" >> .results
    elif [ $rc -gt 128 ]; then
        echo -e "\n${RED}$tester: crashed (SIG$(kill -l $((rc - 128))))${RESET}" >> .results
    fi
    return $rc
}

show_valgrind_log() {
    local title=$1
    local log=$2
    local rc=$3
    [ -f "$log" ] || return 0
    [ "$rc" -ne 0 ] || [ $VERBOSE -eq 1 ] || return 0
    {
        echo ""
        echo "════════════════════════════════════════"
        echo "VALGRIND OUTPUT ($title)"
        echo "════════════════════════════════════════"
        cat "$log"
    } >> .results
}

echo ""
echo -e "${PINK}╔══════════════════════════════╗${RESET}"
echo -e "${PINK}║        LIBFT-FAIRY 🧚        ║${RESET}"
echo -e "${PINK}╚══════════════════════════════╝${RESET}"
echo ""

printf "📝 Checking norm..."
# the subject requires every file at the root of the repository
NORM_OUTPUT=$(norminette "$LIBFT_DIR"/*.c "$LIBFT_DIR"/*.h 2>&1)
if echo "$NORM_OUTPUT" | grep -q "Error"; then
    NORM_TEST_RES=1
    fail
    echo ""
    echo "$NORM_OUTPUT" | grep "Error"
    echo ""
else
    ok
fi

printf "📖 Checking README..."
README="$LIBFT_DIR/README.md"
README_ISSUES=""
if [ ! -f "$README" ]; then
    README_ISSUES+="No README.md at the root of the repository"$'\n'
else
    head -n 1 "$README" | grep -Eq '^[*_]+This project has been created as part of the 42 curriculum by .+[*_]+[[:space:]]*$' \
        || README_ISSUES+="First line must be in italics: *This project has been created as part of the 42 curriculum by <login>.*"$'\n'
    for section in Description Instructions Resources; do
        grep -Eiq "^#+[[:space:]]*$section" "$README" || README_ISSUES+="Missing section: $section"$'\n'
    done
fi
if [ -z "$README_ISSUES" ]; then
    ok
else
    README_TEST_RES=1
    fail
    echo ""
    printf '%s\n' "$README_ISSUES"
fi

printf "📑 Checking prototypes..."
PROTOTYPES='int ft_isalpha(int c);
int ft_isdigit(int c);
int ft_isalnum(int c);
int ft_isascii(int c);
int ft_isprint(int c);
size_t ft_strlen(const char *s);
void *ft_memset(void *s, int c, size_t n);
void ft_bzero(void *s, size_t n);
void *ft_memcpy(void *dest, const void *src, size_t n);
void *ft_memmove(void *dest, const void *src, size_t n);
size_t ft_strlcpy(char *dst, const char *src, size_t size);
size_t ft_strlcat(char *dst, const char *src, size_t size);
int ft_toupper(int c);
int ft_tolower(int c);
char *ft_strchr(const char *s, int c);
char *ft_strrchr(const char *s, int c);
int ft_strncmp(const char *s1, const char *s2, size_t n);
void *ft_memchr(const void *s, int c, size_t n);
int ft_memcmp(const void *s1, const void *s2, size_t n);
char *ft_strnstr(const char *big, const char *little, size_t len);
int ft_atoi(const char *nptr);
void *ft_calloc(size_t nmemb, size_t size);
char *ft_strdup(const char *s);
char *ft_substr(char const *s, unsigned int start, size_t len);
char *ft_strjoin(char const *s1, char const *s2);
char *ft_strtrim(char const *s1, char const *set);
char **ft_split(char const *s, char c);
char *ft_itoa(int n);
char *ft_strmapi(char const *s, char (*f)(unsigned int, char));
void ft_striteri(char *s, void (*f)(unsigned int, char*));
void ft_putchar_fd(char c, int fd);
void ft_putstr_fd(char *s, int fd);
void ft_putendl_fd(char *s, int fd);
void ft_putnbr_fd(int n, int fd);
t_list *ft_lstnew(void *content);
void ft_lstadd_front(t_list **lst, t_list *new);
unsigned int ft_lstsize(t_list *lst);
t_list *ft_lstlast(t_list *lst);
void ft_lstadd_back(t_list **lst, t_list *new);
void ft_lstdelone(t_list *lst, void (*del)(void *));
void ft_lstclear(t_list **lst, void (*del)(void *));
void ft_lstiter(t_list *lst, void (*f)(void *));
t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));'
PROTO_ISSUES=""
grep -qw restrict "$LIBFT_DIR/libft.h" && PROTO_ISSUES+="The 'restrict' qualifier is forbidden"$'\n'
# The compiler checks the types (parameter names and spacing don't matter):
# a missing prototype is an undeclared identifier, a wrong one has conflicting types.
{
    echo '#include "libft.h"'
    echo 'void libft_fairy_uses(void) {'
    grep -oE 'ft_[a-z_]+\(' <<< "$PROTOTYPES" | sed 's/^/(void)/; s/($/;/'
    echo '}'
    echo "$PROTOTYPES"
} > "$TMP_DIR/prototypes.c"
if ! PROTO_LOG=$(gcc -fsyntax-only -I"$LIBFT_DIR" "$TMP_DIR/prototypes.c" 2>&1); then
    PROTO_FUNCS=$(grep 'error:' <<< "$PROTO_LOG" | sed 's/.*error: //' | grep -oE 'ft_[a-z_]+' | sort -u)
    if [ -z "$PROTO_FUNCS" ]; then
        PROTO_ISSUES+="$PROTO_LOG"$'\n'
    else
        PROTO_ISSUES+="Missing or mismatching prototype(s), expected:"$'\n'
        for func in $PROTO_FUNCS; do
            PROTO_ISSUES+="$(grep -E "\b$func\(" <<< "$PROTOTYPES")"$'\n'
        done
    fi
fi
if [ -z "$PROTO_ISSUES" ]; then
    ok
else
    PROTO_TEST_RES=1
    fail
    echo ""
    printf '%s\n' "$PROTO_ISSUES"
fi

printf "🔧 Checking Makefile..."
MAKE_ISSUES=""
for rule in all clean fclean re; do
    grep -Eq "^$rule\s*:" "$MAKEFILE_PATH" || MAKE_ISSUES+="Missing rule: $rule"$'\n'
done
grep -Eq '^\$(\(NAME\)|\{NAME\})\s*:' "$MAKEFILE_PATH" || MAKE_ISSUES+="Missing rule: \$(NAME)"$'\n'
grep -v '^\s*#' "$MAKEFILE_PATH" | grep -q '\*\.c' && MAKE_ISSUES+="Forbidden wildcard inclusion detected (*.c)"$'\n'
# compilation commands as make would run them, with every variable expanded
COMPILE_CMDS=$(make -nB -C "$LIBFT_DIR" --no-print-directory 2>/dev/null | grep -E '(^|\s)-c(\s|$)')
if [ -z "$COMPILE_CMDS" ]; then
    MAKE_ISSUES+="No compilation command found with 'make -n'"$'\n'
else
    grep -vqE '(^|[;&|/[:space:]])cc\s' <<< "$COMPILE_CMDS" && MAKE_ISSUES+="Sources must be compiled with cc"$'\n'
    for flag in -Wall -Wextra -Werror; do
        grep -vq -- "$flag" <<< "$COMPILE_CMDS" && MAKE_ISSUES+="Missing compilation flag: $flag"$'\n'
    done
    grep -q -- '-std=c99' <<< "$COMPILE_CMDS" && MAKE_ISSUES+="Forbidden flag: -std=c99"$'\n'
fi
# nanosecond timestamps: a relink happens in the same second as the previous build
make -C "$LIBFT_DIR" > /dev/null 2>&1
LIB_TIME=$(stat -c %y "$LIBFT_A" 2>/dev/null)
make -C "$LIBFT_DIR" > /dev/null 2>&1
[ "$(stat -c %y "$LIBFT_A" 2>/dev/null)" != "$LIB_TIME" ] \
    && MAKE_ISSUES+="Unnecessary relink detected when running make twice"$'\n'
[ -f "$LIBFT_A" ] || MAKE_ISSUES+="make does not create libft.a at the root of the repository"$'\n'
if [ -z "$MAKE_ISSUES" ]; then
    ok
else
    MAKE_ERRORS=1
    fail
    echo ""
    printf '%s\n' "$MAKE_ISSUES"
fi

printf "📦 Building libft..."
# subject v19.3 has no bonus part: make alone must build Part 1, 2 and 3
if MAKE_OUTPUT=$(make -C "$LIBFT_DIR" 2>&1); then
    ok
else
    BUILD_FAIL=1
    fail
    echo ""
    printf '%s\n\n' "$MAKE_OUTPUT"
    exit 1
fi

printf "🔍 Checking symbols..."
# external functions each libft function may call, all others are forbidden
ALLOWED="ft_calloc:malloc ft_strdup:malloc ft_substr:malloc ft_strjoin:malloc ft_strtrim:malloc
ft_split:malloc,free ft_itoa:malloc ft_strmapi:malloc ft_putchar_fd:write ft_putstr_fd:write
ft_putendl_fd:write ft_putnbr_fd:write ft_lstnew:malloc ft_lstdelone:free ft_lstclear:free ft_lstmap:malloc,free"
# One pass over every object of the archive, wherever the .o files were built
# and whatever functions each of them holds
SYMBOLS_OUTPUT=$(nm -A "$LIBFT_A" 2>/dev/null | awk -v allowed="$ALLOWED" '
    BEGIN { n = split(allowed, pairs); for (i = 1; i <= n; i++) { split(pairs[i], kv, ":"); ok[kv[1]] = kv[2] } }
    { split($1, path, ":"); obj = path[2]; type = $(NF - 1); sym = $NF }
    type == "T" && sym ~ /^ft_/ { allowed_in[obj] = allowed_in[obj] "," ok[sym]; next }
    type == "T" { print obj ": non-static function: " sym }
    type ~ /^[BCDGR]$/ { print obj ": global variable: " sym }
    type == "U" && sym !~ /^(ft_|__stack_chk_|_GLOBAL_OFFSET_TABLE_$)/ { calls[obj] = calls[obj] " " sym }
    END {
        for (obj in calls) {
            n = split(calls[obj], c)
            for (i = 1; i <= n; i++)
                if (index(allowed_in[obj] ",", "," c[i] ",") == 0)
                    print obj ": forbidden function: " c[i] (c[i] ~ /^__ctype/ ? " (<ctype.h> macro)" : "")
        }
    }')
DEFINED=$(nm -g --defined-only "$LIBFT_A" 2>/dev/null | awk '{ print $NF }')
for func in $(grep -oE 'ft_[a-z_]+' <<< "$PROTOTYPES"); do
    grep -qx "$func" <<< "$DEFINED" || SYMBOLS_OUTPUT+="${SYMBOLS_OUTPUT:+$'\n'}libft.a: function not built by make: $func"
done
if [ -z "$SYMBOLS_OUTPUT" ]; then
    ok
else
    EXTERN_TEST_RES=1
    fail
    echo ""
    printf '%s\n\n' "$SYMBOLS_OUTPUT"
fi

printf "🔨 Building tests..."
VERBOSE_FLAG=""
[ $VERBOSE -eq 1 ] && VERBOSE_FLAG="-DVERBOSE=1"
COMPILE_FLAGS="-Wall -Wextra -Werror -Wno-missing-field-initializers -no-pie $VERBOSE_FLAG -Wl,--wrap=malloc"
BUILD_ERRORS=""
compile_test() {
    local src_file=$1
    local output_file=$2
    local compile_output
    rm -f "$output_file"
    if ! compile_output=$(gcc $COMPILE_FLAGS "$src_file" utils.c -L$LIBFT_DIR -lft -I$LIBFT_DIR -o "$output_file" 2>&1); then
        BUILD_ERRORS+="$src_file compilation failed:"$'\n'"$(head -n 15 <<< "$compile_output")"$'\n'
        return 1
    fi
    return 0
}
# a test file that does not compile is reported and skipped, the others still run
compile_test basic_tests.c "$BASIC_TESTER_NAME" || BUILD_FAIL=1
compile_test leak_tests.c "$LEAK_TESTER_NAME" || BUILD_FAIL=1
compile_test basic_tests_lists.c "$LISTS_BASIC_TESTER_NAME" || BUILD_FAIL=1
compile_test leak_tests_lists.c "$LISTS_LEAK_TESTER_NAME" || BUILD_FAIL=1
if [ $BUILD_FAIL -eq 0 ]; then
    ok
else
    fail
    echo ""
    printf '%s\n' "$BUILD_ERRORS"
fi

printf "🧪 Running tests..."
: > .results
run_tester "$BASIC_TESTER_NAME"
BASIC_TESTS_RES=$?
run_tester "$LISTS_BASIC_TESTER_NAME"
LISTS_BASIC_TESTS_RES=$?
run_tester "$LEAK_TESTER_NAME" "$TMP_DIR/valgrind_output.log"
LEAK_TESTS_RES=$?
run_tester "$LISTS_LEAK_TESTER_NAME" "$TMP_DIR/lists_valgrind_output.log"
LISTS_LEAK_TESTS_RES=$?
show_valgrind_log "Part 1 & 2" "$TMP_DIR/valgrind_output.log" $LEAK_TESTS_RES
show_valgrind_log "Part 3, linked lists" "$TMP_DIR/lists_valgrind_output.log" $LISTS_LEAK_TESTS_RES
if [ $BASIC_TESTS_RES -eq 124 ] || [ $LISTS_BASIC_TESTS_RES -eq 124 ] \
    || [ $LEAK_TESTS_RES -eq 124 ] || [ $LISTS_LEAK_TESTS_RES -eq 124 ]; then
    timeout_
elif tests_ran_ok; then
    ok
else
    fail
fi
cat .results

tests_passed
exit $?
