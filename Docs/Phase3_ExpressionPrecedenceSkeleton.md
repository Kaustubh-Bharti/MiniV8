# Phase 3.7 - Expression Precedence Skeleton

## Goal

Prepare the parser for operator precedence.

Hierarchy:

parseExpression()
↓
parseEquality()
↓
parseComparison()
↓
parseTerm()
↓
parseFactor()
↓
parseUnary()
↓
parsePrimary()

## Benefit

This structure allows correct parsing of:

```js
5 + 3 * 2