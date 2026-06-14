#include "ASTPrinter.h"

#include <iostream>

static void printIndent(int indent)
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

    if (auto str =
        dynamic_cast<
            const StringLiteral*>(
                expression))
    {
        printIndent(indent);
        std::cout
            << "StringLiteral(\""
            << str->value
            << "\")"
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
            indent + 1);

        printAST(
            binary->right.get(),
            indent + 1);

        return;
    }

    if (auto call =
        dynamic_cast<
            const CallExpression*>(
                expression))
    {
        printIndent(indent);
        std::cout
            << "CallExpression"
            << std::endl;

        printAST(
            call->callee.get(),
            indent + 1);

        return;
    }

    if (auto member =
        dynamic_cast<
            const MemberExpression*>(
                expression))
    {
        printIndent(indent);
        std::cout
            << "MemberExpression(."
            << member->property
            << ")"
            << std::endl;

        printAST(
            member->object.get(),
            indent + 1);

        return;
    }

    printIndent(indent);
    std::cout << "Expression" << std::endl;
}

void printAST(
    const Statement* statement,
    int indent)
{
    if (!statement)
    {
        return;
    }

    if (auto ifStmt =
        dynamic_cast<
            const IfStatement*>(statement))
    {
        printIndent(indent);
        std::cout << "IfStatement"
            << std::endl;
        printAST(
            ifStmt->condition.get(),
            indent + 1);
        return;
    }

    if (auto whileStmt =
        dynamic_cast<
            const WhileStatement*>(statement))
    {
        printIndent(indent);
        std::cout << "WhileStatement"
            << std::endl;
        printAST(
            whileStmt->condition.get(),
            indent + 1);
        return;
    }

    if (auto funcDecl =
        dynamic_cast<
            const FunctionDeclaration*>(
                statement))
    {
        printIndent(indent);
        std::cout
            << "FunctionDeclaration("
            << funcDecl->name
            << ")"
            << std::endl;
        return;
    }

    if (auto retStmt =
        dynamic_cast<
            const ReturnStatement*>(statement))
    {
        printIndent(indent);
        std::cout << "ReturnStatement"
            << std::endl;
        printAST(
            retStmt->value.get(),
            indent + 1);
        return;
    }

    printIndent(indent);
    std::cout << "Statement" << std::endl;
}
