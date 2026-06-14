#include "Parser.h"
#include "../ast/AST.h"
#include <memory>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens),
      current(0)
{
}

bool Parser::isAtEnd() const
{
    return peek().type == TokenType::EndOfFile;
}

const Token& Parser::peek() const
{
    return tokens[current];
}

const Token& Parser::previous() const
{
    return tokens[current - 1];
}

const Token& Parser::advance()
{
    if (!isAtEnd())
    {
        current++;
    }

    return previous();
}

bool Parser::check(TokenType type) const
{
    if (isAtEnd())
    {
        return false;
    }

    return peek().type == type;
}

bool Parser::match(TokenType type)
{
    if (check(type))
    {
        advance();
        return true;
    }

    return false;
}

std::unique_ptr<Expression> Parser::parsePrimary()
{
    if (match(TokenType::Number))
    {
        double value =
            std::stod(previous().value);

        return std::make_unique<NumberLiteral>(
            value
        );
    }

    if (match(TokenType::String))
    {
        return std::make_unique<StringLiteral>(
            previous().value
        );
    }

    if (match(TokenType::Identifier))
    {
        return std::make_unique<Identifier>(
            previous().value
        );
    }

    if (match(TokenType::True))
    {
        return std::make_unique<BooleanLiteral>(
            true
        );
    }

    if (match(TokenType::False))
    {
        return std::make_unique<BooleanLiteral>(
            false
        );
    }

    if (match(TokenType::Null))
    {
        return std::make_unique<NullLiteral>();
    }

    if (match(TokenType::Undefined))
    {
        return std::make_unique<UndefinedLiteral>();
    }

    return nullptr;

}

std::unique_ptr<Expression> Parser::parseExpression()
{
    return parseLogical();
}

std::unique_ptr<Expression> Parser::parseEquality()
{
    auto expression =
        parseComparison();

    while (
        match(TokenType::Equal) ||
        match(TokenType::StrictEqual) ||
        match(TokenType::NotEqual) ||
        match(TokenType::StrictNotEqual)
    )
    {
        std::string op =
            previous().value;

        auto right =
            parseComparison();

        expression =
            std::make_unique<
                BinaryExpression>(
                    std::move(expression),
                    op,
                    std::move(right)
                );
    }

    return expression;
}

std::unique_ptr<Expression> Parser::parseComparison()
{
    auto expression =
        parseTerm();

    while (
        match(TokenType::Less) ||
        match(TokenType::LessEqual) ||
        match(TokenType::Greater) ||
        match(TokenType::GreaterEqual)
    )
    {
        std::string op =
            previous().value;

        auto right =
            parseTerm();

        expression =
            std::make_unique<
                BinaryExpression>(
                    std::move(expression),
                    op,
                    std::move(right)
                );
    }

    return expression;
}

std::unique_ptr<Expression> Parser::parseTerm()
{
    auto expression =
        parseFactor();

    while (
        match(TokenType::Plus) ||
        match(TokenType::Minus)
    )
    {
        std::string op =
            previous().value;

        auto right =
            parseFactor();

        expression =
            std::make_unique<
                BinaryExpression>(
                    std::move(expression),
                    op,
                    std::move(right)
                );
    }

    return expression;
}

std::unique_ptr<Expression> Parser::parseFactor()
{
    auto expression =
        parseUnary();

    while (
        match(TokenType::Star) ||
        match(TokenType::Slash) ||
        match(TokenType::Percent)
    )
    {
        std::string op =
            previous().value;

        auto right =
            parseUnary();

        expression =
            std::make_unique<
                BinaryExpression>(
                    std::move(expression),
                    op,
                    std::move(right)
                );
    }

    return expression;
}

std::unique_ptr<Expression> Parser::parseUnary()
{
    return parsePrimary();
}

std::unique_ptr<VariableDeclaration> Parser::parseVariableDeclaration()
{
    bool isConst = false;

    if (match(TokenType::Const))
    {
        isConst = true;
    }
    else if (match(TokenType::Let))
    {
        isConst = false;
    }
    else
    {
        return nullptr;
    }

    if (!match(TokenType::Identifier))
    {
        return nullptr;
    }

    std::string name =
        previous().value;

    if (!match(TokenType::Assign))
    {
        return nullptr;
    }

    auto initializer = parseExpression();

    if (!initializer)
    {
        return nullptr;
    }

    if (!match(TokenType::Semicolon))
    {
        return nullptr;
    }

    return std::make_unique<
        VariableDeclaration>(
            isConst,
            name,
            std::move(initializer)
        );
}

std::unique_ptr<IfStatement> Parser::parseIfStatement()
{
    if (!match(TokenType::If))
    {
        return nullptr;
    }

    if (!match(TokenType::LeftParen))
    {
        return nullptr;
    }

    auto condition =
        parseExpression();

    if (!condition)
    {
        return nullptr;
    }

    if (!match(TokenType::RightParen))
    {
        return nullptr;
    }

    if (!match(TokenType::LeftBrace))
    {
        return nullptr;
    }

    if (!match(TokenType::RightBrace))
    {
        return nullptr;
    }

    return std::make_unique<
        IfStatement>(
            std::move(condition)
        );
}

std::unique_ptr<WhileStatement> Parser::parseWhileStatement()
{
    if (!match(TokenType::While))
    {
        return nullptr;
    }

    if (!match(TokenType::LeftParen))
    {
        return nullptr;
    }

    auto condition =
        parseExpression();

    if (!condition)
    {
        return nullptr;
    }

    if (!match(TokenType::RightParen))
    {
        return nullptr;
    }

    if (!match(TokenType::LeftBrace))
    {
        return nullptr;
    }

    if (!match(TokenType::RightBrace))
    {
        return nullptr;
    }

    return std::make_unique<
        WhileStatement>(
            std::move(condition)
        );
}

std::unique_ptr<Expression> Parser::parseLogical()
{
    auto expression =
        parseEquality();

    while (
        match(TokenType::LogicalAnd) ||
        match(TokenType::LogicalOr)
    )
    {
        std::string op =
            previous().value;

        auto right =
            parseEquality();

        expression =
            std::make_unique<
                BinaryExpression>(
                    std::move(expression),
                    op,
                    std::move(right)
                );
    }

    return expression;
}



