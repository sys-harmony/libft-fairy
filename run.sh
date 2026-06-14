#!/bin/bash

set -u

RED='\033[0;31m'
GREEN='\033[0;32m'
PINK='\033[0;95m'
YELLOW='\033[0;33m'
RESET='\033[0m'
TIMEOUT_SEC=2

VERBOSE=0
for arg in "$@"; do
    case $arg in
        -v|--verbose)
            VERBOSE=1
            ;;
    esac
done

LIBFT_DIR=".."
BASIC_TESTER_NAME=".basic_tests"
LEAK_TESTER_NAME=".leak_tests"
BONUS_BASIC_TESTER_NAME=".basic_tests_bonus"
BONUS_LEAK_TESTER_NAME=".leak_tests_bonus"
TESTER_DIR=$(basename "$(pwd)")
TMP_DIR="/tmp/libft_fairy_$$"

mkdir -p "$TMP_DIR"

# ---- result accumulators (init up front so the EXIT trap is set -u safe) ----
NORM_TEST_RES=0
PROTO_TEST_RES=0
BONUS_PROTO_TEST_RES=0
MAKE_ERRORS=0
BUILD_FAIL=0
EXTERN_TEST_RES=0
BASIC_TESTS_RES=0
BONUS_BASIC_TESTS_RES=0
LEAK_TESTS_RES=0
BONUS_LEAK_TESTS_RES=0
BONUS_VERSION=0

# ---- alignment helpers ------------------------------------------------------
# Status words are right-aligned with \033[<n>G (absolute cursor column), so
# emoji width and label length never shift them — no tab arithmetic.
COL=27
ok()       { printf "\033[${COL}G  Done\n"; }
fail()     { printf "\033[${COL}G${RED}Failed${RESET}\n"; }
timeout_() { printf "\033[26G${YELLOW}Timeout${RESET}\n"; }

tests_passed() {
    [[ $NORM_TEST_RES -eq 0 && $PROTO_TEST_RES -eq 0 && $BONUS_PROTO_TEST_RES -eq 0 \
        && $MAKE_ERRORS -eq 0 && $BUILD_FAIL -eq 0 && $EXTERN_TEST_RES -eq 0 \
        && $BASIC_TESTS_RES -eq 0 && $BONUS_BASIC_TESTS_RES -eq 0 \
        && $LEAK_TESTS_RES -eq 0 && $BONUS_LEAK_TESTS_RES -eq 0 ]]
}

print_result() {
    echo ""
    if tests_passed; then
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
    rm -rf "$TMP_DIR" *.o $BASIC_TESTER_NAME $LEAK_TESTER_NAME \
        $BONUS_BASIC_TESTER_NAME $BONUS_LEAK_TESTER_NAME .results
    make -C $LIBFT_DIR fclean > /dev/null 2>&1
    ok
    echo ""
}

trap 'print_result; cleanup' EXIT INT TERM

check_external_functions() {
    local func=$1
    local allowed=$2
    local obj_file="$LIBFT_DIR/${func}.o"
    [ ! -f "$obj_file" ] && return 0
    local externals
    externals=$(nm -u "$obj_file" 2>/dev/null | awk '{print $2}')
    local forbidden=""
    for ext in $externals; do
        [[ "$ext" == ft_* || "$ext" == __* ]] && continue
        [[ ! " $allowed " =~ " $ext " ]] && forbidden="$forbidden $ext"
    done
    if [ -n "$forbidden" ]; then
        echo "$func: forbidden function detected:$forbidden"
        return 1
    fi
    return 0
}

check_externals_batch() {
    local funcs=$1
    local allowed=$2
    local output=""
    local failed=0
    for func in $funcs; do
        local result
        result=$(check_external_functions "$func" "$allowed" 2>&1)
        if [ $? -ne 0 ]; then
            failed=1
            output="${output}${result}\n"
        fi
    done
    echo -e "$output"
    return $failed
}

# runs a plain tester, appends its output to .results, returns its exit code
run_plain() {
    local tester=$1
    timeout ${TIMEOUT_SEC}s "./$tester" > "$TMP_DIR/out.tmp" 2>&1
    local rc=$?
    if grep -q "Segmentation fault" "$TMP_DIR/out.tmp" 2>/dev/null; then
        sed 's/^.*Segmentation fault/\n&/' "$TMP_DIR/out.tmp" >> .results
    else
        cat "$TMP_DIR/out.tmp" >> .results
    fi
    return $rc
}

