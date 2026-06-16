# MiniV8

MiniV8 is a lightweight JavaScript-inspired interpreter written in C++. The project demonstrates the implementation of core compiler and interpreter concepts including lexical analysis, parsing, AST generation, and runtime execution.

## Features

### Supported Language Constructs

* Variables (`let`, `const`)
* Data Types: Numbers, Strings (double and single quotes), Booleans, `null`, `undefined`, Arrays, Objects
* Arithmetic Operators: `+`, `-`, `*`, `/`, `%`, `**`
* Comparison Operators: `==`, `===`, `!=`, `!==`, `<`, `<=`, `>`, `>=`
* Logical Operators: `&&`, `||`, `!`
* Compound Assignment: `+=`, `-=`, `*=`, `/=`, `%=`
* Unary Operators: `++`, `--`, `-` (negation), `typeof`
* Ternary Operator: `? :`
* Conditional Statements: `if`, `else`, `else if`
* Loops: `for`, `while`, `do...while`, `break`, `continue`
* Functions
  * Function Declaration
  * Function Expressions
  * Arrow Functions (`x => expr`, `(x, y) => expr`, `(x) => { ... }`)
  * Callbacks / Higher-Order Functions
* Built-in Functions
  * `console.log()` (supports multiple arguments)
  * `Math.floor()`, `Math.ceil()`, `Math.round()`
  * `Math.abs()`, `Math.sqrt()`, `Math.pow()`
  * `Math.min()`, `Math.max()`, `Math.random()`
  * `Math.PI`, `Math.E`
  * `Date.now()`
  * `Object.keys()`, `Object.values()`
* Arrays
  * Array Literals
  * `length`, `push()`, `pop()`, `shift()`, `unshift()`
  * `indexOf()`, `includes()`, `find()`
  * `join()`, `reverse()`, `slice()`, `concat()`
  * `map()`, `filter()`, `reduce()`, `forEach()`
  * Method chaining: `arr.filter(x => x > 2).map(x => x * 10)`
* Strings
  * `length`, `split()`, `charAt()`
  * `toUpperCase()`, `toLowerCase()`
  * `indexOf()`, `includes()`
  * `trim()`, `substring()`, `slice()`
* Objects
  * Object Literals: `{ key: value }`
  * Property access: `obj.key`
  * Property assignment: `obj.key = value`
  * `Object.keys()`, `Object.values()`
* Comments: `//` single-line, `/* */` multi-line

---

## Building the Project

### Requirements

* CMake 3.15+
* A C++17 compatible compiler (see platform-specific instructions below)
* C++17 Standard

See [requirements.txt](requirements.txt) for the full list of build dependencies.

---

### Windows

#### Prerequisites

