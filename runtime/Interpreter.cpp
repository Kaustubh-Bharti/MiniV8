#include "Interpreter.h"
#include <variant>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <algorithm>

std::string numberToString(
    double number)
{
    if (
        number ==
        static_cast<int>(number)
    )
    {
        return std::to_string(
            static_cast<int>(
                number
            )
        );
    }

    return std::to_string(
        number
    );
}

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

    if (auto unary =
        dynamic_cast<
            UnaryExpression*>(
                expression))
    {
        auto identifier =
            dynamic_cast<
                Identifier*>(
                    unary->operand.get()
                );

        if (!identifier)
        {
            throw std::runtime_error(
                "Unary operator requires identifier"
            );
        }

        JSValue current =
            environment.get(
                identifier->name
            );

        double value =
            std::get<double>(
                current.value
            );

        if (unary->op == "++")
        {
            value++;

            JSValue result(
                value
            );

            environment.assign(
                identifier->name,
                result
            );

            return result;
        }

        if (unary->op == "--")
        {
            value--;

            JSValue result(
                value
            );

            environment.assign(
                identifier->name,
                result
            );

            return result;
        }
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

        if (binary->op == "+")
        {
            if (
                std::holds_alternative<
                    std::string>(
                        left.value)
                ||
                std::holds_alternative<
                    std::string>(
                        right.value))
            {
                std::string lhs;

                std::string rhs;

                if (
                    std::holds_alternative<
                        std::string>(
                            left.value))
                {
                    lhs =
                        std::get<std::string>(
                            left.value);
                }
                else
                {
                    lhs =
                        numberToString(
                            std::get<double>(
                                left.value
                            )
                        );
                }

                if (
                    std::holds_alternative<
                        std::string>(
                            right.value))
                {
                    rhs =
                        std::get<std::string>(
                            right.value);
                }
                else
                {
                    rhs =
                        numberToString(
                            std::get<double>(
                                right.value
                            )
                        );
                }

                return JSValue(
                    lhs + rhs
                );
            }
        }

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
        
        if (binary->op == "**")
        {
            return JSValue(
                std::pow(lhs, rhs)
            );
        }

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
        std::cout
    << "CALL: "
    << call->callee
    << std::endl;

    
    if (
        call->callee.size() > 6
        &&
        call->callee.substr(
            call->callee.size() - 6
        ) == ".split"
    )
    {
        std::string variableName =
            call->callee.substr(
                0,
                call->callee.size() - 6
            );

        JSValue stringValue =
            environment.get(
                variableName
            );

        std::string text =
            std::get<std::string>(
                stringValue.value
            );

        std::vector<JSValue> result;

        for (char c : text)
        {
            result.push_back(
                JSValue(
                    std::string(
                        1,
                        c
                    )
                )
            );
        }

        return JSValue(
            result
        );
    }
        if (
            call->callee.size() > 8
            &&
            call->callee.substr(
                call->callee.size() - 8
            ) == ".reverse"
        )
        {
            std::string variableName =
                call->callee.substr(
                    0,
                    call->callee.size() - 8
                );

            JSValue arrayValue =
                environment.get(
                    variableName
                );

            auto array =
                std::get<
                    std::vector<JSValue>>(
                        arrayValue.value
                    );

            std::reverse(
                array.begin(),
                array.end()
            );

            return JSValue(
                array
            );
        }
        if (
            call->callee.size() > 5
            &&
            call->callee.substr(
                call->callee.size() - 5
            ) == ".join"
        )
        {
            std::string variableName =
                call->callee.substr(
                    0,
                    call->callee.size() - 5
                );

            JSValue arrayValue =
                environment.get(
                    variableName
                );

            auto array =
                std::get<
                    std::vector<JSValue>>(
                        arrayValue.value
                    );

            std::string separator =
                ",";

            if (!call->arguments.empty())
            {
                auto separatorValue =
                    evaluate(
                        call->arguments[0].get()
                    );

                separator =
                    std::get<std::string>(
                        separatorValue.value
                    );
            }

            std::string result;

            for (
                size_t i = 0;
                i < array.size();
                i++
            )
            {
                if (i > 0)
                {
                    result += separator;
                }

                if (
                    std::holds_alternative<
                        double>(
                            array[i].value))
                {
                    result +=
                        numberToString(
                            std::get<double>(
                                array[i].value
                            )
                        );
                }
                else if (
                    std::holds_alternative<
                        std::string>(
                            array[i].value))
                {
                    result +=
                        std::get<std::string>(
                            array[i].value
                        );
                }
                else if (
                    std::holds_alternative<
                        bool>(
                            array[i].value))
                {
                    result +=
                        (
                            std::get<bool>(
                                array[i].value
                            )
                            ? "true"
                            : "false"
                        );
                }
            }

            return JSValue(
                result
            );
        }

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

    if (auto assignment =
        dynamic_cast<
            AssignmentExpression*>(
                expression))
    {
        JSValue value =
            evaluate(
                assignment
                    ->value.get()
            );

        environment.assign(
            assignment
                ->variableName,
            value
        );

        return value;
    }

    if (auto array =
        dynamic_cast<
            ArrayLiteral*>(
                expression))
    {
        std::vector<JSValue>
            values;

        for (
            auto& element :
            array->elements
        )
        {
            values.push_back(
                evaluate(
                    element.get()
                )
            );
        }

        return JSValue(
            values
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
            if (ifStatement->thenBranch)
            {
                for (auto& stmt :
                    ifStatement
                        ->thenBranch
                        ->statements)
                {
                    execute(
                        stmt.get()
                    );
                }
            }
        }
        else
        {
            if (ifStatement->elseBranch)
            {
                for (auto& stmt :
                    ifStatement
                        ->elseBranch
                        ->statements)
                {
                    execute(
                        stmt.get()
                    );
                }
            }
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
            

            if (whileStatement->body)
            {
                for (auto& stmt :
                    whileStatement
                        ->body
                        ->statements)
                {
                    execute(
                        stmt.get()
                    );
                }
            }
        }

        return;
    }

    if (auto forStatement =
        dynamic_cast<
            ForStatement*>(
                statement))
    {
        if (forStatement->initializer)
        {
            execute(
                forStatement
                    ->initializer.get()
            );
        }

        int safetyCounter = 0;

        while (
            !forStatement->condition
            ||
            isTruthy(
                evaluate(
                    forStatement
                        ->condition.get()
                )
            )
        )
        {
            if (forStatement->body)
            {
                for (auto& stmt :
                    forStatement
                        ->body
                        ->statements)
                {
                    execute(
                        stmt.get()
                    );
                }
            }

            if (forStatement->increment)
            {
                evaluate(
                    forStatement
                        ->increment.get()
                );
            }

            safetyCounter++;

            if (safetyCounter > 1000)
            {
                break;
            }
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
                    << (
                        std::get<bool>(
                            value.value
                        )
                        ? "true"
                        : "false"
                    )
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
            else if (
                std::holds_alternative<
                    std::vector<JSValue>>(
                        value.value))
            {
                auto array =
                    std::get<
                        std::vector<JSValue>>(
                            value.value
                        );

                std::cout
                    << "[";

                for (
                    size_t i = 0;
                    i < array.size();
                    i++
                )
                {
                    if (i > 0)
                    {
                        std::cout
                            << ",";
                    }

                    if (
                        std::holds_alternative<
                            std::string>(
                                array[i].value))
                    {
                        std::cout
                            << std::get<std::string>(
                                array[i].value
                            );
                    }
                }

                std::cout
                    << "]"
                    << std::endl;
            }
        }

        return JSValue(0.0);
    }

    if (name == "Math.floor")
    {
        return JSValue(
            std::floor(
                std::get<double>(
                    arguments[0].value
                )
            )
        );
    }

    if (name == "Math.abs")
    {
        return JSValue(
            std::abs(
                std::get<double>(
                    arguments[0].value
                )
            )
        );
    }

    if (name == "Math.pow")
    {
        return JSValue(
            std::pow(
                std::get<double>(
                    arguments[0].value
                ),
                std::get<double>(
                    arguments[1].value
                )
            )
        );
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

