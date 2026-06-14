#pragma once

#include <memory>
#include <string>
#include <vector>

class Statement;
class Expression;

class BlockStatement;

class FunctionDeclaration;
class Program;
class CallExpression;
class ExpressionStatement;
class ReturnStatement;

class Node
{
public:
    virtual ~Node() = default;
};

class Statement : public Node
{
};

class Expression : public Node
{
};

// ===== Literals =====

class NumberLiteral : public Expression
{
public:
    double value;

    explicit NumberLiteral(double value)
        : value(value)
    {
    }
};

class StringLiteral : public Expression
{
public:
    std::string value;

    explicit StringLiteral(const std::string& value)
        : value(value)
    {
    }
};

class Identifier : public Expression
{
public:
    std::string name;

    explicit Identifier(const std::string& name)
        : name(name)
    {
    }
};

class BooleanLiteral : public Expression
{
public:
    bool value;

    explicit BooleanLiteral(bool value)
        : value(value)
    {
    }
};

class NullLiteral : public Expression
{
};

class UndefinedLiteral : public Expression
{
};

class ArrayLiteral : public Expression
{
public:
    std::vector<
        std::unique_ptr<Expression>
    > elements;
};

class ObjectLiteral : public Expression
{
public:
    // Parallel vectors for keys and values
    std::vector<std::string> keys;
    std::vector<
        std::unique_ptr<Expression>
    > values;
};

// ===== Operators =====

class BinaryExpression : public Expression
{
public:
    std::unique_ptr<Expression> left;
    std::string op;
    std::unique_ptr<Expression> right;

    BinaryExpression(
        std::unique_ptr<Expression> left,
        const std::string& op,
        std::unique_ptr<Expression> right)
        : left(std::move(left)),
          op(op),
          right(std::move(right))
    {
    }
};

class UnaryExpression : public Expression
{
public:
    std::string op;
    std::unique_ptr<Expression> operand;
    bool isPrefix;

    UnaryExpression(
        const std::string& op,
        std::unique_ptr<Expression> operand,
        bool isPrefix = true)
        : op(op),
          operand(std::move(operand)),
          isPrefix(isPrefix)
    {
    }
};

class TernaryExpression : public Expression
{
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Expression> consequent;
    std::unique_ptr<Expression> alternate;

    TernaryExpression(
        std::unique_ptr<Expression> condition,
        std::unique_ptr<Expression> consequent,
        std::unique_ptr<Expression> alternate)
        : condition(std::move(condition)),
          consequent(std::move(consequent)),
          alternate(std::move(alternate))
    {
    }
};

class AssignmentExpression : public Expression
{
public:
    std::string variableName;
    std::unique_ptr<Expression> value;

    AssignmentExpression(
        const std::string& variableName,
        std::unique_ptr<Expression> value)
        : variableName(variableName),
          value(std::move(value))
    {
    }
};

// ===== Member / Index Access =====

class MemberExpression : public Expression
{
public:
    std::unique_ptr<Expression> object;
    std::string property;

    MemberExpression(
        std::unique_ptr<Expression> object,
        const std::string& property)
        : object(std::move(object)),
          property(property)
    {
    }
};

class IndexExpression : public Expression
{
public:
    std::unique_ptr<Expression> object;
    std::unique_ptr<Expression> index;

    IndexExpression(
        std::unique_ptr<Expression> object,
        std::unique_ptr<Expression> index)
        : object(std::move(object)),
          index(std::move(index))
    {
    }
};

// ===== Member Assignment (obj.prop = val) =====

class MemberAssignmentExpression : public Expression
{
public:
    std::unique_ptr<Expression> object;
    std::string property;
    std::unique_ptr<Expression> value;

    MemberAssignmentExpression(
        std::unique_ptr<Expression> object,
        const std::string& property,
        std::unique_ptr<Expression> value)
        : object(std::move(object)),
          property(property),
          value(std::move(value))
    {
    }
};

// ===== Call Expression =====
// Callee is now an Expression (Identifier or MemberExpression)

