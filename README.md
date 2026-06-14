# MiniV8

A JavaScript runtime written in C++ inspired by Google's V8 engine.

## Features Implemented

### Lexer
- Keywords
- Identifiers
- Numbers
- Strings
- Single-character symbols
- Multi-character operators
- Token stream visualization

## Architecture

JavaScript Source
↓
Lexer
↓
Parser
↓
AST
↓
Interpreter
↓
Runtime

## Build

```bash
cmake -S . -B build
cmake --build build
```