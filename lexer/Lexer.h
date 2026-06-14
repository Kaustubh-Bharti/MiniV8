#pragma once

#include <string>
#include <vector>

enum class TokenType
{
    EndOfFile,

    Number,
    String,
    Identifier,

    Let,
    Const,
    Function,
    Return,

    If,
    Else,

    For,
    While,

    True,
    False,
    Null,
    Undefined,

    Plus,
    Minus,
    Star,
    Slash,
    Percent,

    Power,

    Assign,

    Equal,
    StrictEqual,

    NotEqual,
    StrictNotEqual,

    Less,
    LessEqual,

    Greater,
    GreaterEqual,

    LogicalAnd,
    LogicalOr,
    LogicalNot,

    Increment,
    Decrement,

    PlusAssign,
    MinusAssign,

    LeftParen,
    RightParen,

    LeftBrace,
    RightBrace,

    LeftBracket,
    RightBracket,

    Semicolon,
    Comma,
    Dot,
    Colon
};

struct Token
{
    TokenType type;
    std::string value;

    Token(TokenType type, const std::string& value)
        : type(type), value(value)
    {
    }
};

class Lexer
{
private:
    std::string source;
    size_t current;

    char peek() const;
    char peekNext() const;
    char advance();
    bool isAtEnd() const;
    bool match(char expected);

    void addToken(std::vector<Token>& tokens, TokenType type);

    std::string readIdentifier();
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;

    std::string readNumber();
    bool isDigit(char c) const;

    std::string readString();

public:
    Lexer(const std::string& source);

    std::vector<Token> tokenize();
};