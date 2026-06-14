# MiniV8

MiniV8 is a lightweight JavaScript-inspired interpreter written in C++. The project demonstrates the implementation of core compiler and interpreter concepts including lexical analysis, parsing, AST generation, and runtime execution.

## Features

### Supported Language Constructs

* Variables (`let`, `const`)
* Arithmetic Operators

  * `+`
  * `-`
  * `*`
  * `/`
  * `%`
  * `**`
* Comparison Operators

  * `==`
  * `===`
  * `!=`
  * `!==`
  * `<`
  * `<=`
  * `>`
  * `>=`
* Compound Assignment Operators

  * `+=`
  * `-=`
  * `*=`
  * `/=`
  * `%=`
* Unary Operators

  * `++`
  * `--`
* Conditional Statements

  * `if`
  * `else`
  * `else if`
* Loops

  * `while`
  * `for`
* Functions

  * Function Declaration
  * Parameters
  * Return Statements
* Built-in Functions

  * `console.log()`
  * `Math.floor()`
  * `Math.abs()`
  * `Math.pow()`
* Arrays

  * Array Literals
  * `join()`
  * `reverse()`
  * `split()`

---

## Building the Project

### Requirements

* CMake 3.20+
* Visual Studio 2022 (MSVC)
* C++17 Compiler

### Build Steps

Open PowerShell in the project root directory.

Configure the project:

```bash
cmake -B build
```

Build the project:

```bash
cmake --build build
```

The executable will be generated in:

```text
build/Debug/MiniV8.exe
```

---

## Running the Project

Execute:

```bash
.\build\Debug\MiniV8.exe
```

The current version contains hardcoded test cases inside `main.cpp`.

The output will display the results of all implemented test cases.

---

## Implemented Test Cases

### Test Case 1 – Odd or Even Checker

Input:

```javascript
let num = 7;

if (num % 2 === 0)
{
    console.log(num + " is Even");
}
else
{
    console.log(num + " is Odd");
}
```

Expected Output:

```text
7 is Odd
```

---

### Test Case 2 – Triangle Pattern

Input:

```javascript
for (let i = 1; i <= 5; i++)
{
    let row = "";

    for (let j = 1; j <= i; j++)
    {
        row += "*";
    }

    console.log(row);
}
```

Expected Output:

```text
*
**
***
****
*****
```

---

### Test Case 3 – Armstrong Number

Input:

```javascript
isArmstrong(153)
isArmstrong(123)
```

Expected Output:

```text
1
0
```

---

### Test Case 4 – Array Reverse

Input:

```javascript
let arr = [1,2,3,4,5];
```

Expected Output:

```text
Original: 1, 2, 3, 4, 5
Reversed: 5, 4, 3, 2, 1
```

---

### Test Case 5 – Palindrome Checker

Input:

```javascript
let str = "racecar";
```

Expected Output:

```text
racecar is a Palindrome
```

---

## Adding More Hardcoded Test Cases

Since the current implementation executes test cases directly from `main.cpp`, new test cases can be added by editing the `main()` function.

Example:

```cpp
std::cout << "===== TEST 6 =====" << std::endl;

int a = 10;
int b = 20;

std::cout
    << "Sum = "
    << a + b
    << std::endl;
```

Output:

```text
===== TEST 6 =====
Sum = 30
```

---

## Project Structure

```text
MiniV8/
│
├── ast/
│   ├── AST.h
│   └── AST.cpp
│
├── lexer/
│   ├── Lexer.h
│   └── Lexer.cpp
│
├── parser/
│   ├── Parser.h
│   └── Parser.cpp
│
├── runtime/
│   ├── Interpreter.h
│   ├── Interpreter.cpp
│   ├── Environment.h
│   └── JSValue.h
│
├── main.cpp
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
