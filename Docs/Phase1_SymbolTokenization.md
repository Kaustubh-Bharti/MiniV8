# Phase 1.1 - Symbol Tokenization

## Goal

The lexer now recognizes single-character JavaScript symbols and operators.

Supported symbols:

```text
(
)
{
}
[
]
;
,
.
+
-
*
/
%
```

---

## Process

The lexer reads one character at a time.

For each character:

1. Consume using `advance()`
2. Match using a `switch`
3. Create the appropriate token
4. Store it in the token list

Example:

```js
(){}
```

Produces:

```text
LeftParen
RightParen
LeftBrace
RightBrace
EndOfFile
```

---

## Current Lexer Capabilities

* Character navigation
* End-of-file detection
* Single-character symbol tokenization
* Token generation
* EOF token generation

The lexer is now producing real token streams rather than returning an empty list.