#include "Parser.h"
#include "../ast/AST.h"
#include <memory>
#include<iostream>

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

std::unique_ptr<Expression> Parser::parseArrayLiteral()
{
    if (!match(TokenType::LeftBracket))
    {
        return nullptr;
    }

    auto array =
        std::make_unique<
            ArrayLiteral>();

    while (
        !check(TokenType::RightBracket)
    )
    {
        auto element =
            parseExpression();

        if (!element)
        {
            return nullptr;
        }

        array->elements.push_back(
            std::move(element)
        );

        if (
            !check(
                TokenType::RightBracket
            )
        )
        {
            match(TokenType::Comma);
        }
    }

    match(TokenType::RightBracket);

    return array;
}

std::unique_ptr<Expression> Parser::parsePrimary()
{
    if (check(TokenType::LeftBracket))
    {
        return parseArrayLiteral();
    }

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

    if (check(TokenType::Identifier))
    {
        size_t lookahead =
            current + 1;

        while (
            lookahead < tokens.size()
            &&
            tokens[lookahead].type
                == TokenType::Dot
        )
        {
            lookahead += 2;
        }

        if (
            lookahead < tokens.size()
            &&
            tokens[lookahead].type
                == TokenType::LeftParen
        )
        {
            auto call =
                parseCallExpression();

            if (!call)
            {
                return nullptr;
            }

            return std::move(call);
        }

        match(TokenType::Identifier);

        return std::make_unique<
            Identifier>(
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
    return parseAssignment();
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
        match(TokenType::Percent) ||
        match(TokenType::Power)
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

    auto thenBranch =
        parseBlockStatement();

    if (!thenBranch)
    {
        return nullptr;
    }

    std::unique_ptr<
        BlockStatement> elseBranch =
            nullptr;

    if (match(TokenType::Else))
    {
        if (check(TokenType::If))
        {
            auto nestedIf =
                parseIfStatement();

            if (!nestedIf)
            {
                return nullptr;
            }

            elseBranch =
                std::make_unique<
                    BlockStatement>();

            elseBranch
                ->statements.push_back(
                    std::move(nestedIf)
                );
        }
        else
        {
            elseBranch =
                parseBlockStatement();

            if (!elseBranch)
            {
                return nullptr;
            }
        }
    }

    return std::make_unique<
        IfStatement>(
            std::move(condition),
            std::move(thenBranch),
            std::move(elseBranch)
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

    auto body =
        parseBlockStatement();

    if (!body)
    {
        return nullptr;
    }

    return std::make_unique<
        WhileStatement>(
            std::move(condition),
            std::move(body)
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

std::unique_ptr<FunctionDeclaration> Parser::parseFunctionDeclaration()
{
    if (!match(TokenType::Function))
    {
        return nullptr;
    }

    if (!match(TokenType::Identifier))
    {
        return nullptr;
    }

    auto function =
        std::make_unique<FunctionDeclaration>(
            previous().value
        );

    if (!match(TokenType::LeftParen))
    {
        return nullptr;
    }

    while (
        !check(TokenType::RightParen)
    )
    {
        if (!match(TokenType::Identifier))
        {
            return nullptr;
        }

        function->parameters.push_back(
            previous().value
        );

        if (!check(TokenType::RightParen))
        {
            match(TokenType::Comma);
        }
    }

    match(TokenType::RightParen);

    function->body =
        parseBlockStatement();

    return function;
}

std::unique_ptr<ReturnStatement> Parser::parseReturnStatement()
{
    if (!match(TokenType::Return))
    {
        return nullptr;
    }

    auto value =
        parseExpression();

    if (!value)
    {
        return nullptr;
    }

    if (!match(TokenType::Semicolon))
    {
        return nullptr;
    }

    return std::make_unique<
        ReturnStatement>(
            std::move(value)
        );
}

std::unique_ptr<BlockStatement> Parser::parseBlockStatement()
{
    if (!match(TokenType::LeftBrace))
    {
        return nullptr;
    }

    auto block =
        std::make_unique<BlockStatement>();

    while (
        !check(TokenType::RightBrace) &&
        !isAtEnd()
    )
    {
        auto statement =
            parseStatement();

        if (!statement)
        {
            return nullptr;
        }

        block->statements.push_back(
            std::move(statement)
        );
    }

    if (!match(TokenType::RightBrace))
    {
        return nullptr;
    }

    return block;
}

std::unique_ptr<Statement> Parser::parseStatement()
{
    if (check(TokenType::Let) ||
        check(TokenType::Const))
    {
        return parseVariableDeclaration();
    }

    if (check(TokenType::If))
    {
        return parseIfStatement();
    }

    if (check(TokenType::While))
    {
        return parseWhileStatement();
    }

    if (check(TokenType::For))
    {
        return parseForStatement();
    }

    if (check(TokenType::Function))
    {
        return parseFunctionDeclaration();
    }

    if (check(TokenType::Return))
    {
        return parseReturnStatement();
    }

    auto expression =
        parseExpression();

    if (!expression)
    {
        return nullptr;
    }

    if (match(TokenType::Semicolon))
    {
    }

    return std::make_unique<
        ExpressionStatement>(
            std::move(expression)
        );
}

std::unique_ptr<Program> Parser::parseProgram()
{
    auto program =
        std::make_unique<Program>();

    while (!isAtEnd())
    {
        auto statement =
            parseStatement();

        if (!statement)
        {
            break;
        }

        program->statements.push_back(
            std::move(statement)
        );
    }

    return program;
}

std::unique_ptr<CallExpression> Parser::parseCallExpression()
{
    if (!match(TokenType::Identifier))
    {
        return nullptr;
    }

    std::string callee =
        previous().value;

    while (match(TokenType::Dot))
    {
        if (!match(TokenType::Identifier))
        {
            return nullptr;
        }

        callee += ".";
        callee += previous().value;
    }

    if (!match(TokenType::LeftParen))
    {
        return nullptr;
    }

    auto call =
        std::make_unique<
            CallExpression>(
                callee
            );

    while (
        !check(TokenType::RightParen)
    )
    {
        auto argument =
            parseExpression();

        if (!argument)
        {
            return nullptr;
        }

        call->arguments.push_back(
            std::move(argument)
        );

        if (!check(TokenType::RightParen))
        {
            match(TokenType::Comma);
        }
    }

    match(TokenType::RightParen);
    std::cout
    << "PARSED CALL: "
    << call->callee
    << std::endl;
    return call;
}

std::unique_ptr<ForStatement> Parser::parseForStatement()
{
    
    if (!match(TokenType::For))
    {
        return nullptr;
    }

    if (!match(TokenType::LeftParen))
    {
        return nullptr;
    }

    auto initializer =
        parseForVariableDeclaration();

    if (!initializer)
    {
        return nullptr;
    }

    auto condition =
        parseExpression();

    if (!condition)
    {
        return nullptr;
    }

    if (!match(TokenType::Semicolon))
    {
        return nullptr;
    }

    auto increment =
        parseExpression();

    if (!increment)
    {
        return nullptr;
    }

    if (!match(TokenType::RightParen))
    {
        return nullptr;
    }

    auto body =
        parseBlockStatement();

    if (!body)
    {
        return nullptr;
    }


    return std::make_unique<ForStatement>(
        std::move(initializer),
        std::move(condition),
        std::move(increment),
        std::move(body)
    );
}

std::unique_ptr<Expression> Parser::parseAssignment()
{
    auto expression =
        parseLogical();

    if (
        match(TokenType::Assign) ||
        match(TokenType::PlusEqual) ||
        match(TokenType::MinusEqual) ||
        match(TokenType::StarEqual) ||
        match(TokenType::SlashEqual) ||
        match(TokenType::PercentEqual)
    )
    {
        TokenType assignmentType =
            previous().type;

        auto value =
            parseAssignment();

        auto identifier =
            dynamic_cast<
                Identifier*>(
                    expression.get()
                );

        if (!identifier)
        {
            return nullptr;
        }

        std::string op;

        if (assignmentType == TokenType::PlusEqual)
        {
            op = "+";
        }
        else if (assignmentType == TokenType::MinusEqual)
        {
            op = "-";
        }
        else if (assignmentType == TokenType::StarEqual)
        {
            op = "*";
        }
        else if (assignmentType == TokenType::SlashEqual)
        {
            op = "/";
        }
        else if (assignmentType == TokenType::PercentEqual)
        {
            op = "%";
        }

        if (!op.empty())
        {
            value =
                std::make_unique<
                    BinaryExpression>(
                        std::make_unique<
                            Identifier>(
                                identifier->name
                            ),

                        op,

                        std::move(value)
                    );
        }

        return std::make_unique<
            AssignmentExpression>(
                identifier->name,
                std::move(value)
            );
    }

    return expression;
}

std::unique_ptr<Expression> Parser::parseUnary()
{
    if (match(TokenType::PlusPlus))
    {
        auto operand =
            parsePrimary();

        return std::make_unique<
            UnaryExpression>(
                "++",
                std::move(operand)
            );
    }

    if (match(TokenType::MinusMinus))
    {
        auto operand =
            parsePrimary();

        return std::make_unique<
            UnaryExpression>(
                "--",
                std::move(operand)
            );
    }

    auto expression =
        parsePrimary();

    if (match(TokenType::PlusPlus))
    {
        return std::make_unique<
            UnaryExpression>(
                "++",
                std::move(expression)
            );
    }

    if (match(TokenType::MinusMinus))
    {
        return std::make_unique<
            UnaryExpression>(
                "--",
                std::move(expression)
            );
    }

    return expression;
}

std::unique_ptr<VariableDeclaration> Parser::parseForVariableDeclaration()
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

    auto initializer =
        parseExpression();
    
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