# runs a tester under Valgrind (leak log -> $2), appends stray output, returns rc
run_leak() {
    local tester=$1
    local log=$2
    { timeout ${TIMEOUT_SEC}s valgrind --leak-check=full --show-leak-kinds=all \
        --errors-for-leak-kinds=all --error-exitcode=1 --track-origins=yes \
        --log-file="$log" "./$tester" > /dev/null; } > "$TMP_DIR/out.tmp" 2>&1
    local rc=$?
    if grep -q "Segmentation fault" "$TMP_DIR/out.tmp" 2>/dev/null; then
        sed 's/^.*Segmentation fault/\n&/' "$TMP_DIR/out.tmp" >> .results
    else
        cat "$TMP_DIR/out.tmp" >> .results
    fi
    return $rc
}

echo ""
echo -e "${PINK}╔══════════════════════════════╗${RESET}"
echo -e "${PINK}║        LIBFT-FAIRY 🧚        ║${RESET}"
echo -e "${PINK}╚══════════════════════════════╝${RESET}"
echo ""

printf "📝 Checking norm..."
NORM_OUTPUT=$(find $LIBFT_DIR -type d -name "$TESTER_DIR" -prune -o \
    \( -name "*.c" -o -name "*.h" \) -type f -print | xargs -r norminette 2>&1)
if echo "$NORM_OUTPUT" | grep -q "Error"; then
    NORM_TEST_RES=1
    fail
    echo ""
    echo "$NORM_OUTPUT" | grep "Error"
    echo ""
else
    ok
fi

printf "🔖 Checking version..."
if grep -qi "bonus:" ../Makefile; then
    BONUS_VERSION=1
    printf "\033[28GBonus\n"
else
    printf "\033[24G${YELLOW}Mandatory${RESET}\n"
fi

printf "📑 Checking prototypes..."
errors=0
error_messages=""
LIBFT_H="../libft.h"
mandatory_prototypes=(
    'int\t+ft_atoi\(const char \*nptr\);::int ft_atoi(const char *nptr);'
    'void\t+ft_bzero\(void \*s, size_t n\);::void ft_bzero(void *s, size_t n);'
    'void\t+\*ft_calloc\(size_t nmemb, size_t size\);::void *ft_calloc(size_t nmemb, size_t size);'
    'int\t+ft_isalnum\(int c\);::int ft_isalnum(int c);'
    'int\t+ft_isalpha\(int c\);::int ft_isalpha(int c);'
    'int\t+ft_isascii\(int c\);::int ft_isascii(int c);'
    'int\t+ft_isdigit\(int c\);::int ft_isdigit(int c);'
    'int\t+ft_isprint\(int c\);::int ft_isprint(int c);'
    'char\t+\*ft_itoa\(int n\);::char *ft_itoa(int n);'
    'void\t+\*ft_memchr\(const void \*s, int c, size_t n\);::void *ft_memchr(const void *s, int c, size_t n);'
    'int\t+ft_memcmp\(const void \*s1, const void \*s2, size_t n\);::int ft_memcmp(const void *s1, const void *s2, size_t n);'
    'void\t+\*ft_memcpy\(void \*dest, const void \*src, size_t n\);::void *ft_memcpy(void *dest, const void *src, size_t n);'
    'void\t+\*ft_memmove\(void \*dest, const void \*src, size_t n\);::void *ft_memmove(void *dest, const void *src, size_t n);'
    'void\t+\*ft_memset\(void \*s, int c, size_t n\);::void *ft_memset(void *s, int c, size_t n);'
    'void\t+ft_putchar_fd\(char c, int fd\);::void ft_putchar_fd(char c, int fd);'
    'void\t+ft_putendl_fd\(char \*s, int fd\);::void ft_putendl_fd(char *s, int fd);'
    'void\t+ft_putnbr_fd\(int n, int fd\);::void ft_putnbr_fd(int n, int fd);'
    'void\t+ft_putstr_fd\(char \*s, int fd\);::void ft_putstr_fd(char *s, int fd);'
    'char\t+\*\*ft_split\(char const \*s, char c\);::char **ft_split(char const *s, char c);'
    'char\t+\*ft_strchr\(const char \*s, int c\);::char *ft_strchr(const char *s, int c);'
    'char\t+\*ft_strdup\(const char \*s\);::char *ft_strdup(const char *s);'
    'void\t+ft_striteri\(char \*s, void \(\*f\)\(unsigned int, char\*\)\);::void ft_striteri(char *s, void (*f)(unsigned int, char*));'
    'char\t+\*ft_strjoin\(char const \*s1, char const \*s2\);::char *ft_strjoin(char const *s1, char const *s2);'
    'size_t\t+ft_strlcat\(char \*dst, const char \*src, size_t size\);::size_t ft_strlcat(char *dst, const char *src, size_t size);'
    'size_t\t+ft_strlcpy\(char \*dst, const char \*src, size_t size\);::size_t ft_strlcpy(char *dst, const char *src, size_t size);'
    'size_t\t+ft_strlen\(const char \*s\);::size_t ft_strlen(const char *s);'
    'char\t+\*ft_strmapi\(char const \*s, char \(\*f\)\(unsigned int, char\)\);::char *ft_strmapi(char const *s, char (*f)(unsigned int, char));'
    'int\t+ft_strncmp\(const char \*s1, const char \*s2, size_t n\);::int ft_strncmp(const char *s1, const char *s2, size_t n);'
    'char\t+\*ft_strnstr\(const char \*big, const char \*little, size_t len\);::char *ft_strnstr(const char *big, const char *little, size_t len);'
    'char\t+\*ft_strrchr\(const char \*s, int c\);::char *ft_strrchr(const char *s, int c);'
    'char\t+\*ft_strtrim\(char const \*s1, char const \*set\);::char *ft_strtrim(char const *s1, char const *set);'
    'char\t+\*ft_substr\(char const \*s, unsigned int start, size_t len\);::char *ft_substr(char const *s, unsigned int start, size_t len);'
    'int\t+ft_tolower\(int c\);::int ft_tolower(int c);'
    'int\t+ft_toupper\(int c\);::int ft_toupper(int c);'
)
for proto in "${mandatory_prototypes[@]}"; do
    regex="${proto%%::*}"
    display="${proto##*::}"
    if ! grep -Pq "$regex" "$LIBFT_H"; then
        ((errors++))
        if [ $errors -eq 1 ]; then
            error_messages+="Missing or malformed prototype(s), expected:"$'\n'
        fi
        error_messages+="$display"$'\n'
    fi
