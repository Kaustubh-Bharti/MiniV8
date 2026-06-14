#include <iostream>
#include "lexer/Lexer.h"
#include "lexer/TokenUtils.h"

int main()
{
    Lexer lexer(
        "switch case default break continue do new"
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