#include "Ast.hpp"
#include "Lexer.hpp"
#include <iostream>
#include <assert.h>

/*


TODO
---------
- Better whitespace
- Nicer way of handling bin ops
- Better numbers floats
- More maths operators ^
- Uniary operators + - 
- Maths functions sin cos tan log ln

*/

using namespace std;

void error(string msg)
{
    cerr << "Error: ";
    cerr << msg << endl;
    exit(1);
}

class Parser
{
    vector<Token> input;
    int size = 0;
    int i = 0;

    bool needRotate(AstBinOpType root, AstBinOpType right)
    {
        return precedence(root) >= precedence(right);
    }

public:
    Parser(vector<Token> input) : i(0), input(input), size(input.size()) {}

    void reset() { i = 0; }
    bool isEnd() { return i == size; }

    Token consume() { return input[i++]; }
    Token peek() { return input[i]; }

    bool peekIs(TokenType type)
    {
        Token t = peek();
        return t.type == type;
    }

    bool peekBinOp()
    {
        Token t = peek();
        switch (t.type)
        {
        case TOKEN_ADD:
        case TOKEN_SUB:
        case TOKEN_MUL:
        case TOKEN_DIV:
            return true;
        default:
            return false;
        }
    }

    void consumeWhiteSpace()
    {
        Token t = peek();
        while (t.type == TOKEN_WHITESPACE)
        {
            t = consume();
        }
    }

    AstExpr *parse() { return parseExpr(); }

    AstNumber *parseNumber()
    {
        Token t = consume();
        assert(t.type == TOKEN_NUMBER_LITERAL);
        float value = std::stof(t.source);

        AstNumber *a = new AstNumber(value);
        return a;
    }

    AstIdent *parseIdent()
    {
        Token t = consume();
        assert(t.type == TOKEN_IDENT);
        AstIdent *a = new AstIdent(t.source);
        return a;
    }

    AstExpr *parseExpr()
    {
        consumeWhiteSpace();
        AstExpr *left;
        if (peekIs(TOKEN_NUMBER_LITERAL))
        {
            left = (AstExpr *)parseNumber();
        }
        else if (peekIs(TOKEN_IDENT))
        {
            left = (AstExpr *)parseIdent();
        }
        else
        {
            error("Unexpected character, expecting digit");
        }

        consumeWhiteSpace();
        if (!peekBinOp())
        {
            return (AstExpr *)left;
        }

        Token bin_op_token = consume();
        AstBinaryOp *bin_op = new AstBinaryOp(bin_op_token.type);

        consumeWhiteSpace();
        AstExpr *right = parseExpr();
        bin_op->left = (AstExpr *)left;
        bin_op->right = right;

        AstBinaryOp *root = bin_op;

        if (right->type == AST_TYPE_BIN_OP)
        {
            AstBinaryOp *right_bin_op = ((AstBinaryOp *)right);
            if (needRotate(bin_op->op_type, right_bin_op->op_type))
            {
                bin_op->right = right_bin_op->left;
                right_bin_op->left = (AstExpr *)bin_op;
                root = right_bin_op;
            }
        }

        return (AstExpr *)root;
    }
};