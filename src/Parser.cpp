#include "Parser.hpp"

#include <assert.h>

#include <iostream>

#include "Ast.hpp"
#include "Lexer.hpp"

/*


TODO
---------
[x] - Better whitespace
[x] - Nicer way of handling bin ops
[x] - Better numbers doubles
[x] - More maths operators
    [x] - MOD %
    [x] - EXP **
    [ ] - right assosiative operators
[ ] - Uniary operators + -
[x] - Maths functions sin cos tan log ln
[x] - statements
[x] - assignment
[ ] - strings
*/

using namespace std;

void Parser::error(string msg) {
    cerr << "Parser Error: " << msg << endl;
    exit(1);
}

void Parser::reset() { i = 0; }
bool Parser::isEnd() { return i == size; }

Token Parser::peek(int d = 0) { return input[i + d]; }
Token Parser::peekNonWhitespace(int d = 1) {
    return peek(nextNonWhitespaceOffset(d));
}
bool Parser::peekIs(TokenType type) {
    Token t = peek();
    return t.type == type;
}

int Parser::nextNonWhitespaceOffset(int d = 1) {
    Token t = peek(d);
    while (t.type == TOKEN_WHITESPACE) {
        d++;
        t = peek(d);
    }
    return d;
}

Token Parser::consume() { return input[i++]; }

Token Parser::expect(TokenType type, string msg = "") {
    Token t = peek();
    if (t.type != type) {
        string output = msg;
        if (output.size()) {
            output += ": ";
        }
        output +=
            "Expected token " + to_string(type) + " but found " + to_string(t);
        error(output);
        return Token();
    } else {
        return consume();
    }
}
void Parser::consumeWhiteSpace() {
    Token t = peek();
    while (t.type == TOKEN_WHITESPACE) {
        consume();
        t = peek();
    }
}

AstStmts* Parser::parse() { return parseStatements(); }

AstStmts* Parser::parseStatements() {
    AstStmts* ast = new AstStmts();
    while (!isEnd() && peek().type != TOKEN_EOF) {
        AstStmt* stmt = parseStatement();
        if (stmt != nullptr) {
            ast->stmts.push_back(stmt);
        }
    }
    return ast;
}

AstStmt* Parser::parseStatement() {
    consumeWhiteSpace();
    AstExpr* expr = nullptr;

    Token t = peek();
    // Assignment statement
    if (t.type == TOKEN_IF) {
        expr = (AstExpr*)parseIf();
    } else if (t.type == TOKEN_IDENT) {
        if (peekNonWhitespace(1).type == TOKEN_EQ) {
            // parse assignment statement
            Token t_ident = expect(TOKEN_IDENT);
            consumeWhiteSpace();

            expect(TOKEN_EQ);
            AstAssign* ast = new AstAssign();
            ast->name = t_ident.source;
            ast->expr = parseExpr();
            expr = (AstExpr*)ast;
        }
    } else if (peek().type == TOKEN_LCURLY) {
        // Block statement
        consume();
        expr = (AstExpr*)parseBlock();
        expect(TOKEN_RCURLY, "Unmatched curly brace, missing closing bracket.");
    }

    // default to parsing expressions.
    if (expr == nullptr) {
        expr = parseExpr();
    }

    if (expr == nullptr) {
        return nullptr;
    }

    consumeWhiteSpace();
    if (expr->type != AST_TYPE_BLOCK && expr->type != AST_TYPE_IF &&
        peek().type != TOKEN_EOF) {
        expect(TOKEN_SEMI, "Missing semi colon");
    }

    AstStmt* stmt = new AstStmt();
    stmt->expr = expr;
    return stmt;
}

AstIf* Parser::parseIf() {
    AstIf* ast = new AstIf();
    expect(TOKEN_IF);  // if
    consumeWhiteSpace();

    expect(TOKEN_LPAREN);  // if's test expr e.g (10 = 10)
    ast->expr = parseExpr();
    expect(TOKEN_RPAREN);

    consumeWhiteSpace();
    expect(TOKEN_LCURLY);  // if's if_branch
    ast->if_branch = parseBlock();
    expect(TOKEN_RCURLY);
    consumeWhiteSpace();

    if (peekIs(TOKEN_ELSE)) {
        // Else branch.
        expect(TOKEN_ELSE);
        consumeWhiteSpace();
        expect(TOKEN_LCURLY);  // if's else_branch
        ast->else_branch = parseBlock();
        expect(TOKEN_RCURLY);
    }

    return ast;
}

AstBlock* Parser::parseBlock() {
    AstBlock* ast = new AstBlock();
    while (!isEnd() && !peekIs(TOKEN_EOF) && !peekIs(TOKEN_RCURLY)) {
        AstStmt* stmt = parseStatement();
        if (stmt != nullptr) {
            ast->stmts.push_back(stmt);
        }
    }
    return ast;
}

AstNumber* Parser::parseNumber() {
    Token t = expect(TOKEN_NUMBER_LITERAL);
    double value = std::stod(t.source);

    AstNumber* a = new AstNumber(value);
    return a;
}

AstIdent* Parser::parseIdent() {
    Token t = expect(TOKEN_IDENT);
    AstIdent* a = new AstIdent(t.source);
    return a;
}

AstFunctionCall* Parser::parseFunctionCall() {
    Token t = expect(TOKEN_IDENT);
    AstFunctionCall* a = new AstFunctionCall(t.source);
    consumeWhiteSpace();

    expect(TOKEN_LPAREN);
    AstExpr* left = parseExpr();
    expect(TOKEN_RPAREN, "Unmatched parentheses");

    a->arg = left;
    return a;
}

AstExpr* Parser::parseSubExpr() {
    if (peekIs(TOKEN_NUMBER_LITERAL)) {
        return (AstExpr*)parseNumber();
    } else if (peekIs(TOKEN_IDENT)) {
        ////////////
        // Parse Function Call
        if (peekNonWhitespace(1).type == TOKEN_LPAREN) {
            return (AstExpr*)parseFunctionCall();
        }
        ////////////

        return (AstExpr*)parseIdent();
    } else {
        error("Unexpected character, expecting digit or ident found :" +
              to_string(peek()));
        return nullptr;
    }
}

AstExpr* Parser::parseExpr(int precedence_level) {
    AstExpr* left;
    consumeWhiteSpace();
    if (peekIs(TOKEN_EOF) || peekIs(TOKEN_RCURLY)) {
        return nullptr;
    } else if (peekIs(TOKEN_LPAREN)) {
        expect(TOKEN_LPAREN);
        left = parseExpr();
        expect(TOKEN_RPAREN, "Unmatched parentheses");
    } else {
        left = parseSubExpr();
    }

    while (true) {
        consumeWhiteSpace();
        Token t = peek();
        if (t.type == TOKEN_SEMI) {
            break;
        }
        if (!t.isBinOp()) {
            break;
        }

        int p = precedence(Token_to_bin_op(t.type));

        if (p <= precedence_level) {
            break;
        }

        Token bin_op_token = consume();
        AstBinaryOp* bin_op = new AstBinaryOp(bin_op_token.type);

        bin_op->left = left;
        bin_op->right = parseExpr(p);
        assert(bin_op->right != nullptr);
        left = (AstExpr*)bin_op;
    }
    return left;
}