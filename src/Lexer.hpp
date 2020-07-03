#pragma once
#include <string>
#include <vector>
#include <assert.h>

enum TokenType
{
    TOKEN_IDENT = 257,
    TOKEN_NUMBER_LITERAL,
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_WHITESPACE,
    TOKEN_EOF,
};

std::string to_string(TokenType t);

struct Token
{
    TokenType type;
    std::string source;

    int row = 0;
    int col = 0;

    std::string toString();
};

std::string to_string(Token t)
{
    return t.toString();
}

std::string Token::toString()
{
    return "Token: type=" + to_string(type) + " source='" + source + "'" + " row=" + std::to_string(row) + " col=" + std::to_string(col);
}

std::string to_string(TokenType t)
{
    switch (t)
    {
    case TOKEN_IDENT:
        return "IDENT";
    case TOKEN_NUMBER_LITERAL:
        return "NUMBER_LITERAL";
    case TOKEN_ADD:
        return "ADD";
    case TOKEN_SUB:
        return "SUB";
    case TOKEN_MUL:
        return "MUL";
    case TOKEN_DIV:
        return "DIV";
    case TOKEN_WHITESPACE:
        return "WHITESPACE";
    case TOKEN_EOF:
        return "EOF";
    default:
        return std::to_string((int)t);
    }
}

class Lexer
{
    std::string input;
    int i = 0;
    int size = 0;
    int row = 0;
    int col = 0;

public:
    Lexer(std::string input) : input(input), i(0), size(input.size()), row(1), col(1) {}

    char peek() { return input[i]; }

    void advance()
    {
        col++;
        i++;
    }

    void advanceRow()
    {
        col = 1;
        row++;
    }

    Token createToken(TokenType t)
    {
        Token token = {t};
        assert((int)token.type >= 0);
        token.col = col;
        token.row = row;
        return token;
    }

    std::vector<Token> tokenize();
};