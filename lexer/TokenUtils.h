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

        case TokenType::Typeof: return "Typeof";

        case TokenType::Assign: return "Assign";
        case TokenType::StrictEqual: return "StrictEqual";
        case TokenType::Equal: return "Equal";
        case TokenType::NotEqual: return "NotEqual";
        case TokenType::StrictNotEqual: return "StrictNotEqual";

        case TokenType::Plus: return "Plus";
        case TokenType::Minus: return "Minus";

        case TokenType::Star: return "Star";
        case TokenType::Slash: return "Slash";

        case TokenType::Percent: return "Percent";
        case TokenType::Power: return "Power";

        case TokenType::Semicolon: return "Semicolon";

        case TokenType::EndOfFile: return "EOF";

        case TokenType::Colon: return "Colon";

        case TokenType::LeftParen: return "LeftParen";

        case TokenType::RightParen: return "RightParen";

        case TokenType::LeftBrace: return "LeftBrace";

        case TokenType::RightBrace: return "RightBrace";

        case TokenType::LeftBracket: return "LeftBracket";

        case TokenType::RightBracket: return "RightBracket";

        case TokenType::Comma: return "Comma";

        case TokenType::Dot: return "Dot";

        case TokenType::Arrow: return "Arrow";

        case TokenType::Spread: return "Spread";

        case TokenType::Switch: return "Switch";

        case TokenType::Case: return "Case";

        case TokenType::Default: return "Default";

        case TokenType::Break: return "Break";

        case TokenType::Continue: return "Continue";

        case TokenType::Do: return "Do";

        case TokenType::New: return "New";

        case TokenType::QuestionMark: return "QuestionMark";

        case TokenType::LogicalAnd: return "LogicalAnd";
        case TokenType::LogicalOr: return "LogicalOr";
        case TokenType::LogicalNot: return "LogicalNot";

        case TokenType::PlusPlus: return "PlusPlus";
        case TokenType::MinusMinus: return "MinusMinus";

        case TokenType::PlusEqual: return "PlusEqual";
        case TokenType::MinusEqual: return "MinusEqual";
        case TokenType::StarEqual: return "StarEqual";
        case TokenType::SlashEqual: return "SlashEqual";
        case TokenType::PercentEqual: return "PercentEqual";

        case TokenType::Less: return "Less";
        case TokenType::LessEqual: return "LessEqual";
        case TokenType::Greater: return "Greater";
        case TokenType::GreaterEqual: return "GreaterEqual";

        case TokenType::Increment: return "Increment";
        case TokenType::Decrement: return "Decrement";

        case TokenType::PlusAssign: return "PlusAssign";
        case TokenType::MinusAssign: return "MinusAssign";

        case TokenType::For: return "For";
        case TokenType::While: return "While";

        default:
            return "Other";
    }
}