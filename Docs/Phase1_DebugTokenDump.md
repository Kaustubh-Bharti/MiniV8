# Phase 1.7 - Token Stream Visualization

## Goal

A debugging utility was added to display generated tokens.

This allows inspection of lexer output before parser development begins.

---

## Example

Input:

```js
let num = 7;
```

Output:

```text
Let
Identifier(num)
Assign
Number(7)
Semicolon
EOF
```

---

## Benefits

* Easier debugging
* Faster feature verification
* Parser development becomes significantly simpler
* Tokenization errors become immediately visible

This utility will be used extensively throughout future development.