1. Install [CMake](https://cmake.org/download/) (3.15 or later). During installation, select **"Add CMake to the system PATH"**.
2. Install [Visual Studio 2022](https://visualstudio.microsoft.com/) with the **"Desktop development with C++"** workload (this includes MSVC and the Windows SDK).

#### Build Steps

Open **PowerShell** or **Developer Command Prompt** in the project root directory.

Configure the project:

```powershell
cmake -B build
```

Build the project:

```powershell
cmake --build build
```

The executable will be generated at:

```text
build\Debug\MiniV8.exe
```

---

### macOS

#### Prerequisites

1. Install the **Xcode Command Line Tools** (provides Apple Clang and Make):
   ```bash
   xcode-select --install
   ```
2. Install **CMake** via [Homebrew](https://brew.sh/):
   ```bash
   brew install cmake
   ```

#### Build Steps

Open **Terminal** in the project root directory.

Configure the project:

```bash
cmake -B build
```

Build the project:

```bash
cmake --build build
```

The executable will be generated at:

```text
build/MiniV8
```

---

### Linux

#### Prerequisites

1. Install **CMake**, **GCC** (or **Clang**), and **Make** using your distribution's package manager:

   **Ubuntu / Debian:**
   ```bash
   sudo apt update
   sudo apt install cmake g++ make
   ```

   **Fedora:**
   ```bash
   sudo dnf install cmake gcc-c++ make
   ```

   **Arch Linux:**
   ```bash
   sudo pacman -S cmake gcc make
   ```

#### Build Steps

Open a **terminal** in the project root directory.

Configure the project:

```bash
cmake -B build
```

Build the project:

```bash
cmake --build build
```

The executable will be generated at:

```text
build/MiniV8
```

---

## Running the Project

### Windows

Run a single JavaScript file:

```powershell
.\build\Debug\MiniV8.exe path/to/file.js
```

Run all test files in a directory:

```powershell
.\build\Debug\MiniV8.exe --test-dir tests
```

Show usage help:

```powershell
.\build\Debug\MiniV8.exe
```

### macOS / Linux

Run a single JavaScript file:

```bash
./build/MiniV8 path/to/file.js
```

Run all test files in a directory:

```bash
./build/MiniV8 --test-dir tests
```

Show usage help:

```bash
./build/MiniV8
```

---

## Test Suite

The project includes 13 test files in the `tests/` directory:

| Test | File | Features Tested |
|------|------|----------------|
| 1 | `01_odd_even.js` | if/else, modulo, string concatenation |
| 2 | `02_triangle_pattern.js` | Nested for loops, string concatenation |
| 3 | `03_armstrong.js` | Functions, while loop, Math.floor, exponentiation |
| 4 | `04_array_reverse.js` | Arrays, reverse, join |
| 5 | `05_palindrome.js` | String split/reverse/join chaining |
| 6 | `06_data_types.js` | Numbers, strings, booleans, null, undefined, typeof |
| 7 | `07_operators.js` | Arithmetic, comparison, logical, ternary, compound assignment |
| 8 | `08_math_functions.js` | Math.floor/ceil/round/abs/sqrt/pow/min/max/PI/E |
| 9 | `09_loops.js` | for, while, do-while, break, continue |
| 10 | `10_array_methods.js` | push, pop, indexOf, includes, slice, concat, reverse, find |
| 11 | `11_objects.js` | Object literals, property access/mutation, Object.keys/values |
| 12 | `12_date.js` | Date.now() |
| 13 | `13_callbacks.js` | map, filter, reduce, forEach, arrow functions, chaining |

---

## Adding More Test Cases

Create a new `.js` file in the `tests/` directory:

```javascript
let greeting = "Hello";
let name = "World";

console.log(greeting + " " + name);
```

Run it:

**Windows:**
```powershell
.\build\Debug\MiniV8.exe tests/my_test.js
```

**macOS / Linux:**
```bash
./build/MiniV8 tests/my_test.js
```

Or run all tests at once:

**Windows:**
```powershell
.\build\Debug\MiniV8.exe --test-dir tests
```

**macOS / Linux:**
```bash
./build/MiniV8 --test-dir tests
```

---

## Project Structure

```text
MiniV8/
│
├── ast/
│   ├── AST.h           # AST node definitions
│   ├── AST.cpp
│   ├── ASTPrinter.h    # AST debug printer
│   └── ASTPrinter.cpp
│
├── lexer/
│   ├── Lexer.h         # Token types and lexer class
│   ├── Lexer.cpp       # Lexical analysis implementation
│   └── TokenUtils.h    # Token type name utilities
│
├── parser/
│   ├── Parser.h        # Parser class declaration
│   └── Parser.cpp      # Recursive descent parser
│
├── runtime/
│   ├── Interpreter.h   # Interpreter class declaration
│   ├── Interpreter.cpp # Runtime evaluation engine
│   ├── Environment.h   # Variable scope management
│   └── JSValue.h       # JavaScript value types
│
├── tests/              # JavaScript test files
│   ├── 01_odd_even.js
│   ├── ...
│   └── 13_callbacks.js
│
├── main.cpp            # CLI entry point
│
├── CMakeLists.txt
│
└── README.md
```

---

## Authors

Kaustubh Bharti

VIT Chennai

Integrated M. Tech
