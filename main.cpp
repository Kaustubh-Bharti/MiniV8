#include <iostream>

#include "lexer/Lexer.h"
#include "parser/Parser.h"

int main()
{
    std::string source =
R"(

if (5 > 10)
{
}
else if (5 > 2)
{
}
else
{
}

)";

    Lexer lexer(source);

    auto tokens =
        lexer.tokenize();

    Parser parser(tokens);

    auto statement =
        parser.parseStatement();

    if (
        dynamic_cast<
            IfStatement*>(
                statement.get()
            )
    )
    {
        std::cout
            << "ELSE IF PARSED"
            << std::endl;
    }

    return 0;
}