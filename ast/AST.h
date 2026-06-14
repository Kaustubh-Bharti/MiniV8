#pragma once

#include <memory>
#include <string>
#include <vector>

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

    UnaryExpression(
        const std::string& op,
        std::unique_ptr<Expression> operand)
        : op(op),
          operand(std::move(operand))
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

    explicit IfStatement(
        std::unique_ptr<Expression> condition)
        : condition(
            std::move(condition))
    {
    }
};

class WhileStatement : public Statement
{
public:
    std::unique_ptr<Expression> condition;

    explicit WhileStatement(
        std::unique_ptr<Expression> condition)
        : condition(
            std::move(condition))
    {
    }
};

