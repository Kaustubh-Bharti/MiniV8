#include <iostream>

#include "runtime/Interpreter.h"
#include "ast/AST.h"

int main()
{
    Interpreter interpreter;

    auto function =
        std::make_unique<
            FunctionDeclaration>(
                "greet"
            );

    interpreter.execute(
        function.get()
    );

    auto stored =
        interpreter
            .getEnvironment()
            .getFunction(
                "greet"
            );

    if (stored)
    {
        std::cout
            << stored->name
            << std::endl;
    }

    return 0;
}