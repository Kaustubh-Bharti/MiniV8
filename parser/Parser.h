#pragma once

#include <vector>

#include "../lexer/Lexer.h"
#include "../ast/AST.h"

class Parser
{
private:
    std::vector<Token> tokens;
    size_t current;

public:
    explicit Parser(
        const std::vector<Token>& tokens);

    bool isAtEnd() const;

    const Token& peek() const;

    const Token& previous() const;

    const Token& advance();

    bool check(TokenType type) const;

    bool match(TokenType type);

    // Expression parsing (precedence climbing)
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseAssignment();
    std::unique_ptr<Expression> parseTernary();
    std::unique_ptr<Expression> parseLogical();
    std::unique_ptr<Expression> parseEquality();
    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parseTerm();
    std::unique_ptr<Expression> parseFactor();
    std::unique_ptr<Expression> parseUnary();
    std::unique_ptr<Expression> parsePostfix();
    std::unique_ptr<Expression> parsePrimary();

    // Specialized parsers
    std::unique_ptr<Expression> parseArrayLiteral();
    std::unique_ptr<Expression> parseObjectLiteral();
    std::unique_ptr<Expression> parseArrowFunction(
        std::vector<std::string> params);
    std::unique_ptr<Expression> parseFunctionExpression();

    // Statement parsing
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Program> parseProgram();
    std::unique_ptr<VariableDeclaration> parseVariableDeclaration();
    std::unique_ptr<VariableDeclaration> parseForVariableDeclaration();
    std::unique_ptr<IfStatement> parseIfStatement();
    std::unique_ptr<WhileStatement> parseWhileStatement();
    std::unique_ptr<ForStatement> parseForStatement();
    std::unique_ptr<FunctionDeclaration> parseFunctionDeclaration();
    std::unique_ptr<ReturnStatement> parseReturnStatement();
    std::unique_ptr<BlockStatement> parseBlockStatement();
    std::unique_ptr<DoWhileStatement> parseDoWhileStatement();

    // Helper: check if tokens starting at pos form an arrow function
    bool isArrowFunction() const;
};