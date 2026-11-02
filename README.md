# 🧚 libft-fairy

A comprehensive tester for 42's **libft** project (subject v19.3, where the linked list functions are part of the mandatory part), featuring memory leak detection, edge case testing, and rigorous validation.

## ✨ Features

- ✅ Complete tests for all functions (Part 1, 2 and 3)
- 🔍 Automatic verification with **norminette**
- 📖 README.md requirements check (italic first line, Description / Instructions / Resources sections)
- 📋 Prototype validation in libft.h, done by the compiler
- ⚙️ Makefile rules verification (NAME, all, clean, fclean, re) and relink detection
- 🔧 Compiler and flags check (cc -Wall -Wextra -Werror) on the commands make really runs
- 🔎 External functions, global variables and non-static helpers detection
- 💧 Memory leak detection with **Valgrind**
- 📏 Allocation size check (catches the classic missing `+ 1` for the `'\0'`)
- 🚨 Edge case testing (NULL, overflow, boundary cases)
- 🧪 Malloc failure simulation for robustness
- ⏱️ Per-test timeout, so an infinite loop only fails the test that hangs
- 📊 Clear and colorful output, with the reason of each failure
- 🔇 Optional verbose mode for detailed results

## 📋 Prerequisites

Before using libft-fairy, make sure you have installed:

- `gcc`
- `make`
- `valgrind`
- `norminette`
- `bash`
- `nm` (usually pre-installed)

## 🚀 Installation

1. Clone the repository inside your libft directory:

```bash
git clone https://github.com/sys-harmony/libft-fairy.git
```

2. Navigate to the tester folder:

```bash
cd libft-fairy
```

3. Make the script executable:

```bash
chmod +x run.sh
```

## 🎯 Usage

### Run all tests

```bash
./run.sh
```

### Verbose mode (displays all tests, even passing ones)

```bash
./run.sh --verbose
# or
./run.sh -v
```

## 📊 Output

The tester displays a comprehensive validation process:

```
╔═══════════════════════════╗
║       LIBFT-FAIRY 🧚      ║
╚═══════════════════════════╝

📝 Checking norm...         Done
📖 Checking README...        Done
📑 Checking prototypes...    Done
🔧 Checking Makefile...      Done
📦 Building libft...         Done
🔍 Checking symbols...       Done
🔨 Building tests...         Done
🧪 Running tests...          Done

========================================
ft_isalpha
========================================
[OK]  Test EOF to 255
...

╔═══════════════════════════╗
║     OH MY, YOU PASSED!    ║
╚═══════════════════════════╝
```

On failure, details of failed tests will be displayed with their reason, along with Valgrind logs if memory leaks are detected:

```
========================================
ft_split
========================================
[OK]  Test basic split
...
[KO]  Test malloc fail #2 (word 1)  (Segmentation fault)
```

## 🧪 Tests Covered

### Pre-compilation Checks

