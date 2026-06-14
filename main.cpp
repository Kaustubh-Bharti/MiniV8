#include <iostream>

#include "runtime/Interpreter.h"
#include "ast/AST.h"

int main()
{
    Interpreter interpreter;

    auto initializer =
        std::make_unique<
            VariableDeclaration>(
                false,
                "i",
                std::make_unique<
                    NumberLiteral>(0)
            );

    auto condition =
        std::make_unique<
            BinaryExpression>(
                std::make_unique<
                    Identifier>("i"),

                "<",

                std::make_unique<
                    NumberLiteral>(3)
            );

    auto increment =
        std::make_unique<
            AssignmentExpression>(
                "i",

                std::make_unique<
                    BinaryExpression>(
                        std::make_unique<
                            Identifier>("i"),

                        "+",

                        std::make_unique<
                            NumberLiteral>(1)
                    )
            );

    auto body =
        std::make_unique<
            BlockStatement>();

    auto loop =
        std::make_unique<
            ForStatement>(
                std::move(initializer),
                std::move(condition),
                std::move(increment),
                std::move(body)
            );

    interpreter.execute(
        loop.get()
    );

    auto result =
        interpreter
            .getEnvironment()
            .get("i");

    std::cout
        << std::get<double>(
            result.value
        )
        << std::endl;

    return 0;
}