#include <iostream>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "ast/ASTPrinter.h"

int main()
{
    Lexer lexer(
        "while (count < 10) {}"
    );

    auto tokens =
        lexer.tokenize();

    Parser parser(tokens);

    auto statement =
        parser.parseWhileStatement();

    if (!statement)
    {
        std::cout
            << "Parse failed"
            << std::endl;

        return 0;
    }

    printAST(
        statement.get()
    );

    return 0;
}