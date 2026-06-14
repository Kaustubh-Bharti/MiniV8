#include <iostream>

#include "runtime/Interpreter.h"
#include "ast/AST.h"

int main()
{
    Interpreter interpreter;

    interpreter
        .getEnvironment()
        .define(
            "x",
            JSValue(5.0)
        );

    auto expression =
        std::make_unique<
            BinaryExpression>(
                std::make_unique<
                    Identifier>("x"),

                "+",

                std::make_unique<
                    NumberLiteral>(10)
            );

    auto result =
        interpreter.evaluate(
            expression.get()
        );

    std::cout
        << std::get<double>(
            result.value
        )
        << std::endl;

    return 0;
}