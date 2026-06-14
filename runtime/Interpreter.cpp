#include "Interpreter.h"
#include <variant>
#include <stdexcept>
#include <iostream>

bool isTruthy(const JSValue& value)
{
    if (std::holds_alternative<bool>(value.value))
    {
        return std::get<bool>(value.value);
    }

    if (std::holds_alternative<double>(value.value))
    {
        return std::get<double>(value.value) != 0;
    }

    if (std::holds_alternative<std::string>(value.value))
    {
        return !std::get<std::string>(
            value.value
        ).empty();
    }

    return false;
}

JSValue Interpreter::evaluate(
    Expression* expression)
{
    if (!expression)
    {
        throw std::runtime_error(
            "Null expression");
    }

    if (auto number =
        dynamic_cast<
            NumberLiteral*>(
                expression))
    {
        return JSValue(
            number->value
        );
    }

    if (auto boolean =
        dynamic_cast<
            BooleanLiteral*>(
                expression))
    {
        return JSValue(
            boolean->value
        );
    }

    if (auto stringLiteral =
        dynamic_cast<
            StringLiteral*>(
                expression))
    {
        return JSValue(
            stringLiteral->value
        );
    }

    if (auto identifier =
        dynamic_cast<
            Identifier*>(
                expression))
    {
        return environment.get(
            identifier->name
        );
    }

    if (auto binary =
    dynamic_cast<
        BinaryExpression*>(
            expression))
    {
        JSValue left =
            evaluate(
                binary->left.get()
            );

        JSValue right =
            evaluate(
                binary->right.get()
            );

        double lhs =
            std::get<double>(
                left.value
            );

        double rhs =
            std::get<double>(
                right.value
            );

        if (binary->op == "+")
            return JSValue(lhs + rhs);

        if (binary->op == "-")
            return JSValue(lhs - rhs);

        if (binary->op == "*")
            return JSValue(lhs * rhs);

        if (binary->op == "/")
            return JSValue(lhs / rhs);

        if (binary->op == "%")
            return JSValue(
                static_cast<double>(
                    static_cast<int>(lhs)
                    %
                    static_cast<int>(rhs)
                )
            );

        if (binary->op == ">")
            return JSValue(lhs > rhs);

        if (binary->op == "<")
            return JSValue(lhs < rhs);

        if (binary->op == ">=")
            return JSValue(lhs >= rhs);

        if (binary->op == "<=")
            return JSValue(lhs <= rhs);

        if (binary->op == "==")
            return JSValue(lhs == rhs);

        if (binary->op == "===")
            return JSValue(lhs == rhs);

        if (binary->op == "!=")
            return JSValue(lhs != rhs);

        if (binary->op == "!==")
            return JSValue(lhs != rhs);
    }

    if (auto call =
    dynamic_cast<
        CallExpression*>(
            expression))
    {
        std::vector<JSValue> args;

        for (auto& argument :
            call->arguments)
        {
            args.push_back(
                evaluate(
                    argument.get()
                )
            );
        }

        return callFunction(
            call->callee,
            args
        );
    }

    throw std::runtime_error(
        "Unsupported expression");
}

void Interpreter::execute(
    Statement* statement)
{
    if (!statement)
    {
        return;
    }

    if (auto variable =
        dynamic_cast<
            VariableDeclaration*>(
                statement))
    {
        JSValue value =
            evaluate(
                variable->initializer.get()
            );

        environment.define(
            variable->name,
            value
        );

        return;
    }

    if (auto ifStatement =
        dynamic_cast<
            IfStatement*>(
                statement))
    {
        JSValue condition =
            evaluate(
                ifStatement
                    ->condition.get()
            );

        if (isTruthy(condition))
        {
            std::cout<< "IF EXECUTED"<< std::endl;
        }

        return;
    }

    if (auto expressionStatement =
    dynamic_cast<
        ExpressionStatement*>(
            statement))
    {
        evaluate(
            expressionStatement
                ->expression.get()
        );

        return;
    }

    if (auto whileStatement =
        dynamic_cast<
            WhileStatement*>(
                statement))
    {
        while (
            isTruthy(
                evaluate(
                    whileStatement
                        ->condition.get()
                )
            )
        )
        {
            std::cout
                << "WHILE ITERATION"
                << std::endl;

            break;
        }

        return;
    }

    if (auto function =
        dynamic_cast<
            FunctionDeclaration*>(
                statement))
    {
        environment.defineFunction(
            function->name,
            function
        );

        return;
    }

    if (auto returnStatement =
        dynamic_cast<
            ReturnStatement*>(
                statement))
    {
        returnValue =
            evaluate(
                returnStatement
                    ->value.get()
            );

        return;
    }
}

void Interpreter::executeProgram(
    Program* program)
{
    if (!program)
    {
        return;
    }

    for (auto& statement :
         program->statements)
    {
        execute(
            statement.get()
        );
    }
}

JSValue Interpreter::callFunction(
    const std::string& name,
    const std::vector<JSValue>& arguments)
{
    if (name == "console.log")
    {
        if (!arguments.empty())
        {
            const JSValue& value =
                arguments[0];

            if (std::holds_alternative<double>(
                    value.value))
            {
                std::cout
                    << std::get<double>(
                        value.value)
                    << std::endl;
            }
            else if (
                std::holds_alternative<bool>(
                    value.value))
            {
                std::cout
                    << std::get<bool>(
                        value.value)
                    << std::endl;
            }
            else if (
                std::holds_alternative<
                    std::string>(
                    value.value))
            {
                std::cout
                    << std::get<std::string>(
                        value.value)
                    << std::endl;
            }
        }

        return JSValue(0.0);
    }

    auto function =
        environment.getFunction(
            name
        );

    returnValue.reset();

    for (
        size_t i = 0;
        i < function->parameters.size()
        &&
        i < arguments.size();
        i++
    )
    {
        environment.define(
            function->parameters[i],
            arguments[i]
        );
    }

    if (function->body)
    {
        for (auto& statement :
             function->body->statements)
        {
            execute(
                statement.get()
            );

            if (returnValue.has_value())
            {
                return *returnValue;
            }
        }
    }

    return JSValue(0.0);
}