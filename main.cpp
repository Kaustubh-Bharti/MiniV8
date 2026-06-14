#include <iostream>
#include "lexer/Lexer.h"
#include "lexer/TokenUtils.h"

int main()
{
    Lexer lexer(
        "{ name: \"John\", age: 20 }"
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