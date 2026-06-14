#include <iostream>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "runtime/Interpreter.h"

int main()
{
    std::string source =
R"(

function identity(x)
{
    return x;
}

)";

    Lexer lexer(source);

    auto tokens =
        lexer.tokenize();

    Parser parser(tokens);

    auto program =
        parser.parseProgram();

    Interpreter interpreter;

    interpreter.executeProgram(
        program.get()
    );

    auto result =
        interpreter.callFunction(
            "identity",
            {
                JSValue(777.0)
            }
        );

    std::cout
        << std::get<double>(
            result.value
        )
        << std::endl;

    return 0;
}