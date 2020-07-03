#include <map>

class Interp
{
    map<string, double> globals;

public:
    Interp()
    {
        globals["PI"] = 3.141592;
        globals["a"] = 110;
        globals["phil"] = 2806;
    }

    double evalBinOp(AstBinaryOp *bin_op)
    {
        switch (bin_op->op_type)
        {
        case AST_BIN_OP_TYPE_ADD:
            return eval(bin_op->left) + eval(bin_op->right);
        case AST_BIN_OP_TYPE_SUB:
            return eval(bin_op->left) - eval(bin_op->right);
        case AST_BIN_OP_TYPE_MUL:
            return eval(bin_op->left) * eval(bin_op->right);
        case AST_BIN_OP_TYPE_DIV:
            return eval(bin_op->left) / eval(bin_op->right);
        default:
            error("Unknown binary expression type: " + bin_op->op_type);
        }
    }

    double evalIdent(AstIdent *ast)
    {
        if (globals.count(ast->name))
        {
            return globals[ast->name];
        }
        else
        {
            return 0.0;
        }
    }

    double eval(AstExpr *expr)
    {
        switch (expr->type)
        {
        case AST_TYPE_NUMBER:
            return ((AstNumber *)expr)->value;
        case AST_TYPE_IDENT:
            return evalIdent((AstIdent *)expr);
        case AST_TYPE_BIN_OP:
            return evalBinOp((AstBinaryOp *)expr);
        default:
            error("Unknown expression type: " + expr->type);
        }
    }
};