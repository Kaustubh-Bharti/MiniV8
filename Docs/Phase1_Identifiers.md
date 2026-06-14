# Phase 1.3 - Identifier and Keyword Recognition

## Goal

The lexer now recognizes JavaScript identifiers and reserved keywords.

Examples:

```js
let
const
function
if
else
return
```

as well as user-defined names:

```js
x
count
userName
myVariable
```

---

## Identifier Rules

Valid identifiers may contain:

* Letters
* Digits (not as first character)
* Underscore (_)

Examples:

```js
userName
_count
value123
```

---

## Keyword Recognition

The lexer distinguishes between ordinary identifiers and JavaScript keywords.

Example:

```js
let x;
```

Produces:

```text
LET
IDENTIFIER(x)
SEMICOLON
EOF
```

rather than:

```text
IDENTIFIER(let)
IDENTIFIER(x)
```

---

## Benefit

Keyword recognition is required before parsing statements such as:

* Variable declarations
* Conditionals
* Loops
* Functions
* Returns

This is the first step toward understanding JavaScript program structure.