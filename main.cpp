#include <iostream>

#include "runtime/Interpreter.h"
#include "ast/AST.h"

int main()
{
    Interpreter interpreter;

    auto condition =
        std::make_unique<
            BinaryExpression>(
                std::make_unique<
                    NumberLiteral>(5),

                ">",

                std::make_unique<
                    NumberLiteral>(2)
            );

    auto whileStatement =
        std::make_unique<
            WhileStatement>(
                std::move(condition)
            );

    interpreter.execute(
        whileStatement.get()
    );

    return 0;
}