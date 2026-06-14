# Phase 1 - Lexer Foundations

## Goal

The lexer is responsible for converting raw JavaScript source code into a stream of tokens that can later be parsed into an Abstract Syntax Tree.

Example:

JavaScript:

```js
let x = 5;
```

Token Stream:

```text
LET
IDENTIFIER(x)
ASSIGN
NUMBER(5)
SEMICOLON
EOF
```

---

## Initial Lexer Infrastructure

The following helper functions were implemented:

### isAtEnd()

Determines whether the lexer has reached the end of the source code.

### peek()

Returns the current character without consuming it.

### advance()

Consumes the current character and moves to the next position.

---

## Example

For source code:

```js
let x = 5;
```

The lexer processes characters sequentially:

```text
l
e
t
space
x
space
=
space
5
;
```

These functions form the foundation for all future tokenization logic.