# Phase 1.8 - Decimal Number Support

## Goal

Extend numeric literal recognition to support floating-point values.

Examples:

```js
3.14
0.5
10.25
```

---

## Previous Behavior

Input:

```js
3.14
```

Produced:

```text
NUMBER(3)
DOT
NUMBER(14)
```

---

## New Behavior

Input:

```js
3.14
```

Produces:

```text
NUMBER(3.14)
```

---

## Supported Examples

```js
0.5
3.14
10.25
100.0
```

---

## Benefit

Decimal support is required for:

* Mathematical calculations
* Financial values
* Scientific values
* General JavaScript number handling

This brings MiniV8 closer to real JavaScript numeric behavior.