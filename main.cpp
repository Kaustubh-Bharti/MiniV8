#include <iostream>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "ast/ASTPrinter.h"

int main()
{
    Lexer lexer(
        "function greet() {}"
    );

    auto tokens =
        lexer.tokenize();

    Parser parser(tokens);

    auto function =
        parser.parseFunctionDeclaration();

    if (!function)
    {
        std::cout
            << "Parse failed"
            << std::endl;

        return 0;
    }

    printAST(
        function.get()
    );

    return 0;
}