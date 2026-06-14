# Phase 1.4 - Number Scanning

## Goal

The lexer now recognizes integer literals.

Examples:

```js
123
456
789
```

---

## Number Recognition

When the lexer encounters a digit:

```text
0-9
```

it consumes all consecutive digits and creates a Number token.

Example:

```js
12345
```

Produces:

```text
NUMBER(12345)
```

---

## Current Support

Supported:

```js
0
15
999
123456
```

Not yet supported:

```js
3.14
0.5
1e10
```

These will be added in later phases.

---

## Benefit

Numeric literals are required for:

* Arithmetic expressions
* Conditions
* Loops
* Function arguments
* Variable initialization

This is one of the core building blocks of JavaScript execution.