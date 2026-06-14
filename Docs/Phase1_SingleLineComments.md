# Phase 1.9 - Single-Line Comment Support

## Goal

The lexer now ignores JavaScript single-line comments.

Example:

```js
// comment
let x = 5;
```

---

## Behavior

All characters following:

```text
//
```

until the next newline are skipped.

Input:

```js
// test
let x = 5;
```

Produces:

```text
LET
IDENTIFIER(x)
ASSIGN
NUMBER(5)
SEMICOLON
EOF
```

---

## Benefit

Comments are common in real JavaScript programs and should not affect execution.

This improves compatibility with user-written code and hidden test cases.