class CallExpression : public Expression
{
public:
    std::unique_ptr<Expression> callee;

    std::vector<
        std::unique_ptr<Expression>
    > arguments;

    explicit CallExpression(
        std::unique_ptr<Expression> callee)
        : callee(std::move(callee))
    {
    }
};

// ===== Functions =====

class FunctionExpression : public Expression
{
public:
    std::string name; // empty for anonymous
    std::vector<std::string> parameters;
    std::unique_ptr<BlockStatement> body;
};

class ArrowFunctionExpression : public Expression
{
public:
    std::vector<std::string> parameters;
    // Either body or expressionBody is set
    std::unique_ptr<BlockStatement> body;
    std::unique_ptr<Expression> expressionBody;
};

// ===== Statements =====

class VariableDeclaration : public Statement
{
public:
    bool isConst;
    std::string name;
    std::unique_ptr<Expression> initializer;

    VariableDeclaration(
        bool isConst,
        const std::string& name,
        std::unique_ptr<Expression> initializer)
        : isConst(isConst),
          name(name),
          initializer(std::move(initializer))
    {
    }
};

class IfStatement : public Statement
{
public:
    std::unique_ptr<Expression> condition;

    std::unique_ptr<BlockStatement>
        thenBranch;

    std::unique_ptr<BlockStatement>
        elseBranch;

    IfStatement(
        std::unique_ptr<Expression> condition,
        std::unique_ptr<BlockStatement>
            thenBranch,
        std::unique_ptr<BlockStatement>
            elseBranch = nullptr)
        : condition(
              std::move(condition)),
          thenBranch(
              std::move(thenBranch)),
          elseBranch(
              std::move(elseBranch))
    {
    }
};

class WhileStatement : public Statement
{
public:
    std::unique_ptr<Expression> condition;

    std::unique_ptr<BlockStatement> body;

    WhileStatement(
        std::unique_ptr<Expression> condition,
        std::unique_ptr<BlockStatement> body)
        : condition(
            std::move(condition)),
          body(
            std::move(body))
    {
    }
};

class DoWhileStatement : public Statement
{
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> body;

    DoWhileStatement(
        std::unique_ptr<Expression> condition,
        std::unique_ptr<BlockStatement> body)
        : condition(std::move(condition)),
          body(std::move(body))
    {
    }
};

class ForStatement : public Statement
{
public:
    std::unique_ptr<Statement>
        initializer;

    std::unique_ptr<Expression>
        condition;

    std::unique_ptr<Expression>
        increment;

    std::unique_ptr<BlockStatement>
        body;

    ForStatement(
        std::unique_ptr<Statement>
            initializer,
        std::unique_ptr<Expression>
            condition,
        std::unique_ptr<Expression>
            increment,
        std::unique_ptr<BlockStatement>
            body)
        : initializer(
              std::move(initializer)),
          condition(
              std::move(condition)),
          increment(
              std::move(increment)),
          body(
              std::move(body))
    {
    }
};

class BreakStatement : public Statement
{
};

class ContinueStatement : public Statement
{
};

class FunctionDeclaration : public Statement
{
public:
    std::string name;

    std::vector<std::string> parameters;

    std::unique_ptr<BlockStatement> body;

    explicit FunctionDeclaration(
        const std::string& name)
        : name(name)
    {
    }
};

class ReturnStatement : public Statement
{
public:
    std::unique_ptr<Expression> value;

    explicit ReturnStatement(
        std::unique_ptr<Expression> value)
        : value(std::move(value))
    {
    }
};

class BlockStatement : public Statement
{
public:
    std::vector<std::unique_ptr<Statement>> statements;
};

class Program : public Node
{
public:
    std::vector<std::unique_ptr<Statement>> statements;
};

class ExpressionStatement : public Statement
{
public:
    std::unique_ptr<Expression> expression;

    explicit ExpressionStatement(
        std::unique_ptr<Expression> expression)
        : expression(std::move(expression))
    {
    }
};
