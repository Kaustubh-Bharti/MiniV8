#include "Lexer.h"

Lexer::Lexer(const std::string& source)
    : source(source), current(0)
{
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while (!isAtEnd())
    {
        char c = advance();

        if (c == '"')
        {
            std::string value = readString();

            tokens.emplace_back(
                TokenType::String,
                value
            );

            continue;
        }

        if (isDigit(c))
        {
            current--;

            std::string number = readNumber();

            tokens.emplace_back(TokenType::Number, number);

            continue;
        }

        if (isAlpha(c))
        {
            current--;

            std::string identifier = readIdentifier();

            if (identifier == "let")
                tokens.emplace_back(TokenType::Let, identifier);

            else if (identifier == "const")
                tokens.emplace_back(TokenType::Const, identifier);

            else if (identifier == "function")
                tokens.emplace_back(TokenType::Function, identifier);

            else if (identifier == "if")
                tokens.emplace_back(TokenType::If, identifier);

            else if (identifier == "else")
                tokens.emplace_back(TokenType::Else, identifier);

            else if (identifier == "for")
                tokens.emplace_back(TokenType::For, identifier);

            else if (identifier == "while")
                tokens.emplace_back(TokenType::While, identifier);

            else if (identifier == "return")
                tokens.emplace_back(TokenType::Return, identifier);

            else if (identifier == "true")
                tokens.emplace_back(TokenType::True, identifier);

            else if (identifier == "false")
                tokens.emplace_back(TokenType::False, identifier);

            else if (identifier == "null")
                tokens.emplace_back(TokenType::Null, identifier);

            else if (identifier == "undefined")
                tokens.emplace_back(TokenType::Undefined, identifier);

            else if (identifier == "switch")
                tokens.emplace_back(TokenType::Switch, identifier);

            else if (identifier == "case")
                tokens.emplace_back(TokenType::Case, identifier);

            else if (identifier == "default")
                tokens.emplace_back(TokenType::Default, identifier);

            else if (identifier == "break")
                tokens.emplace_back(TokenType::Break, identifier);

            else if (identifier == "continue")
                tokens.emplace_back(TokenType::Continue, identifier);

            else if (identifier == "do")
                tokens.emplace_back(TokenType::Do, identifier);

            else if (identifier == "new")
                tokens.emplace_back(TokenType::New, identifier);

            else
                tokens.emplace_back(TokenType::Identifier, identifier);

            continue;
        }

        

        switch (c)
        {
        case ' ':
        case '\r':
        case '\t':
        case '\n':
            break;

        case '(':
            addToken(tokens, TokenType::LeftParen);
            break;

        case ')':
            addToken(tokens, TokenType::RightParen);
            break;

        case '{':
            addToken(tokens, TokenType::LeftBrace);
            break;

        case '}':
            addToken(tokens, TokenType::RightBrace);
            break;

        case '[':
            addToken(tokens, TokenType::LeftBracket);
            break;

        case ']':
            addToken(tokens, TokenType::RightBracket);
            break;

        case ';':
            addToken(tokens, TokenType::Semicolon);
            break;
        
        case ':':
            addToken(tokens, TokenType::Colon);
            break;
            
        case ',':
            addToken(tokens, TokenType::Comma);
            break;

        case '.':
        {
            if (match('.') && match('.'))
            {
                addToken(tokens, TokenType::Spread);
            }
            else
            {
                addToken(tokens, TokenType::Dot);
            }

            break;
        }

        case '+':
            if (match('+'))
                addToken(tokens, TokenType::Increment);
            else if (match('='))
                addToken(tokens, TokenType::PlusAssign);
            else
                addToken(tokens, TokenType::Plus);
            break;

        case '-':
            if (match('-'))
                addToken(tokens, TokenType::Decrement);
            else if (match('='))
                addToken(tokens, TokenType::MinusAssign);
            else
                addToken(tokens, TokenType::Minus);
            break;

        case '*':
            addToken(tokens, TokenType::Star);
            break;

        case '/':
        {
            if (match('/'))
            {
                while (!isAtEnd() &&
                    peek() != '\n')
                {
                    advance();
                }
            }
            else if (match('*'))
            {
                while (!isAtEnd())
                {
                    if (peek() == '*' &&
                        peekNext() == '/')
                    {
                        advance();
                        advance();
                        break;
                    }

                    advance();
                }
            }
            else
            {
                addToken(tokens, TokenType::Slash);
            }

            break;
        }

        case '%':
            addToken(tokens, TokenType::Percent);
            break;
        
        case '=':
            if (match('>'))
            {
                addToken(tokens, TokenType::Arrow);
            }
            else if (match('='))
            {
                if (match('='))
                    addToken(tokens, TokenType::StrictEqual);
                else
                    addToken(tokens, TokenType::Equal);
            }
            else
            {
                addToken(tokens, TokenType::Assign);
            }
            break;

        case '!':
            if (match('='))
            {
                if (match('='))
                    addToken(tokens, TokenType::StrictNotEqual);
                else
                    addToken(tokens, TokenType::NotEqual);
            }
            else
            {
                addToken(tokens, TokenType::LogicalNot);
            }
            break;

        case '<':
            if (match('='))
                addToken(tokens, TokenType::LessEqual);
            else
                addToken(tokens, TokenType::Less);
            break;

        case '>':
            if (match('='))
                addToken(tokens, TokenType::GreaterEqual);
            else
                addToken(tokens, TokenType::Greater);
            break;

        case '&':
            if (match('&'))
                addToken(tokens, TokenType::LogicalAnd);
            break;

        case '|':
            if (match('|'))
                addToken(tokens, TokenType::LogicalOr);
            break;

        default:
            break;
        }
    }

    tokens.emplace_back(TokenType::EndOfFile, "");

    return tokens;
}

