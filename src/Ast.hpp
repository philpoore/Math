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
    AST_BIN_OP_TYPE_ADD = 10,
    AST_BIN_OP_TYPE_SUB = 11,
    AST_BIN_OP_TYPE_MUL = 12,
    AST_BIN_OP_TYPE_DIV = 13,
};

struct AstExpr
{
    AstType type;
};

struct AstNumber
{
    AstType type;
    double value;

    AstNumber(double value) : type(AST_TYPE_NUMBER), value(value) {}
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

string to_string(AstNumber *ast)
{
    return "AstNumber(" + std::to_string(ast->value) + ")";
}

string to_string(AstIdent *ast)
{
    return "AstIdent(" + ast->name + ")";
}

string to_string(AstExpr *ast);

string to_string(AstBinaryOp *ast)
{
    return "AstBinaryOp(op_type=" + std::to_string(ast->op_type) + " left=" + to_string(ast->left) + " right=" + to_string(ast->right) + ")";
}

string to_string(AstExpr *ast)
{
    switch (ast->type)
    {
    case AST_TYPE_EXPR:
        return "AstExpr";
    case AST_TYPE_NUMBER:
        return to_string((AstNumber *)ast);
    case AST_TYPE_IDENT:
        return to_string((AstIdent *)ast);
    case AST_TYPE_BIN_OP:
        return to_string((AstBinaryOp *)ast);
    }
    return "AstExpr";
}

string to_expr_string(AstNumber *ast)
{
    return std::to_string(ast->value);
}

string to_expr_string(AstIdent *ast)
{
    return ast->name;
}

string to_expr_string(AstExpr *ast);
string to_expr_string(AstBinOpType type);

string to_expr_string(AstBinaryOp *ast)
{
    return "(" + to_expr_string(ast->left) + to_expr_string(ast->op_type) + to_expr_string(ast->right) + ")";
}

string to_expr_string(AstExpr *ast)
{
    switch (ast->type)
    {
    case AST_TYPE_EXPR:
        return "AstExpr";
    case AST_TYPE_NUMBER:
        return to_expr_string((AstNumber *)ast);
    case AST_TYPE_IDENT:
        return to_expr_string((AstIdent *)ast);
    case AST_TYPE_BIN_OP:
        return to_expr_string((AstBinaryOp *)ast);
    }
    return "AstExpr";
}

string to_expr_string(AstBinOpType type)
{
    switch (type)
    {
    case AST_BIN_OP_TYPE_ADD:
        return "+";
    case AST_BIN_OP_TYPE_SUB:
        return "-";
    case AST_BIN_OP_TYPE_MUL:
        return "*";
    case AST_BIN_OP_TYPE_DIV:
        return "/";
    }
    return "AstBinOpType";
}

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