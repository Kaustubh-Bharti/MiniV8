# MiniV8 - Project Setup

## Overview

MiniV8 is a custom JavaScript runtime written entirely in C++.

The goal of this project is to build a simplified JavaScript engine inspired by Google's V8 engine. Rather than executing JavaScript through an existing runtime such as Node.js, MiniV8 will tokenize, parse, interpret, and execute JavaScript code using a custom implementation.

This project was created as part of Thunder Hackathon 2.0.

---

## Architecture

The runtime follows a traditional interpreter pipeline:

JavaScript Source Code

↓

Lexer (Tokenizer)

↓

Parser

↓

Abstract Syntax Tree (AST)

↓

Interpreter

↓

Runtime Environment

↓

Program Output

---

## Project Structure

```text
MiniV8/
│
├── main.cpp
│
├── lexer/
│   ├── Lexer.h
│   └── Lexer.cpp
│
├── parser/
│   ├── Parser.h
│   └── Parser.cpp
│
├── ast/
│   ├── AST.h
│   └── AST.cpp
│
├── runtime/
│   ├── Interpreter.h
│   ├── Interpreter.cpp
│   ├── Environment.h
│   └── JSValue.h
│
├── builtins/
│   ├── Console.h
│   ├── Math.h
│   ├── Array.h
│   └── String.h
│
└── CMakeLists.txt
```

---

## Technologies

* C++17
* MinGW g++ 11.2
* CMake 4.x
* Visual Studio Code

---

## Development Strategy

The project is developed incrementally.

1. Project Setup
2. Lexer
3. Parser
4. AST Construction
5. Runtime Values
6. Scope Management
7. Interpreter
8. Built-in Objects
9. Testing
10. Optimization

Each phase is implemented and tested before moving to the next phase.