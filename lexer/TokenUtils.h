#pragma once

#include "Lexer.h"
#include <string>

inline std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
        case TokenType::Number: return "Number";
        case TokenType::String: return "String";
        case TokenType::Identifier: return "Identifier";

        case TokenType::Let: return "Let";
        case TokenType::Const: return "Const";
        case TokenType::Function: return "Function";
        case TokenType::Return: return "Return";

        case TokenType::If: return "If";
        case TokenType::Else: return "Else";

        case TokenType::True: return "True";
        case TokenType::False: return "False";

        case TokenType::Null: return "Null";
        case TokenType::Undefined: return "Undefined";

        case TokenType::Assign: return "Assign";
        case TokenType::StrictEqual: return "StrictEqual";

        case TokenType::Plus: return "Plus";
        case TokenType::Minus: return "Minus";

        case TokenType::Star: return "Star";
        case TokenType::Slash: return "Slash";

        case TokenType::Percent: return "Percent";

        case TokenType::Semicolon: return "Semicolon";

        case TokenType::EndOfFile: return "EOF";

        default:
            return "Other";
    }
}