# Phase 2 - AST Design

## Goal

Transform token streams into structured program representations.

The lexer converts source code into tokens.

The parser will convert tokens into an Abstract Syntax Tree (AST).

---

## Example

Source:

```js
let x = 5 + 3;
```

Tokens:

```text
LET
IDENTIFIER(x)
ASSIGN
NUMBER(5)
PLUS
NUMBER(3)
SEMICOLON
```

AST:

```text
VariableDeclaration
|
+- name = x
|
+- BinaryExpression (+)
   |
   +- NumberLiteral(5)
   |
   +- NumberLiteral(3)
```

---

## Planned Expression Nodes

- NumberLiteral
- StringLiteral
- BooleanLiteral
- Identifier
- BinaryExpression
- UnaryExpression
- AssignmentExpression
- CallExpression
- MemberExpression
- ArrayExpression
- ObjectExpression

---

## Planned Statement Nodes

- ExpressionStatement
- VariableDeclaration
- BlockStatement
- IfStatement
- WhileStatement
- ForStatement
- FunctionDeclaration
- ReturnStatement

---

## Goal of Phase 2

Build a complete AST capable of representing the JavaScript subset required by MiniV8.