- **Norminette**: Validates the .c and .h files at the root of the repository
- **README**: Checks the requirements of the subject (first line, required sections)
- **Prototypes**: The compiler checks every prototype of libft.h against the subject (parameter names and spacing don't matter); `restrict` is forbidden
- **Makefile**: Checks the required rules, the compiler and flags of the commands `make -n` prints, `-std=c99`, and relinks (`make` and `make bonus` run twice)
- **Symbols**: In every object of libft.a, ensures only allowed external functions are used, and that there is no global variable nor non-static helper

### Part 1 & 2 Functions

#### Character Classification
- `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint`: Compared to the libc for every value from EOF to 255, must return exactly 1 on a match (subject rule)
- `ft_toupper`/`ft_tolower`: Compared to the libc for every value from EOF to 255

#### String Manipulation
- `ft_strlen`: Length calculation including NULL handling
- `ft_strchr`/`ft_strrchr`: Character search with unsigned char support
- `ft_strncmp`: Comparison with SIZE_MAX edge case
- `ft_strnstr`: Substring search with boundary tests
- `ft_strlcpy`/`ft_strlcat`: Safe string operations with size limits

#### Memory Operations
- `ft_memset`: Pattern filling with return value check
- `ft_bzero`: Zero-filling validation
- `ft_memcpy`: Copy operations with NULL detection
- `ft_memmove`: Overlap handling (forward/backward)
- `ft_memchr`: Binary-safe search
- `ft_memcmp`: Unsigned byte comparison

#### Conversion
- `ft_atoi`: Integer parsing with whitespace and sign handling

#### Allocation Functions
- `ft_calloc`: Zero-initialization with overflow protection, freeable pointer when nmemb or size is 0
- `ft_strdup`: String duplication with empty string handling

#### String Creation
- `ft_substr`: Substring extraction with edge cases
- `ft_strjoin`: String concatenation with NULL handling
- `ft_strtrim`: Trimming with multiple character sets
- `ft_split`: Word splitting with malloc failure simulation
- `ft_itoa`: Integer to string (including INT_MIN/MAX)

#### Iteration
- `ft_strmapi`: Mapping with index parameter
- `ft_striteri`: In-place iteration

#### File Descriptors
- `ft_putchar_fd`: Character output
- `ft_putstr_fd`: String output with NULL handling
- `ft_putendl_fd`: String with newline
- `ft_putnbr_fd`: Number output (INT_MIN/MAX)

### Part 3 Functions

#### List Operations
- `ft_lstnew`: Node creation with NULL content
- `ft_lstadd_front`/`ft_lstadd_back`: List insertion with NULL checks
- `ft_lstsize`: Size calculation
- `ft_lstlast`: Last node retrieval
- `ft_lstdelone`: Single node deletion with custom del function
- `ft_lstclear`: Complete list cleanup
- `ft_lstiter`: Iteration with function application
- `ft_lstmap`: Mapping with malloc failure handling

### Edge Cases Tested

- ❌ NULL pointers for all applicable functions
- 🔢 Boundary values (INT_MIN, INT_MAX, UINT_MAX, SIZE_MAX)
- 🔤 Non-ASCII characters (127, 128, 200, 255, -1)
- 📏 Empty strings and zero sizes
- 🔄 Memory overlaps (ft_memmove forward/backward)
- 💥 Simulated malloc failures (multiple scenarios per function)
- 🧩 Consecutive delimiters in ft_split
- 🎯 Start index beyond string length (ft_substr)
- 📊 Zero count/size in ft_calloc
- 🔗 List operations with NULL parameters

### Memory Leak Tests

Dedicated leak tests for:
- All allocation functions (ft_calloc, ft_strdup, ft_substr, ft_strjoin, ft_strtrim, ft_split, ft_itoa, ft_strmapi)
- List functions (ft_lstnew, ft_lstdelone, ft_lstclear)
- ft_lstmap with three content scenarios:
  - Static content
  - Shared content
  - Dynamic content – commented out by default (not tested by Moulinette)
- Malloc failure cleanup validation

## 📁 Project Structure

```
libft-fairy/
├── run.sh                    # Main test orchestrator
├── basic_tests.c             # Part 1 & 2 function tests
├── basic_tests_bonus.c       # Part 3 (linked list) function tests
├── leak_tests.c              # Leak tests (Part 1 & 2)
├── leak_tests_bonus.c        # Leak tests (Part 3)
├── utils.c                   # Utility functions and malloc wrapper
├── libft_fairy.h             # Header file
└── README.md                 # This file
```

## 🔍 Advanced Features

### Malloc Failure Simulation

The tester uses a custom malloc wrapper (`__wrap_malloc`) to simulate allocation failures at specific points, ensuring your functions return NULL on malloc errors and don't leak memory on failure. The wrapper also records the size of each allocation, so a returned string must fit in the memory allocated for it.

### Forked Tests

Every test runs in a separate process with a timeout, so a crash or an infinite loop only fails that test. A failed test shows why: `wrong result`, the signal that killed it (`Segmentation fault`...), `timeout`, or `did not crash` for tests expecting a crash.

### Symbol Validation

Every object of libft.a is checked with `nm`, wherever the .o files are built and whatever functions each file holds:
- Only authorized external functions: `malloc` (allocating functions), `free` (`ft_split`, `ft_lstdelone`, `ft_lstclear`, `ft_lstmap`), `write` (`ft_put*_fd`), none for the others
- No global variable, no non-static helper function
- Every function of the subject is defined

## 🐛 Debugging

If a test fails:

1. **Verbose mode**: Run `./run.sh -v` to see all test details
2. **Valgrind logs**: Complete output displayed for memory issues
3. **Norminette**: Specific errors shown with file and line
4. **Prototype errors**: Exact expected signatures displayed

## ⚠️ Limitations

- Tested only on Linux (uses `fork`, `nm`, and Valgrind)
- Requires specific directory structure (tester in libft subfolder)
- May produce false positives if non-standard implementation approaches are used

## 🤝 Contributing

Contributions are welcome! Feel free to:

- Open an issue to report a bug
- Suggest new edge cases
- Improve test coverage
- Enhance documentation

## 📜 License

This project is free to use for educational purposes.

## 💖 Credits

Created with ✨ by **sys-harmony**.

Special thanks to the 42 community for feedback and suggestions.

---

*If libft-fairy helped you validate your libft, don't forget to leave a ⭐ on the repo!*
