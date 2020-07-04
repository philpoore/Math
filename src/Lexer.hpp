#pragma once
#include <assert.h>

#include <map>
#include <string>
#include <vector>

#include "Token.hpp"

class Lexer {
    std::string input;
    int i = 0;
    int size = 0;
    Loc loc;

   public:
    Lexer(std::string input) : input(input), i(0), size(input.size()) {}

    char peek(int d = 0) { return input[i + d]; }

    void advance() {
        loc.col++;
        i++;
    }

    void advanceRow() {
        loc.col = 1;
        loc.row++;
    }

    Token createToken(TokenType t) {
        Token token = {t};
        assert((int)token.type >= 0);
        token.loc = loc;
        return token;
    }

    std::vector<Token> tokenize();
};