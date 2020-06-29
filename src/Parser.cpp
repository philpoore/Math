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

enum AstType
{
    AST_TYPE_EXPR,
    AST_TYPE_NUMBER,
    AST_TYPE_BIN_OP,
};

enum AstBinOpType
{
    AST_BIN_OP_TYPE_ADD,
    AST_BIN_OP_TYPE_SUB,
    AST_BIN_OP_TYPE_MUL,
    AST_BIN_OP_TYPE_DIV,
};

struct AstExpr
{
    AstType type;
};

struct AstNumber
{
    AstType type;
    float value;
};

struct AstBinaryOp
{
    AstType type;
    AstBinOpType op_type;
    AstExpr *left;
    AstExpr *right;
};

typedef struct AstNumber AstNumber;

int precedence(AstBinOpType type)
{
    if (type == AST_BIN_OP_TYPE_ADD || type == AST_BIN_OP_TYPE_SUB)
    {
        return 1;
    }
    else if (type == AST_BIN_OP_TYPE_MUL || type == AST_BIN_OP_TYPE_DIV)
    {
        return 2;
    }
    else
    {
        error("Unknown binary op type for precedence");
    }
}

bool needRotate(AstBinOpType root, AstBinOpType right)
{
    return precedence(root) >= precedence(right);
}

AstNumber *createNumber(float value)
{
    AstNumber *a = new AstNumber();
    a->type = AST_TYPE_NUMBER;
    a->value = value;
    return a;
};

AstBinaryOp *createBinOp(AstBinOpType op_type)
{
    AstBinaryOp *a = new AstBinaryOp();
    a->type = AST_TYPE_BIN_OP;
    a->op_type = op_type;
    return a;
};

AstBinaryOp *createBinOp(AstBinOpType op_type, AstExpr *left, AstExpr *right)
{
    AstBinaryOp *a = new AstBinaryOp();
    a->type = AST_TYPE_BIN_OP;
    a->op_type = op_type;
    a->left = left;
    a->right = right;
    return a;
};

class Parser
{
    vector<Token> input;
    int size = 0;
    int i = 0;

public:
    Parser(vector<Token> _input)
    {
        i = 0;
        size = _input.size();
        input = _input;
    }

    void reset() { i = 0; }

    Token peek()
    {
        return input[i];
    }

    bool peekDigit()
    {
        Token t = peek();
        return t.type == TOKEN_NUMBER_LITERAL;
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

    Token consume()
    {
        return input[i++];
    }

    void consumeWhiteSpace()
    {
        Token t = peek();
        while (t.type == TOKEN_WHITESPACE)
        {
            t = consume();
        }
    }

    bool isEnd()
    {
        return i == size;
    }

    AstNumber *parseNumber()
    {
        Token t = consume();
        assert(t.type == TOKEN_NUMBER_LITERAL);
        float value = std::stof(t.source);

        AstNumber *a = createNumber(value);
        return a;
    }

    AstExpr *parseExpr()
    {
        consumeWhiteSpace();
        if (!peekDigit())
        {
            error("Unexpected character, expecting digit");
        }

        AstNumber *left = parseNumber();
        consumeWhiteSpace();
        if (!peekBinOp())
        {
            return (AstExpr *)left;
        }

        AstBinaryOp *bin_op;

        Token bin_op_token = consume();
        if (bin_op_token.type == TOKEN_ADD)
        {
            bin_op = createBinOp(AST_BIN_OP_TYPE_ADD);
        }
        else if (bin_op_token.type == TOKEN_SUB)
        {
            bin_op = createBinOp(AST_BIN_OP_TYPE_SUB);
        }
        else if (bin_op_token.type == TOKEN_MUL)
        {
            bin_op = createBinOp(AST_BIN_OP_TYPE_MUL);
        }
        else if (bin_op_token.type == TOKEN_DIV)
        {
            bin_op = createBinOp(AST_BIN_OP_TYPE_DIV);
        }
        else
        {
            error("Unexpected character, expecting binary operator :" + bin_op_token.type);
        }
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

    AstExpr *parse()
    {
        return parseExpr();
    }
};