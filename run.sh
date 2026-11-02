#!/bin/bash
# libft-fairy: a tester for 42's libft (subject v19.3, linked lists mandatory).
# Clone it inside your libft repository, then: ./run.sh [-v|--verbose]

# ═════════════════════════════════ fairy core ═════════════════════════════════
# Identical in libft-fairy, printf-fairy and gnl-fairy: display, steps, the
# checks the three subjects share, and the parallel test runner.

set -u

RED=$'\033[31m' GREEN=$'\033[32m' YELLOW=$'\033[33m' PINK=$'\033[95m'
BOLD=$'\033[1m' DIM=$'\033[2m' RESET=$'\033[0m'
WIDTH=46                        # banner width; every status word ends there
JOBS=$(nproc 2> /dev/null || echo 4)
VERBOSE=0

for arg in "$@"; do
    case $arg in
        -v|--verbose) VERBOSE=1 ;;
        -h|--help)    echo "usage: $0 [-v|--verbose]"; exit 0 ;;
        *)            echo "usage: $0 [-v|--verbose]" >&2; exit 2 ;;
    esac
done

TESTER_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
TESTER=$(basename "$TESTER_DIR")
cd "$TESTER_DIR/.." || exit 1
TMP=$(mktemp -d "/tmp/$TESTER.XXXXXX") || exit 1
export FAIRY_STATS=$TMP/stats   # each test binary appends "passed failed warned"
FAILED=0 WARNED=0 FAILED_STEPS="" START=$(date +%s%N)
declare -A PIDS

