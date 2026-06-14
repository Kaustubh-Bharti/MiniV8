#include <iostream>

#include "runtime/Interpreter.h"
#include "ast/AST.h"

int main()
{
    Interpreter interpreter;

    auto expression =
        std::make_unique<
            BinaryExpression>(
                std::make_unique<
                    StringLiteral>(
                        "Value = "
                    ),

                "+",

                std::make_unique<
                    NumberLiteral>(
                        42
                    )
            );

    auto result =
        interpreter.evaluate(
            expression.get()
        );

    std::cout
        << std::get<std::string>(
            result.value
        )
        << std::endl;

    return 0;
}