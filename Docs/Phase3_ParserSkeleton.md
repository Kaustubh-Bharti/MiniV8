# Phase 3.1 - Parser Skeleton

## Goal

Create the foundation for token consumption.

The parser maintains:

- Token list
- Current position
- Navigation utilities

## Implemented Functions

### peek()

View current token.

### previous()

View previously consumed token.

### advance()

Consume next token.

### check()

Check token type without consuming.

### match()

Check and consume token.

## Purpose

These utilities will be used to construct AST nodes from token streams.