bool Lexer::isAtEnd() const
{
    return current >= source.length();
}

char Lexer::peek() const
{
    if (isAtEnd())
    {
        return '\0';
    }

    return source[current];
}

char Lexer::peekNext() const
{
    if (current + 1 >= source.length())
    {
        return '\0';
    }

    return source[current + 1];
}

char Lexer::advance()
{
    return source[current++];
}

bool Lexer::match(char expected)
{
    if (isAtEnd())
    {
        return false;
    }

    if (source[current] != expected)
    {
        return false;
    }

    current++;

    return true;
}

void Lexer::addToken(std::vector<Token>& tokens,TokenType type)
{
    std::string value;

    switch(type)
    {
    case TokenType::Plus:
        value = "+";
        break;

    case TokenType::Minus:
        value = "-";
        break;

    case TokenType::Star:
        value = "*";
        break;

    case TokenType::Slash:
        value = "/";
        break;

    case TokenType::Percent:
        value = "%";
        break;

    case TokenType::Less:
        value = "<";
        break;

    case TokenType::LessEqual:
        value = "<=";
        break;

    case TokenType::Greater:
        value = ">";
        break;

    case TokenType::GreaterEqual:
        value = ">=";
        break;

    case TokenType::Equal:
        value = "==";
        break;

    case TokenType::StrictEqual:
        value = "===";
        break;

    case TokenType::NotEqual:
        value = "!=";
        break;

    case TokenType::StrictNotEqual:
        value = "!==";
        break;

    case TokenType::LogicalAnd:
        value = "&&";
        break;

    case TokenType::LogicalOr:
        value = "||";
        break;

    default:
        value = "";
        break;
    }

    tokens.emplace_back(type, value);
}

bool Lexer::isAlpha(char c) const
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Lexer::isAlphaNumeric(char c) const
{
    return isAlpha(c) ||
           (c >= '0' && c <= '9');
}

std::string Lexer::readIdentifier()
{
    std::string result;

    while (!isAtEnd() && isAlphaNumeric(peek()))
    {
        result += advance();
    }

    return result;
}

bool Lexer::isDigit(char c) const
{
    return c >= '0' && c <= '9';
}

std::string Lexer::readNumber()
{
    std::string result;

    while (!isAtEnd() && isDigit(peek()))
    {
        result += advance();
    }

    if (!isAtEnd() &&
        peek() == '.')
    {
        result += advance();

        while (!isAtEnd() &&
               isDigit(peek()))
        {
            result += advance();
        }
    }

    return result;
}

std::string Lexer::readString()
{
    std::string result;

    while (!isAtEnd() && peek() != '"')
    {
        result += advance();
    }

    if (!isAtEnd())
    {
        advance();
    }

    return result;
}