done
PROTO_TEST_RES=$errors
if [ $BONUS_VERSION -eq 1 ]; then
    errors=0
    bonus_prototypes=(
        'void\t+ft_lstadd_back\(t_list \*\*lst, t_list \*new\);::void ft_lstadd_back(t_list **lst, t_list *new);'
        'void\t+ft_lstadd_front\(t_list \*\*lst, t_list \*new\);::void ft_lstadd_front(t_list **lst, t_list *new);'
        'void\t+ft_lstclear\(t_list \*\*lst, void \(\*del\)\(void \*\)\);::void ft_lstclear(t_list **lst, void (*del)(void *));'
        'void\t+ft_lstdelone\(t_list \*lst, void \(\*del\)\(void \*\)\);::void ft_lstdelone(t_list *lst, void (*del)(void *));'
        'void\t+ft_lstiter\(t_list \*lst, void \(\*f\)\(void \*\)\);::void ft_lstiter(t_list *lst, void (*f)(void *));'
        't_list\t+\*ft_lstlast\(t_list \*lst\);::t_list *ft_lstlast(t_list *lst);'
        't_list\t+\*ft_lstmap\(t_list \*lst, void \*\(\*f\)\(void \*\), void \(\*del\)\(void \*\)\);::t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));'
        't_list\t+\*ft_lstnew\(void \*content\);::t_list *ft_lstnew(void *content);'
        'int\t+ft_lstsize\(t_list \*lst\);::int ft_lstsize(t_list *lst);'
    )
    for proto in "${bonus_prototypes[@]}"; do
        regex="${proto%%::*}"
        display="${proto##*::}"
        if ! grep -Pq "$regex" "$LIBFT_H"; then
            ((errors++))
            if [ $PROTO_TEST_RES -eq 0 ] && [ $errors -eq 1 ]; then
                error_messages+="Missing or malformed prototype(s), expected:"$'\n'
            fi
            error_messages+="$display"$'\n'
        fi
    done
    BONUS_PROTO_TEST_RES=$errors
fi
if [ $PROTO_TEST_RES -eq 0 ] && [ $BONUS_PROTO_TEST_RES -eq 0 ]; then
    ok
else
    fail
fi
if [ -n "$error_messages" ]; then
    echo ""
    echo -e "$error_messages"
fi

printf "🔧 Checking Makefile..."
MAKEFILE_PATH="$LIBFT_DIR/Makefile"
MAKE_ISSUES=""
if [ ! -f "$MAKEFILE_PATH" ]; then
    MAKE_ERRORS=1
    fail
    echo ""
    echo "No Makefile found!"
    exit 1
