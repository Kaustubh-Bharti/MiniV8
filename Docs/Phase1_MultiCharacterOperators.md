# Phase 1.6 - Multi-Character Operator Recognition

## Goal

JavaScript contains operators consisting of multiple characters.

Examples:

```js
===
!==
<=
>=
++
--
+=
-=
&&
||
```

---

## Matching Strategy

The lexer uses look-ahead matching.

Example:

```js
===
```

The lexer reads:

```text
=
=
=
```

and combines them into a single token:

```text
StrictEqual
```

rather than three separate Assign tokens.

---

## Supported Operators

Comparison:

```text
==
===
!=
!==
<
<=
>
>=
```

Logical:

```text
&&
||
!
```

Assignment:

```text
=
+=
-=
```

Unary:

```text
++
--
```

---

## Benefit

These operators are essential for:

* Conditions
* Loops
* Equality checks
* Arithmetic updates
* Logical expressions

This phase enables tokenization of most beginner and intermediate JavaScript programs.