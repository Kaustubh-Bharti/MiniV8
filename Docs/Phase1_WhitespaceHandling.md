# Phase 1.2 - Whitespace Handling

## Goal

JavaScript source code frequently contains spaces, tabs, and line breaks for readability.

These characters do not usually affect program behavior and should not generate tokens.

---

## Ignored Characters

The lexer now skips:

```text
Space
Tab (\t)
Carriage Return (\r)
Newline (\n)
```

---

## Example

Input:

```js
let

x     =     5;
```

is treated identically to:

```js
let x = 5;
```

The lexer consumes whitespace and continues scanning meaningful tokens.

---

## Benefit

Ignoring whitespace simplifies parsing and ensures formatting differences do not affect execution.