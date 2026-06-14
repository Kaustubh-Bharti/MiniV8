#include "Parser.h"
#include "../ast/AST.h"
#include <memory>
#include <iostream>

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

// ========== Arrow function detection ==========

bool Parser::isArrowFunction() const
{
    size_t pos = current;

    // Case 1: identifier =>
    if (pos < tokens.size() &&
        tokens[pos].type == TokenType::Identifier &&
        pos + 1 < tokens.size() &&
        tokens[pos + 1].type == TokenType::Arrow)
    {
        return true;
    }

    // Case 2: ( ... ) =>
    if (pos < tokens.size() &&
        tokens[pos].type == TokenType::LeftParen)
    {
        size_t temp = pos + 1;
        int depth = 1;

        while (temp < tokens.size() && depth > 0)
        {
            if (tokens[temp].type == TokenType::LeftParen)
                depth++;
            else if (tokens[temp].type == TokenType::RightParen)
                depth--;
            temp++;
        }

        if (temp < tokens.size() &&
            tokens[temp].type == TokenType::Arrow)
        {
            return true;
        }
    }

    return false;
}

// ========== Expression Parsing ==========

std::unique_ptr<Expression> Parser::parseExpression()
{
    return parseAssignment();
}

std::unique_ptr<Expression> Parser::parseAssignment()
{
    // Check for arrow function first
    if (isArrowFunction())
    {
        // Single param: x => ...
        if (check(TokenType::Identifier) &&
            current + 1 < tokens.size() &&
            tokens[current + 1].type == TokenType::Arrow)
        {
            std::string param = advance().value;
            advance(); // consume =>

            std::vector<std::string> params;
            params.push_back(param);

            return parseArrowFunction(params);
        }

        // Multi param: (x, y) => ...
        if (check(TokenType::LeftParen))
        {
            advance(); // consume (

            std::vector<std::string> params;

            while (!check(TokenType::RightParen))
            {
                if (match(TokenType::Identifier))
                {
                    params.push_back(
                        previous().value);
                }

                if (!check(TokenType::RightParen))
                {
                    match(TokenType::Comma);
                }
            }

            match(TokenType::RightParen);
            match(TokenType::Arrow);

            return parseArrowFunction(params);
        }
    }

    auto expression = parseTernary();

    if (
        check(TokenType::Assign) ||
        check(TokenType::PlusEqual) ||
        check(TokenType::MinusEqual) ||
        check(TokenType::StarEqual) ||
        check(TokenType::SlashEqual) ||
        check(TokenType::PercentEqual)
    )
    {
        advance();

        TokenType assignmentType =
            previous().type;

        auto value =
            parseAssignment();

        // Check for member assignment: obj.prop = val
        auto memberExpr =
            dynamic_cast<MemberExpression*>(
                expression.get());

        if (memberExpr &&
            assignmentType == TokenType::Assign)
        {
            // Transfer ownership carefully
            auto obj = std::move(
                memberExpr->object);
            std::string prop =
                memberExpr->property;

            return std::make_unique<
                MemberAssignmentExpression>(
                    std::move(obj),
                    prop,
                    std::move(value));
        }

        // Check for index assignment: arr[i] = val
        auto indexExpr =
            dynamic_cast<IndexExpression*>(
                expression.get());

        if (indexExpr &&
            assignmentType == TokenType::Assign)
        {
            auto obj = std::move(
                indexExpr->object);
            auto idx = std::move(
                indexExpr->index);

            return std::make_unique<
                IndexAssignmentExpression>(
                    std::move(obj),
                    std::move(idx),
                    std::move(value));
        }

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
            op = "+";
        else if (assignmentType == TokenType::MinusEqual)
            op = "-";
        else if (assignmentType == TokenType::StarEqual)
            op = "*";
        else if (assignmentType == TokenType::SlashEqual)
            op = "/";
        else if (assignmentType == TokenType::PercentEqual)
            op = "%";

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

std::unique_ptr<Expression> Parser::parseTernary()
{
    auto expression = parseLogical();

    if (match(TokenType::QuestionMark))
    {
        auto consequent = parseAssignment();

        if (!match(TokenType::Colon))
        {
            return nullptr;
        }

        auto alternate = parseAssignment();

        return std::make_unique<
            TernaryExpression>(
                std::move(expression),
                std::move(consequent),
                std::move(alternate)
            );
    }

    return expression;
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

std::unique_ptr<Expression> Parser::parseUnary()
{
    // Prefix ++
    if (match(TokenType::PlusPlus))
    {
        auto operand = parsePostfix();

        return std::make_unique<
            UnaryExpression>(
                "++",
                std::move(operand),
                true
            );
    }

    // Prefix --
    if (match(TokenType::MinusMinus))
    {
        auto operand = parsePostfix();

        return std::make_unique<
            UnaryExpression>(
                "--",
                std::move(operand),
                true
            );
    }

    // Logical NOT
    if (match(TokenType::LogicalNot))
    {
        auto operand = parseUnary();

        return std::make_unique<
            UnaryExpression>(
                "!",
                std::move(operand),
                true
            );
    }

    // Unary minus
    if (match(TokenType::Minus))
    {
        auto operand = parseUnary();

        return std::make_unique<
            UnaryExpression>(
                "-",
                std::move(operand),
                true
            );
    }

    // typeof
    if (match(TokenType::Typeof))
    {
        auto operand = parseUnary();

        return std::make_unique<
            UnaryExpression>(
                "typeof",
                std::move(operand),
                true
            );
    }

    auto expression = parsePostfix();

    // Postfix ++
    if (match(TokenType::PlusPlus))
    {
        return std::make_unique<
            UnaryExpression>(
                "++",
                std::move(expression),
                false
            );
    }

    // Postfix --
    if (match(TokenType::MinusMinus))
    {
        return std::make_unique<
            UnaryExpression>(
                "--",
                std::move(expression),
                false
            );
    }

    return expression;
}

// ========== Postfix: member access, calls, index ==========

std::unique_ptr<Expression> Parser::parsePostfix()
{
    auto expression = parsePrimary();

    while (true)
    {
        if (match(TokenType::Dot))
        {
            if (!match(TokenType::Identifier))
            {
                return nullptr;
            }

            std::string propName =
                previous().value;

            if (check(TokenType::LeftParen))
            {
                // Method call: expr.method(args)
                advance(); // consume (

                auto call = std::make_unique<
                    CallExpression>(
                        std::make_unique<
                            MemberExpression>(
                                std::move(expression),
                                propName));

                while (
                    !check(TokenType::RightParen)
                    && !isAtEnd()
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

                    if (!check(
                        TokenType::RightParen))
                    {
                        match(TokenType::Comma);
                    }
                }

                match(TokenType::RightParen);

                expression = std::move(call);
            }
            else
            {
                // Property access: expr.prop
                expression =
                    std::make_unique<
                        MemberExpression>(
                            std::move(expression),
                            propName);
            }
        }
        else if (match(TokenType::LeftBracket))
        {
            // Index access: expr[index]
            auto index = parseExpression();

            if (!match(TokenType::RightBracket))
            {
                return nullptr;
            }

            expression =
                std::make_unique<
                    IndexExpression>(
                        std::move(expression),
                        std::move(index));
        }
        else if (check(TokenType::LeftParen))
        {
            // Direct function call: expr(args)
            // Only if expr is an Identifier
            auto id = dynamic_cast<
                Identifier*>(
                    expression.get());

            if (!id) break;

            advance(); // consume (

            auto call = std::make_unique<
                CallExpression>(
                    std::move(expression));

            while (
                !check(TokenType::RightParen)
                && !isAtEnd()
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

                if (!check(
                    TokenType::RightParen))
                {
                    match(TokenType::Comma);
                }
            }

            match(TokenType::RightParen);

            expression = std::move(call);
        }
        else
        {
            break;
        }
    }

    return expression;
}

// ========== Primary ==========

std::unique_ptr<Expression> Parser::parsePrimary()
{
    // Array literal
    if (check(TokenType::LeftBracket))
    {
        return parseArrayLiteral();
    }

    // Object literal
    if (check(TokenType::LeftBrace))
    {
        return parseObjectLiteral();
    }

    // Function expression
    if (check(TokenType::Function))
    {
        return parseFunctionExpression();
    }

    // Number
    if (match(TokenType::Number))
    {
        double value =
            std::stod(previous().value);

        return std::make_unique<NumberLiteral>(
            value
        );
    }

    // String
    if (match(TokenType::String))
    {
        return std::make_unique<StringLiteral>(
            previous().value
        );
    }

    // Boolean
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

    // Null
    if (match(TokenType::Null))
    {
        return std::make_unique<NullLiteral>();
    }

    // Undefined
    if (match(TokenType::Undefined))
    {
        return std::make_unique<UndefinedLiteral>();
    }

    // Identifier
    if (match(TokenType::Identifier))
    {
        return std::make_unique<
            Identifier>(
                previous().value
            );
    }

    // Parenthesized expression
    if (match(TokenType::LeftParen))
    {
        auto expr = parseExpression();

        if (!match(TokenType::RightParen))
        {
            return nullptr;
        }

        return expr;
    }

    return nullptr;
}

// ========== Array Literal ==========

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
        && !isAtEnd()
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

// ========== Object Literal ==========

std::unique_ptr<Expression> Parser::parseObjectLiteral()
{
    if (!match(TokenType::LeftBrace))
    {
        return nullptr;
    }

    auto obj = std::make_unique<ObjectLiteral>();

    while (
        !check(TokenType::RightBrace)
        && !isAtEnd()
    )
    {
        // Key can be identifier or string
        std::string key;

        if (match(TokenType::Identifier))
        {
            key = previous().value;
        }
        else if (match(TokenType::String))
        {
            key = previous().value;
        }
        else
        {
            return nullptr;
        }

        if (!match(TokenType::Colon))
        {
            return nullptr;
        }

        auto value = parseExpression();

        if (!value)
        {
            return nullptr;
        }

        obj->keys.push_back(key);
        obj->values.push_back(
            std::move(value));

        if (!check(TokenType::RightBrace))
        {
            match(TokenType::Comma);
        }
    }

    match(TokenType::RightBrace);

    return obj;
}

// ========== Arrow Function ==========

std::unique_ptr<Expression> Parser::parseArrowFunction(
    std::vector<std::string> params)
{
    auto arrow = std::make_unique<
        ArrowFunctionExpression>();

    arrow->parameters = std::move(params);

    // Block body: (...) => { ... }
    if (check(TokenType::LeftBrace))
    {
        arrow->body = parseBlockStatement();
    }
    else
    {
        // Expression body: (...) => expr
        arrow->expressionBody =
            parseAssignment();
    }

    return arrow;
}

// ========== Function Expression ==========

std::unique_ptr<Expression> Parser::parseFunctionExpression()
{
    if (!match(TokenType::Function))
    {
        return nullptr;
    }

    auto funcExpr = std::make_unique<
        FunctionExpression>();

    // Optional name
    if (match(TokenType::Identifier))
    {
        funcExpr->name = previous().value;
    }

    if (!match(TokenType::LeftParen))
    {
        return nullptr;
    }

    while (!check(TokenType::RightParen))
    {
        if (!match(TokenType::Identifier))
        {
            return nullptr;
        }

        funcExpr->parameters.push_back(
            previous().value);

        if (!check(TokenType::RightParen))
        {
            match(TokenType::Comma);
        }
    }

    match(TokenType::RightParen);

    funcExpr->body = parseBlockStatement();

    return funcExpr;
}

// ========== Statement Parsing ==========

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
        // Declaration without initializer
        match(TokenType::Semicolon);
        return std::make_unique<
            VariableDeclaration>(
                isConst,
                name,
                std::make_unique<UndefinedLiteral>()
            );
    }

    auto initializer = parseExpression();

    if (!initializer)
    {
        return nullptr;
    }

    match(TokenType::Semicolon);

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

std::unique_ptr<DoWhileStatement> Parser::parseDoWhileStatement()
{
    if (!match(TokenType::Do))
    {
        return nullptr;
    }

    auto body = parseBlockStatement();

    if (!body)
    {
        return nullptr;
    }

    if (!match(TokenType::While))
    {
        return nullptr;
    }

    if (!match(TokenType::LeftParen))
    {
        return nullptr;
    }

    auto condition = parseExpression();

    if (!condition)
    {
        return nullptr;
    }

    if (!match(TokenType::RightParen))
    {
        return nullptr;
    }

    match(TokenType::Semicolon);

    return std::make_unique<
        DoWhileStatement>(
            std::move(condition),
            std::move(body)
        );
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

    // Handle return without value
    if (check(TokenType::Semicolon) ||
        check(TokenType::RightBrace))
    {
        match(TokenType::Semicolon);
        return std::make_unique<
            ReturnStatement>(
                std::make_unique<
                    UndefinedLiteral>()
            );
    }

    auto value =
        parseExpression();

    if (!value)
    {
        return nullptr;
    }

    match(TokenType::Semicolon);

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

    if (check(TokenType::Do))
    {
        return parseDoWhileStatement();
    }

    if (check(TokenType::Function))
    {
        // Distinguish declaration vs expression
        // If function is at statement level and
        // followed by identifier, it's a declaration
        if (current + 1 < tokens.size() &&
            tokens[current + 1].type ==
                TokenType::Identifier)
        {
            return parseFunctionDeclaration();
        }
    }

    if (check(TokenType::Return))
    {
        return parseReturnStatement();
    }

    if (check(TokenType::Break))
    {
        advance();
        match(TokenType::Semicolon);
        return std::make_unique<
            BreakStatement>();
    }

    if (check(TokenType::Continue))
    {
        advance();
        match(TokenType::Semicolon);
        return std::make_unique<
            ContinueStatement>();
    }

    auto expression =
        parseExpression();

    if (!expression)
    {
        return nullptr;
    }

    match(TokenType::Semicolon);

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
