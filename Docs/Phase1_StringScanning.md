# Phase 1.5 - String Literal Scanning

## Goal

The lexer now recognizes JavaScript string literals.

Examples:

```js
"hello"
"world"
"MiniV8"
```

---

## Scanning Process

When the lexer encounters:

```text
"
```

it begins collecting characters until the next matching quote is found.

Example:

```js
"hello"
```

Produces:

```text
STRING(hello)
```

---

## Current Support

Supported:

```js
"hello"
"world"
"sample text"
```

Not yet supported:

```js
"hello\nworld"
"quote: \""
```

Escape sequences will be added later.

---

## Benefit

Strings are required for:

* console.log()
* Text processing
* Comparisons
* Array operations
* Function arguments

This capability enables execution of many beginner JavaScript programs.