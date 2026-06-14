#include <iostream>

#include "lexer/Lexer.h"
#include "parser/Parser.h"

int main()
{
    Lexer lexer(
        "identity(123)"
    );

    auto tokens =
        lexer.tokenize();

    Parser parser(tokens);

    auto expression =
        parser.parseExpression();

    if (
        dynamic_cast<
            CallExpression*>(
                expression.get()
            )
    )
    {
        std::cout
            << "Call parsed"
            << std::endl;
    }

    return 0;
}