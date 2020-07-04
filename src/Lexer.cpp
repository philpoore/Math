#include "Lexer.hpp"

using namespace std;

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;

    while (i < size) {
        char a = peek();
        if (isalpha(a)) {
            // keyword
            // ident
            string ident = "";
            Token t = createToken(TOKEN_IDENT);
            while (isalpha(a) || isdigit(a)) {
                ident += a;
                advance();
                a = peek();
            }
            t.source = ident;
            tokens.push_back(t);
            continue;
        } else if (isdigit(a)) {
            Token t = createToken(TOKEN_NUMBER_LITERAL);
            // number literal
            string number = "";
            while (isdigit(a)) {
                number += a;
                advance();
                a = peek();
            }

            // decimal point
            if (a == '.') {
                number += a;
                advance();
                a = peek();
                while (isdigit(a)) {
                    number += a;
                    advance();
                    a = peek();
                }
            }

            t.source = number;
            tokens.push_back(t);
            continue;
        } else if (isspace(a)) {
            string whitespace;

            Token t = createToken(TOKEN_WHITESPACE);
            while (isspace(a)) {
                whitespace += a;
                if (a == '\n') {
                    advanceRow();
                }
                advance();
                a = peek();
            }
            t.source = whitespace;
            tokens.push_back(t);
            continue;
        } else if (a == '*' && peek(1) == '*') {
            Token t = createToken(TOKEN_EXP);
            t.source = "**";
            tokens.push_back(t);
            advance();
        } else {
            TokenType tt =
                tokenTypeMap.count(a) ? tokenTypeMap.at(a) : (TokenType)a;
            Token t = createToken(tt);
            t.source = a;
            tokens.push_back(t);
        }

        advance();
    }

    Token t = createToken(TOKEN_EOF);
    tokens.push_back(t);

    return tokens;
};