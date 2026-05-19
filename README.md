# 🧚 libft-fairy

A comprehensive tester for 42's **libft** project, featuring memory leak detection, edge case testing, and rigorous validation.

## ✨ Features

- ✅ Complete tests for all functions (mandatory + bonus)
- 🔍 Automatic verification with **norminette**
- 📋 Prototype validation in libft.h
- ⚙️ Makefile rules verification (NAME, all, clean, fclean, re)
- 🔧 Compilation flags check (-Wall -Wextra -Werror)
- 🔎 External function usage verification
- 💧 Memory leak detection with **Valgrind**
- 🚨 Edge case testing (NULL, overflow, boundary cases)
- 🧪 Malloc failure simulation for robustness
- 📊 Clear and colorful output
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
git clone https://github.com/gcxd68/libft-fairy.git
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
🏷️  Checking version...      Mandatory
📋 Checking prototypes...    Done
⚙️  Checking Makefile...     Done
📦 Building libft...         Done
🔍 Checking externals...     Done
🔨 Building tests...         Done
🧪 Running tests...          Done

========================================
ft_isalpha
========================================
[OK]  Test EOF to '?'
[OK]  Test 'A' to 'Z'
[OK]  Test '[' to '`'
[OK]  Test 'a' to 'z'
[OK]  Test '{' to 255
...

╔═══════════════════════════╗
║     OH MY, YOU PASSED!    ║
╚═══════════════════════════╝
```

On failure, details of failed tests will be displayed, along with Valgrind logs if memory leaks are detected.

## 🧪 Tests Covered

### Pre-compilation Checks

- **Norminette**: Validates all .c and .h files
- **Prototypes**: Verifies exact function signatures in libft.h
- **Makefile**: Checks for required rules and proper compilation flags
- **External functions**: Ensures only allowed functions are used per function

### Mandatory Functions

#### Character Classification
- `ft_isalpha`: Tests all ASCII ranges (EOF to 255)
- `ft_isdigit`: Validates digit detection
- `ft_isalnum`: Combined alphanumeric tests
- `ft_isascii`: ASCII range validation
- `ft_isprint`: Printable character detection

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
- `ft_toupper`/`ft_tolower`: Case conversion for all ASCII

#### Allocation Functions
- `ft_calloc`: Zero-initialization with overflow protection
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

### Bonus Functions

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
- Bonus list functions (ft_lstnew, ft_lstdelone, ft_lstclear)
- ft_lstmap with three content scenarios:
  - Static content
  - Shared content
  - Dynamic content – commented out by default (not tested by Moulinette)
- Malloc failure cleanup validation

## 📁 Project Structure

```
libft-fairy/
├── run.sh                    # Main test orchestrator
├── basic_tests.c             # Mandatory function tests
├── basic_tests_bonus.c       # Bonus function tests
├── leak_tests.c              # Leak tests (mandatory)
├── leak_tests_bonus.c        # Leak tests (bonus)
├── utils.c                   # Utility functions and malloc wrapper
├── libft_fairy.h             # Header file
└── README.md                 # This file
```

## 🔍 Advanced Features

### Malloc Failure Simulation

The tester uses a custom malloc wrapper (`__wrap_malloc`) to simulate allocation failures at specific points, ensuring your functions properly handle malloc errors and don't leak memory on failure.

### Forked Tests

Crash-prone tests (NULL dereferences, segfaults) run in separate processes to prevent tester crashes, allowing comprehensive validation without interruption.

### External Function Validation

Each function is checked with `nm -u` to ensure only authorized external functions are used:
- Functions with no external calls allowed
- Functions allowed to use only `malloc`
- Functions allowed to use only `write`
- `ft_split` allowed to use `malloc` and `free`

## 🐛 Debugging

If a test fails:

1. **Verbose mode**: Run `./run.sh -v` to see all test details
2. **Valgrind logs**: Complete output displayed for memory issues
3. **Norminette**: Specific errors shown with file and line
4. **Results file**: Check `.results` for detailed output
5. **Prototype errors**: Exact expected signatures displayed

## ⚠️ Limitations

- Tested only on Linux (uses `fork`, `nm`, and Valgrind)
- Requires specific directory structure (tester in libft subfolder)
- Assumes standard prototype format in libft.h
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
