#include <iostream>

#include "runtime/Interpreter.h"
#include "ast/AST.h"

int main()
{
    Interpreter interpreter;

    auto function =
        std::make_unique<
            FunctionDeclaration>(
                "identity"
            );

    function->parameters.push_back(
        "x"
    );

    function->body =
        std::make_unique<
            BlockStatement>();

    function->body
        ->statements.push_back(
            std::make_unique<
                ReturnStatement>(
                    std::make_unique<
                        Identifier>("x")
                )
        );

    interpreter.execute(
        function.get()
    );

    auto result =
        interpreter.callFunction(
            "identity",
            {
                JSValue(99.0)
            }
        );

    std::cout
        << std::get<double>(
            result.value
        )
        << std::endl;

    return 0;
}