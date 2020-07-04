#pragma once

#include <map>
#include <string>

enum TokenType {
    TOKEN_IDENT = 257,
    TOKEN_NUMBER_LITERAL,
    TOKEN_EQ,
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_MOD,
    TOKEN_EXP,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_SEMI,
    TOKEN_WHITESPACE,
    TOKEN_EOF,
};

std::map<TokenType, std::string> tokenTypeStringMap = {
    {TOKEN_IDENT, "IDENT"},
    {TOKEN_NUMBER_LITERAL, "NUMBER_LITERAL"},
    {TOKEN_EQ, "EQ"},
    {TOKEN_ADD, "ADD"},
    {TOKEN_SUB, "SUB"},
    {TOKEN_MUL, "MUL"},
    {TOKEN_DIV, "DIV"},
    {TOKEN_MOD, "MOD"},
    {TOKEN_EXP, "EXP"},
    {TOKEN_LPAREN, "LPAREN"},
    {TOKEN_RPAREN, "RPAREN"},
    {TOKEN_SEMI, "SEMI"},
    {TOKEN_WHITESPACE, "WHITESPACE"},
    {TOKEN_EOF, "EOF"},
};

const std::map<char, TokenType> tokenTypeMap = {
    {'+', TOKEN_ADD},    {'-', TOKEN_SUB},    {'*', TOKEN_MUL},
    {'/', TOKEN_DIV},    {'%', TOKEN_MOD},

    {'(', TOKEN_LPAREN}, {')', TOKEN_RPAREN}, {'=', TOKEN_EQ},
    {';', TOKEN_SEMI},
};

std::string to_string(TokenType t);

typedef struct Loc {
    int row = 0;
    int col = 0;
} Loc;

std::string to_string(Loc loc) {
    return "(" + std::to_string(loc.row) + "," + std::to_string(loc.col) + ")";
}

struct Token {
    TokenType type;
    std::string source;

    Loc loc;

    std::string toString();
    bool isBinOp();
};

std::string to_string(Token t) { return t.toString(); }

std::string Token::toString() {
    return "Token: type=" + to_string(type) + " source='" + source + "'" +
           " loc=" + to_string(loc);
}

std::string to_string(TokenType t) {
    return tokenTypeStringMap.count(t) ? tokenTypeStringMap.at(t)
                                       : std::to_string((int)t);
}

bool Token::isBinOp() {
    switch (type) {
        case TOKEN_ADD:
        case TOKEN_SUB:
        case TOKEN_MUL:
        case TOKEN_DIV:
        case TOKEN_MOD:
        case TOKEN_EXP:
            return true;
        default:
            return false;
    }
}