fi
if ! grep -Eq '(^|\s)cc(\s|$)' "$MAKEFILE_PATH"; then
    ((MAKE_ERRORS++))
    MAKE_ISSUES+="Missing or incorrect compiler (should use 'cc')"$'\n'
fi
for flag in -Wall -Wextra -Werror; do
    if ! grep -Fq -- "$flag" "$MAKEFILE_PATH"; then
        ((MAKE_ERRORS++))
        MAKE_ISSUES+="Missing compilation flag: $flag"$'\n'
    fi
done
for rule in all clean fclean re; do
    if ! grep -Eq "^$rule\s*:" "$MAKEFILE_PATH"; then
        ((MAKE_ERRORS++))
        MAKE_ISSUES+="Missing rule: $rule"$'\n'
    fi
done
if ! grep -Eq '^\$\(NAME\)\s*:' "$MAKEFILE_PATH"; then
    ((MAKE_ERRORS++))
    MAKE_ISSUES+="Missing rule: \$(NAME)"$'\n'
fi
make -C "$LIBFT_DIR" > /dev/null 2>&1 || true
lib_file=$(grep -E '^\$\(NAME\)\s*:' "$MAKEFILE_PATH" | awk '{print $1}' | tr -d ':')
[ -z "$lib_file" ] && lib_file="libft.a"
lib_file="$LIBFT_DIR/$lib_file"
timestamp_before=$(stat -c %Y "$lib_file" 2>/dev/null || echo 0)
make -C "$LIBFT_DIR" > /dev/null 2>&1 || true
timestamp_after=$(stat -c %Y "$lib_file" 2>/dev/null || echo 0)
if [ "$timestamp_after" -ne "$timestamp_before" ]; then
    ((MAKE_ERRORS++))
    MAKE_ISSUES+="Unnecessary relink detected when running make twice"$'\n'
fi
if [ $BONUS_VERSION -eq 1 ]; then
    if ! grep -Eq '_bonus\.c' "$MAKEFILE_PATH"; then
        ((MAKE_ERRORS++))
        MAKE_ISSUES+="Missing *_bonus.c file(s) in bonus version"$'\n'
    fi
fi
if grep -Eq '\*\.c' "$MAKEFILE_PATH" || grep -Eq '\*\.o' "$MAKEFILE_PATH"; then
    ((MAKE_ERRORS++))
    MAKE_ISSUES+="Forbidden wildcard inclusion detected (*.c or *.o)"$'\n'
fi
if [ $MAKE_ERRORS -eq 0 ]; then
    ok
else
    fail
    echo ""
    echo -e "$MAKE_ISSUES"
fi

printf "📦 Building libft..."
MAKE_CMD="make -C $LIBFT_DIR"
[ $BONUS_VERSION -eq 1 ] && MAKE_CMD="$MAKE_CMD bonus"
MAKE_OUTPUT=$($MAKE_CMD 2>&1)
if [ $? -eq 0 ]; then
    ok
else
    BUILD_FAIL=1
    fail
    echo ""
    echo -e "$MAKE_OUTPUT"
    echo ""
    exit 1
fi

printf "🔍 Checking externals..."
EXTERN_OUTPUT=""
no_extern="ft_striteri ft_isdigit ft_isalnum ft_isascii ft_isprint ft_strlen ft_memset ft_bzero ft_memcpy ft_memmove ft_strlcpy ft_strlcat ft_toupper ft_tolower ft_strchr ft_strrchr ft_strncmp ft_memchr ft_memcmp ft_strnstr ft_atoi"
malloc_funcs="ft_strmapi ft_itoa ft_calloc ft_strdup ft_substr ft_strjoin ft_strtrim"
write_funcs="ft_putchar_fd ft_putstr_fd ft_putendl_fd ft_putnbr_fd"
result=$(check_externals_batch "$no_extern" "")
[ $? -ne 0 ] && EXTERN_TEST_RES=1 && EXTERN_OUTPUT="$EXTERN_OUTPUT$result"
result=$(check_externals_batch "$malloc_funcs" "malloc")
[ $? -ne 0 ] && EXTERN_TEST_RES=1 && EXTERN_OUTPUT="$EXTERN_OUTPUT$result"
result=$(check_external_functions "ft_split" "malloc free" 2>&1)
[ $? -ne 0 ] && EXTERN_TEST_RES=1 && EXTERN_OUTPUT="${EXTERN_OUTPUT}${result}\n"
result=$(check_externals_batch "$write_funcs" "write")
[ $? -ne 0 ] && EXTERN_TEST_RES=1 && EXTERN_OUTPUT="$EXTERN_OUTPUT$result"
if [ $BONUS_VERSION -eq 1 ]; then
    bonus_no_extern="ft_lstiter ft_lstadd_front ft_lstsize ft_lstlast ft_lstadd_back"
    result=$(check_externals_batch "$bonus_no_extern" "")
    [ $? -ne 0 ] && EXTERN_TEST_RES=1 && EXTERN_OUTPUT="$EXTERN_OUTPUT$result"
    for func_pair in "ft_lstnew:malloc" "ft_lstdelone:free" "ft_lstclear:free" "ft_lstmap:malloc free"; do
        IFS=':' read -r func allowed <<< "$func_pair"
        result=$(check_external_functions "$func" "$allowed" 2>&1)
        [ $? -ne 0 ] && EXTERN_TEST_RES=1 && EXTERN_OUTPUT="${EXTERN_OUTPUT}${result}\n"
    done
