#include "Lexer.hpp"

enum AstType
{
    AST_TYPE_EXPR,
    AST_TYPE_NUMBER,
    AST_TYPE_BIN_OP,
    AST_TYPE_IDENT,
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

    AstNumber(float value) : type(AST_TYPE_NUMBER), value(value) {}
};

struct AstIdent
{
    AstType type;
    string name;

    AstIdent(string name) : type(AST_TYPE_IDENT), name(name) {}
};

struct AstBinaryOp
{
    AstType type;
    AstBinOpType op_type;
    AstExpr *left;
    AstExpr *right;

    AstBinaryOp(TokenType type) : type(AST_TYPE_BIN_OP)
    {
        switch (type)
        {
        case TOKEN_ADD:
            op_type = AST_BIN_OP_TYPE_ADD;
            break;
        case TOKEN_SUB:
            op_type = AST_BIN_OP_TYPE_SUB;
            break;
        case TOKEN_MUL:
            op_type = AST_BIN_OP_TYPE_MUL;
            break;
        case TOKEN_DIV:
            op_type = AST_BIN_OP_TYPE_DIV;
            break;
        default:
            // error("Unknown bin op type");
            break;
        }
    }
};

int precedence(AstBinOpType type)
{
    switch (type)
    {
    case AST_BIN_OP_TYPE_ADD:
    case AST_BIN_OP_TYPE_SUB:
        return 1;

    case AST_BIN_OP_TYPE_MUL:
    case AST_BIN_OP_TYPE_DIV:
        return 2;

    default:
        -1;
    }
}