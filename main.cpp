#include <iostream>
#include "lexer/Lexer.h"
#include "lexer/TokenUtils.h"

int main()
{
    Lexer lexer(
        "let num = 7;"
        "if (num % 2 === 0)"
        "{"
        "console.log(\"Even\");"
        "}"
    );

    auto tokens = lexer.tokenize();

    for (const auto& token : tokens)
    {
        std::cout
            << tokenTypeToString(token.type)
            << " : "
            << token.value
            << std::endl;
    }

    return 0;
}