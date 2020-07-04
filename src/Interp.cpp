#include "Interp.hpp"

using namespace std;

void Interp::error(string msg) {
    cout << "Interpreter Error: " << msg << endl;
    exit(1);
}

double Interp::evalBinOp(AstBinaryOp* bin_op) {
    double left_value = eval(bin_op->left);
    double right_value = eval(bin_op->right);

    switch (bin_op->op_type) {
        case AST_BIN_OP_TYPE_ADD:
            return left_value + right_value;
        case AST_BIN_OP_TYPE_SUB:
            return left_value - right_value;
        case AST_BIN_OP_TYPE_MUL:
            return left_value * right_value;
        case AST_BIN_OP_TYPE_DIV:
            return left_value / right_value;
        case AST_BIN_OP_TYPE_MOD:
            return fmodl(left_value, right_value);
        case AST_BIN_OP_TYPE_EXP:
            return pow(left_value, right_value);
        default:
            error("Unknown binary expression type: ");  // +
                                                        // bin_op->op_type);
            return 0.0f;
    }
}

double Interp::evalIdent(AstIdent* ast) {
    if (variables.count(ast->name)) {
        return variables.at(ast->name);
    } else if (globals.count(ast->name)) {
        return globals.at(ast->name);
    } else {
        return 0.0;
    }
}

double Interp::evalFunctionCall(AstFunctionCall* ast) {
    if (!functions.count(ast->name)) {
        // @TODO: unknown function name
        return 0.0;
    }

    auto func = functions.at(ast->name);

    return func(eval(ast->arg));
}

double Interp::eval(AstExpr* expr) {
    switch (expr->type) {
        case AST_TYPE_NUMBER:
            return ((AstNumber*)expr)->value;
        case AST_TYPE_IDENT:
            return evalIdent((AstIdent*)expr);
        case AST_TYPE_BIN_OP:
            return evalBinOp((AstBinaryOp*)expr);
        case AST_TYPE_FUNCTION_CALL:
            return evalFunctionCall((AstFunctionCall*)expr);
        default:
            error(string("Unknown expression type: ") + to_string(expr->type));
            return 0.0;
    }
}

void Interp::exec(AstStmts* ast) {
    for (auto stmt : ast->stmts) {
        if (stmt->expr->type == AST_TYPE_ASSIGN) {
            AstAssign* assign = (AstAssign*)stmt->expr;
            double value = eval(assign->expr);
            string name = assign->name;
#ifdef DEBUG
            cerr << "--> setting variable: " + name + " to " + to_string(value)
                 << endl;
#endif
            variables[name] = value;
        } else {
            double value = eval(stmt->expr);
            cout << setprecision(16) << value << endl;
        }
    }
}