# ---- display -----------------------------------------------------------------
# box <color> <text> [emoji]: a WIDTH-wide double-line box, text centered
box() {
    local text=$2 w=${#2} bar l
    [[ -n ${3:-} ]] && text+=" $3" w=$((w + 3))
    printf -v bar '%*s' $((WIDTH - 2)) ''
    l=$(( (WIDTH - 2 - w) / 2 ))
    printf '%s╔%s╗\n║%*s%s%*s║\n╚%s╝%s\n' "$1" "${bar// /═}" $l '' "$text" \
        $((WIDTH - 2 - w - l)) '' "${bar// /═}" "$RESET"
}

# step <emoji> <label> opens a step line; ok, fail, warn or note closes it with
# a dot leader and a status word ending in column WIDTH. Details go below.
step() { STEP=$2 STEP_COL=$((${#2} + 4)); printf '%s %s ' "$1" "$2"; }
status() {
    local dots
    printf -v dots '%*s' $((WIDTH - STEP_COL - ${#2} - 1)) ''
    printf '%s%s%s %s%s%s\n' "$DIM" "${dots// /.}" "$RESET" "$1" "$2" "$RESET"
}
ok()   { status "$GREEN" Done; }
fail() { FAILED=1 FAILED_STEPS+="${FAILED_STEPS:+, }$STEP"; status "$RED" Failed; }
warn() { WARNED=1; status "$YELLOW" Warning; }
note() { status "$BOLD" "$1"; }
# verdict <issues...>: fails with one line per issue, or succeeds
verdict() { if [[ $# -eq 0 ]]; then ok; else fail; for i in "$@"; do say "$i"; done; fi; }

say()   { printf '   ↳ %s\n' "$*"; }          # one detail line under a step
quote() { sed 's/^/     /'; }                  # indents what it reads

# section <title>: the header of a block of results, as test binaries print it
section() {
    local line n=$((WIDTH - ${#1} - 4))
    printf -v line '%*s' $((n > 3 ? n : 3)) ''
    printf '\n%s──%s %s%s%s %s%s%s\n' "$DIM" "$RESET" "$BOLD" "$1" "$RESET" "$DIM" "${line// /─}" "$RESET"
}

# ---- end of run: cleanup and verdict -----------------------------------------
clean_project() { :; }
kill_tree() { local c; for c in $(pgrep -P "$1" 2> /dev/null); do kill_tree "$c"; done; kill "$1" 2> /dev/null; }

finish() {
    local rc=$? pid total=0 ko=0 secs
    for pid in $(pgrep -P $$ 2> /dev/null); do kill_tree "$pid"; done
    if [[ $rc -eq 130 ]]; then
        clean_project; rm -rf "$TMP"
        printf '\n%sInterrupted: no verdict.%s\n\n' "$YELLOW" "$RESET"
        return
    fi
    step 🧹 Cleanup; clean_project; ok
    [[ -f $FAIRY_STATS ]] && read -r total ko < <(awk '{p += $1; f += $2} END {print p + f, f + 0}' "$FAIRY_STATS")
    rm -rf "$TMP"
    secs=$(( ($(date +%s%N) - START) / 10000000 ))
    printf -v secs '%d.%02ds' $((secs / 100)) $((secs % 100))
    echo
    if [[ $FAILED -eq 0 ]]; then
        box "$GREEN" "OH MY, YOU PASSED!"
        printf '   %s✔ %d tests passed in %s%s' "$GREEN" "$total" "$secs" "$RESET"
        [[ $WARNED -eq 1 ]] && printf '  %s⚠ warnings above: not failures, worth a look%s' "$YELLOW" "$RESET"
    else
        box "$RED" "OH NO... YOU FAILED!"
        printf '   %s✘ %s' "$RED" "$FAILED_STEPS"
        [[ $ko -gt 0 ]] && printf ' · %d/%d tests failed' "$ko" "$total"
        printf ' · %s%s' "$secs" "$RESET"
    fi
    printf '\n\n'
}
trap finish EXIT
trap 'exit 130' INT TERM

# ---- background jobs ---------------------------------------------------------
# job <name> <cmd...>: runs cmd in the background, at most JOBS at once. Its
# output lands in $TMP/<name>.log, its exit status in $TMP/<name>.rc.
job() {
    local name=$1
    shift
    while [[ $(jobs -rp | wc -l) -ge $JOBS ]]; do wait -n; done
    { "$@" > "$TMP/$name.log" 2>&1; echo $? > "$TMP/$name.rc"; } &
    PIDS[$name]=$!
}
await() { local n; for n in "$@"; do wait "${PIDS[$n]}" 2> /dev/null; done; }
rc() { cat "$TMP/$1.rc" 2> /dev/null || echo 1; }

# ---- checks shared by the three subjects -------------------------------------
in_git() { git rev-parse --is-inside-work-tree > /dev/null 2>&1; }

# sources: the .c and .h files a grader gets. In a git repository that already
# tracks some: the tracked ones (a local main.c is fine). Otherwise all.
sources() {
    local files=""
    in_git && files=$(git ls-files -- '*.c' '*.h' | grep -v "^$TESTER/")
    [[ -z $files ]] && files=$(find . -path "./$TESTER" -prune -o -type f \( -name '*.c' -o -name '*.h' \) -print | sed 's|^\./||' | sort)
    [[ -n $files ]] && printf '%s\n' "$files"
}

# report_untracked <pathspecs...>: warns about the project files git would not
# hand to the grader. Returns 1 when there is none.
report_untracked() {
    local files
    files=$(in_git && git ls-files -o --exclude-standard -- "$@" | grep -v "^$TESTER/" | xargs)
    [[ -z $files ]] && return 1
    warn; say "not committed, hence not graded: $files"
}

# Norminette starts in the background at once: it is the slowest check.
norm_start() { [[ -n $(sources) ]] && job norm norminette $(sources); }
check_norm() {
    step 📝 Norm
    await norm
    if ! command -v norminette > /dev/null; then
        warn; say "norminette not found: the norm was NOT checked"
    elif [[ -z $(sources) ]]; then
        fail; say "no .c or .h file to check"
    elif [[ $(rc norm) -eq 0 ]]; then
        ok
    else
        fail
        grep -Ev 'OK!$|^Setting locale' "$TMP/norm.log" \
            | awk 'NR <= 20; END {if (NR > 20) print "… " NR - 20 " more lines"}' | quote
    fi
}

# check_readme [topic]: README.md as the subjects describe it. A missing one
# fails; its shape only warns, as a pattern cannot read prose.
check_readme() {
    local issues=() section
    step 📖 README
    if [[ ! -f README.md ]]; then
        fail; say "no README.md at the root of the repository"; return
    fi
    head -n 1 README.md | grep -qE '^\s*(\*|_|<i>|<em>)This project has been created as part of the 42 curriculum by ' \
        || issues+=("first line, in italics: *This project has been created as part of the 42 curriculum by <login>*")
    for section in Description Instructions Res+ources; do
        grep -qiE "^\s*(#+|[*_]{1,2}).*$section" README.md || issues+=("no \"${section/s+/s}\" section")
    done
    grep -qiE '\bAI\b|artificial intelligence|chatgpt|claude|copilot|gemini|LLM' README.md \
        || issues+=("Resources: say how AI was used, for which tasks and parts")
    [[ -n ${1:-} ]] && ! grep -qi "$1" README.md && issues+=("no explanation of the $1")
    if [[ ${#issues[@]} -eq 0 ]]; then
        ok
    else
        warn; for section in "${issues[@]}"; do say "$section"; done
    fi
}

# emitted <symbol>: a call the compiler emits on its own (array initializer,
# struct copy, stack protector), never written in the sources (comments aside)
emitted() {
    [[ -f $TMP/code ]] || sources | grep '\.c$' | xargs -r cat \
        | { perl -0777 -pe 's{/\*.*?\*/}{ }gs; s{//[^\n]*}{}g' 2> /dev/null || cat; } > "$TMP/code"
    [[ $1 =~ ^(memset|memcpy|memmove|__stack_chk_fail|_GLOBAL_OFFSET_TABLE_)$ ]] && ! grep -qw -- "$1" "$TMP/code"
}

# ---- test runs ---------------------------------------------------------------
# tests <title> <binary>: a test binary prints the sections that did not pass
# and exits 0, 1 (failure) or 2 (warnings). Anything else is reported here.
tests() {
    local rc
    (timeout --foreground 600 "$2" 2>&1) 2> /dev/null     # no "Segmentation fault" from bash
    rc=$?
    [[ $rc -le 2 ]] && return $rc
    section "$1"
    if [[ $rc -eq 124 ]]; then say "timed out"; else say "crashed: SIG$(kill -l $((rc - 128)) 2> /dev/null)"; fi
    if [[ -n ${LENIENT:-} ]]; then echo "1 0 1" >> "$FAIRY_STATS"; return 2; fi
    echo "0 1 0" >> "$FAIRY_STATS"
    return 1
}

# memcheck <title> <binary> [args...]: runs it under Valgrind, where VG_LEAKS
# are the leak kinds that fail. Prints the report if it found something.
memcheck() {
    local log=$TMP/vg.$BASHPID rc note=""
    if ! command -v valgrind > /dev/null; then
        echo "1 0 1" >> "$FAIRY_STATS"
        section "valgrind · $1"; say "valgrind not found: leaks NOT checked"
        return 2
    fi
    (timeout --foreground 30 valgrind --leak-check=full --show-leak-kinds=all \
        --errors-for-leak-kinds="$VG_LEAKS" --error-exitcode=42 --track-origins=yes \
        --log-file="$log" "${@:2}" > /dev/null 2>&1) 2> /dev/null
    rc=$?
    if [[ $rc -ne 0 ]] && grep -q "very large text, data or bss" "$log"; then
        rc=2 note="Valgrind cannot load it (huge static data): leaks NOT checked"
    elif [[ $rc -eq 0 ]] && grep -qE "(still reachable|possibly lost): [1-9]" "$log"; then
        rc=2 note="memory still held at exit: not a leak for Valgrind, worth freeing"
    elif [[ $rc -eq 124 ]]; then
        rc=1 note="timed out"
    elif [[ $rc -ne 0 && $rc -ne 42 ]]; then
        note="crashed: SIG$(kill -l $((rc - 128)) 2> /dev/null)" rc=1
    elif [[ $rc -eq 42 ]]; then
        rc=1
    fi
    [[ $rc -eq 1 && -n ${LENIENT:-} ]] && rc=2
    echo "$((rc != 1)) $((rc == 1)) $((rc == 2))" >> "$FAIRY_STATS"
    [[ $rc -eq 0 && $VERBOSE -eq 0 ]] && return 0
    section "valgrind · $1"
    [[ -n $note ]] && say "$note"
    # The report without its banner, pid prefixes and hints
    sed -E 's/^==[0-9]+== ?//' "$log" | awk '
        /^If you believe this happened/ {skip = 1} skip {skip = !/stack size used/; next}
        /^(Memcheck,|Copyright|Using Valgrind|Command:|Parent PID:)/ || /rerun with/ {next}
        /^$/ {if (!blank++ && shown) print; next} {blank = 0; shown = 1; print}' | quote
    return $rc
}

# lenient <cmd...>: runs tests or memcheck where a failure only warns
# (outside the subject, or debatable)
lenient() { LENIENT=1 "$@"; }

# queue <list> <name> <cmd...>: starts a run in the background, in a named
# list. run_queue <list> waits for the list, closes the step with the worst
# status and prints the reports in order.
queue() { local list=$1; shift; eval "Q_$list+=(\"\$1\")"; job "$@"; }
run_queue() {
    local -n runs=Q_$1
    local name worst=0 code
    await "${runs[@]}"
    for name in "${runs[@]}"; do
        code=$(rc "$name")
        if [[ $code -eq 2 ]]; then [[ $worst -eq 0 ]] && worst=2; elif [[ $code -ne 0 ]]; then worst=1; fi
    done
    case $worst in 0) ok ;; 2) warn ;; *) fail ;; esac
    for name in "${runs[@]}"; do cat "$TMP/$name.log"; done
}

# ═══════════════════════════════ end of core ══════════════════════════════════

NAME=libft.a
VG_LEAKS=all            # no static state in a libft: any memory left is a leak
clean_project() { make fclean > /dev/null 2>&1; }

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
FUNCTIONS=$(grep -oE 'ft_[a-z_]+\(' <<< "$PROTOTYPES" | tr -d '(')

echo; box "$PINK" "LIBFT-FAIRY" 🧚; echo
norm_start

# Makefile, libft.h and ft_*.c, all at the root, and no unused file
step 📂 Files
CMDS=$(make -nB 2> /dev/null)
issues=()
for f in Makefile libft.h; do [[ -f $f ]] || issues+=("missing at the root: $f"); done
for f in $(sources); do
    if [[ $f != libft.h && ! $f =~ ^ft_[^/]*\.c$ ]]; then
        issues+=("not in the turn-in list (Makefile, libft.h, ft_*.c at the root): $f")
    elif [[ $f == *.c ]] && ! grep -qw -- "$f" <<< "$CMDS"; then
        issues+=("not compiled by make, and unused files are forbidden: $f")
    fi
done
if [[ ${#issues[@]} -gt 0 ]]; then verdict "${issues[@]}"
elif ! report_untracked Makefile README.md libft.h $(sources | grep '\.c$'); then ok; fi
[[ -f Makefile && -f libft.h ]] || exit 1

check_readme
check_norm

# The compiler checks every prototype against the subject (names and spacing
# aside): a missing one is undeclared, a different one a conflicting type
step 📋 Prototypes
issues=()
grep -qw restrict libft.h && issues+=("the restrict qualifier is forbidden")
{
    echo '#include "libft.h"'
    echo 'void libft_fairy_uses(void) {'
    sed 's/^/(void)/; s/$/;/' <<< "$FUNCTIONS"
    echo '}'
    echo "$PROTOTYPES"
} > "$TMP/prototypes.c"
if ! cc -fsyntax-only -I. "$TMP/prototypes.c" 2> "$TMP/proto.err"; then
    for f in $(grep 'error:' "$TMP/proto.err" | grep -oE 'ft_[a-z_]+' | sort -u); do
        issues+=("expected: $(grep -E "\b$f\(" <<< "$PROTOTYPES")")
    done
    [[ ${#issues[@]} -eq 0 ]] && issues+=("libft.h does not compile: $(grep -m 1 'error' "$TMP/proto.err")")
fi
verdict "${issues[@]}"

# Rules and flags as make really runs them (make -n), variables expanded
step 🔧 Makefile
rule() { grep -qE "^([^#:=]+[[:space:]])?($1)([[:space:]][^#:=]*)?[[:space:]]*:([^=]|$)" Makefile; }
issues=()
rule '\$[({]NAME[)}]|libft\.a' || issues+=("no \$(NAME) rule")
for r in all clean fclean re; do rule "$r" || issues+=("no '$r' rule"); done
grep -vE '^\s*#' Makefile | grep -qE '\$\((wildcard|shell)|^[^#[:space:]][^#]*=[^#]*\*\.c' \
    && issues+=("sources listed with a wildcard: name each file")
COMPILES=$(grep -E '(^|\s)-c(\s|$)' <<< "$CMDS")
if [[ -z $COMPILES ]]; then
    issues+=("make -n shows no compilation")
else
    grep -vqE '(^|[;&|/[:space:]])cc\s' <<< "$COMPILES" && issues+=("sources not compiled with cc")
    for flag in -Wall -Wextra -Werror; do
        grep -vq -- "$flag" <<< "$COMPILES" && issues+=("sources compiled without $flag")
    done
    grep -q -- '-std=c99' <<< "$COMPILES" && issues+=("-std=c99 is forbidden")
fi
grep -qE '(^|[;&|/[:space:]])libtool\s' <<< "$CMDS" && issues+=("libtool is forbidden")
grep -qE '(^|[;&|/[:space:]])ar\s' <<< "$CMDS" || issues+=("$NAME not created with ar")
verdict "${issues[@]}"

# A build from scratch (in parallel; sequential if that fails, as some valid
# Makefiles do not support -j), then make again, which must not relink
step 📦 Build
make fclean > /dev/null 2>&1
if ! { make -j"$JOBS" > /dev/null 2>&1 && [[ -f $NAME ]]; } \
    && ! { make fclean > /dev/null 2>&1; make > "$TMP/make.log" 2>&1 && [[ -f $NAME ]]; }; then
    fail; tail -n 15 "$TMP/make.log" | quote
    [[ -f $NAME ]] || say "make did not create $NAME at the root"
    exit 1
fi
STAMP=$(stat -c %y "$NAME")
make > /dev/null 2>&1
if [[ $(stat -c %y "$NAME") == "$STAMP" ]]; then ok; else fail; say "relinks: a second 'make' rebuilds $NAME"; fi

# Every object of the archive, wherever it was built and whatever functions it
# holds: the external functions each ft_ function may call, no global
# variable, no helper left non-static, every function of the subject built
step 🔍 Symbols
ALLOWED="ft_calloc:malloc ft_strdup:malloc ft_substr:malloc ft_strjoin:malloc ft_strtrim:malloc
ft_split:malloc,free ft_itoa:malloc ft_strmapi:malloc ft_putchar_fd:write ft_putstr_fd:write
ft_putendl_fd:write ft_putnbr_fd:write ft_lstnew:malloc ft_lstdelone:free ft_lstclear:free ft_lstmap:malloc,free"
issues=() extras=()
while IFS='|' read -r kind obj sym; do
    [[ $kind == "forbidden function" ]] && emitted "$sym" && continue
    [[ $sym == __ctype* ]] && sym+=" (a <ctype.h> macro)"
    if [[ $kind == extra ]]; then extras+=("$obj: $sym is not in the subject: static, if it is a helper"); continue; fi
    issues+=("$obj: $kind: $sym")
done < <(nm -A "$NAME" 2> /dev/null | awk -v allowed="$ALLOWED" -v subject="$(xargs <<< "$FUNCTIONS")" '
    BEGIN {
        n = split(allowed, pairs); for (i = 1; i <= n; i++) { split(pairs[i], kv, ":"); can[kv[1]] = kv[2] }
        n = split(subject, f); for (i = 1; i <= n; i++) in_subject[f[i]] = 1
    }
    { split($1, path, ":"); obj = path[2]; type = $(NF - 1); sym = $NF }
    type == "T" && sym in in_subject { allowed_in[obj] = allowed_in[obj] "," can[sym]; next }
    type == "T" && sym ~ /^ft_/ { print "extra|" obj "|" sym; next }
    type == "T" { print "non-static helper|" obj "|" sym }
    type ~ /^[BCDGR]$/ { print "global variable|" obj "|" sym }
    type == "U" && sym !~ /^ft_/ { calls[obj] = calls[obj] " " sym }
    END {
        for (obj in calls) {
            n = split(calls[obj], c)
            for (i = 1; i <= n; i++)
                if (index(allowed_in[obj] ",", "," c[i] ",") == 0)
                    print "forbidden function|" obj "|" c[i]
        }
    }' | sort)
DEFINED=$(nm -g --defined-only "$NAME" 2> /dev/null | awk '{print $NF}')
for f in $FUNCTIONS; do
    grep -qx "$f" <<< "$DEFINED" || issues+=("$f is not in $NAME")
done
if [[ ${#issues[@]} -gt 0 ]]; then verdict "${issues[@]}"
elif [[ ${#extras[@]} -gt 0 ]]; then warn
else ok; fi
for e in "${extras[@]}"; do say "$e"; done

# The sources again, with -g, so that Valgrind reports point to their lines
# (libft.a if that fails), then every test file; all in parallel
step 🔨 "Test build"
mkdir "$TMP/obj"
for src in $(sources | grep '\.c$'); do job "g_$src" cc -g -I. -c "$src" -o "$TMP/obj/${src%.c}.o"; done
await $(sources | grep '\.c$' | sed 's/^/g_/')
LIB=("$TMP"/obj/*.o)
for src in $(sources | grep '\.c$'); do [[ $(rc "g_$src") -eq 0 ]] || LIB=(-L. -lft); done
TESTS="basic_tests basic_tests_lists leak_tests"
for t in $TESTS; do
    job "cc_$t" cc -g -Wall -Wextra -Wno-missing-field-initializers -DVERBOSE=$VERBOSE -I. -no-pie \
        "$TESTER/$t.c" "$TESTER/utils.c" "${LIB[@]}" -o "$TMP/$t" -Wl,--wrap=malloc
done
errors=""
for t in $TESTS; do
    await "cc_$t"
    [[ $(rc "cc_$t") -eq 0 ]] || errors+="$t.c:"$'\n'"$(head -n 12 "$TMP/cc_$t.log")"$'\n'
done
if [[ -z $errors ]]; then ok; else fail; printf '%s' "$errors" | quote; exit 1; fi

queue main basic tests "basic_tests" "$TMP/basic_tests"
queue main lists tests "basic_tests_lists" "$TMP/basic_tests_lists"
queue main leak memcheck "leak_tests" "$TMP/leak_tests"
queue main lstmap lenient memcheck "ft_lstmap: f's content on failure (warning)" "$TMP/leak_tests" lstmap

step 🧪 Tests
run_queue main
exit $FAILED
