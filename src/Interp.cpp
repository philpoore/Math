class Interp
{
public:
    float evalBinOp(AstBinaryOp *bin_op)
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

    float eval(AstExpr *expr)
    {
        if (expr->type == AST_TYPE_NUMBER)
        {
            return ((AstNumber *)expr)->value;
        }
        else if (expr->type == AST_TYPE_BIN_OP)
        {
            return evalBinOp((AstBinaryOp *)expr);
        }
        else
        {
            error("Unknown expression type: " + expr->type);
        }
    }
};