#include "ASTPrinter.h"

#include <iostream>

void printIndent(int indent)
{
    for(int i = 0; i < indent; i++)
    {
        std::cout << "  ";
    }
}

void printAST(
    const Expression* expression,
    int indent)
{
    if (!expression)
    {
        return;
    }

    if (auto number =
        dynamic_cast<
            const NumberLiteral*>(
                expression))
    {
        printIndent(indent);

        std::cout
            << "NumberLiteral("
            << number->value
            << ")"
            << std::endl;

        return;
    }

    if (auto identifier =
        dynamic_cast<
            const Identifier*>(
                expression))
    {
        printIndent(indent);

        std::cout
            << "Identifier("
            << identifier->name
            << ")"
            << std::endl;

        return;
    }

    if (auto binary =
        dynamic_cast<
            const BinaryExpression*>(
                expression))
    {
        printIndent(indent);

        std::cout
            << "BinaryExpression("
            << binary->op
            << ")"
            << std::endl;

        printAST(
            binary->left.get(),
            indent + 1
        );

        printAST(
            binary->right.get(),
            indent + 1
        );

        return;
    }
}

void printAST(
    const IfStatement* statement,
    int indent)
{
    if (!statement)
    {
        return;
    }

    printIndent(indent);

    std::cout
        << "IfStatement"
        << std::endl;

    printAST(
        statement->condition.get(),
        indent + 1
    );
}

void printAST(
    const WhileStatement* statement,
    int indent)
{
    if (!statement)
    {
        return;
    }

    printIndent(indent);

    std::cout
        << "WhileStatement"
        << std::endl;

    printAST(
        statement->condition.get(),
        indent + 1
    );
}

void printAST(
    const FunctionDeclaration* statement,
    int indent)
{
    if (!statement)
    {
        return;
    }

    printIndent(indent);

    std::cout
        << "FunctionDeclaration("
        << statement->name
        << ")"
        << std::endl;
}

void printAST(
    const ReturnStatement* statement,
    int indent)
{
    if (!statement)
    {
        return;
    }

    printIndent(indent);

    std::cout
        << "ReturnStatement"
        << std::endl;

    printAST(
        statement->value.get(),
        indent + 1
    );
}

