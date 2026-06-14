#include <iostream>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "ast/ASTPrinter.h"

int main()
{
    Lexer lexer(
        "let x = value > 5;"
    );

    auto tokens =
        lexer.tokenize();

    Parser parser(tokens);

    auto declaration =
        parser.parseVariableDeclaration();

    if (!declaration)
    {
        std::cout
            << "Parse failed"
            << std::endl;

        return 0;
    }

    printAST(
        declaration->initializer.get()
    );

    return 0;
}