fi
if [ $EXTERN_TEST_RES -eq 0 ]; then
    ok
else
    fail
    echo ""
    echo -e "$EXTERN_OUTPUT"
fi

printf "🔨 Building tests..."
VERBOSE_FLAG=""
[ $VERBOSE -eq 1 ] && VERBOSE_FLAG="-DVERBOSE=1"
COMPILE_FLAGS="-Wall -Wextra -Werror -no-pie $VERBOSE_FLAG -Wl,--wrap=malloc"
BUILD_ERRORS=""
compile_test() {
    local src_file=$1
    local output_file=$2
    local label=$3
    local compile_output
    if ! compile_output=$(gcc $COMPILE_FLAGS "$src_file" utils.c -L$LIBFT_DIR -lft -I$LIBFT_DIR -o "$output_file" 2>&1); then
        BUILD_ERRORS+="$label compilation failed:\n$compile_output"$'\n'
        return 1
    fi
    return 0
}
compile_test basic_tests.c "$BASIC_TESTER_NAME" "basic_tests" || BUILD_FAIL=1
compile_test leak_tests.c "$LEAK_TESTER_NAME" "leak_tests" || BUILD_FAIL=1
if [ $BONUS_VERSION -eq 1 ]; then
    compile_test basic_tests_bonus.c "$BONUS_BASIC_TESTER_NAME" "bonus basic_tests" || BUILD_FAIL=1
    compile_test leak_tests_bonus.c "$BONUS_LEAK_TESTER_NAME" "bonus leak_tests" || BUILD_FAIL=1
fi
if [ $BUILD_FAIL -eq 0 ]; then
    ok
else
    fail
    echo ""
    echo -e "$BUILD_ERRORS"
    exit 1
fi

printf "🧪 Running tests..."
: > .results
run_plain "$BASIC_TESTER_NAME"
BASIC_TESTS_RES=$?
if [ $BONUS_VERSION -eq 1 ]; then
    run_plain "$BONUS_BASIC_TESTER_NAME"
    BONUS_BASIC_TESTS_RES=$?
fi
run_leak "$LEAK_TESTER_NAME" "$TMP_DIR/valgrind_output.log"
LEAK_TESTS_RES=$?
if [ $BONUS_VERSION -eq 1 ]; then
    run_leak "$BONUS_LEAK_TESTER_NAME" "$TMP_DIR/bonus_valgrind_output.log"
    BONUS_LEAK_TESTS_RES=$?
fi
if [ $LEAK_TESTS_RES -ne 0 ] || [ $VERBOSE -eq 1 ]; then
    {
        echo ""
        echo "════════════════════════════════════════"
        echo "VALGRIND OUTPUT (mandatory)"
        echo "════════════════════════════════════════"
        cat "$TMP_DIR/valgrind_output.log"
    } >> .results
fi
if [ $BONUS_LEAK_TESTS_RES -ne 0 ] || ( [ $VERBOSE -eq 1 ] && [ $BONUS_VERSION -eq 1 ] ); then
    {
        echo ""
        echo "════════════════════════════════════════"
        echo "VALGRIND OUTPUT (bonus)"
        echo "════════════════════════════════════════"
        cat "$TMP_DIR/bonus_valgrind_output.log"
    } >> .results
fi
if [ $BASIC_TESTS_RES -eq 124 ] || [ $BONUS_BASIC_TESTS_RES -eq 124 ] \
    || [ $LEAK_TESTS_RES -eq 124 ] || [ $BONUS_LEAK_TESTS_RES -eq 124 ]; then
    timeout_
elif tests_passed; then
    ok
else
    fail
fi
cat .results

tests_passed
exit $?
