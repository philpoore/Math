#pragma once
#include <string>

#include "Lexer.hpp"

using namespace std;

enum AstType {
    AST_TYPE_STMT,
    AST_TYPE_ASSIGN,
    AST_TYPE_STMTS,
    AST_TYPE_EXPR,
    AST_TYPE_NUMBER,
    AST_TYPE_BIN_OP,
    AST_TYPE_IDENT,
    AST_TYPE_FUNCTION_CALL,
};

enum AstBinOpType {
    AST_BIN_OP_TYPE_UNKNOWN = -1,
    AST_BIN_OP_TYPE_ADD = 10,
    AST_BIN_OP_TYPE_SUB = 11,
    AST_BIN_OP_TYPE_MUL = 12,
    AST_BIN_OP_TYPE_DIV = 13,
    AST_BIN_OP_TYPE_MOD = 14,
    AST_BIN_OP_TYPE_EXP = 15,
};

AstBinOpType Token_to_bin_op(TokenType type);
int precedence(AstBinOpType type);

struct AstExpr {
    AstType type;
};

struct AstStmt {
    AstType type;
    AstExpr* expr;
    AstStmt() : type(AST_TYPE_STMT) {}
};

struct AstStmts {
    AstType type;
    vector<AstStmt*> stmts;
    AstStmts() : type(AST_TYPE_STMTS) {}
};

struct AstAssign {
    AstType type;
    string name;
    AstExpr* expr;
    AstAssign() : type(AST_TYPE_ASSIGN) {}
};

struct AstNumber {
    AstType type;
    double value;

    AstNumber(double value) : type(AST_TYPE_NUMBER), value(value) {}
};

struct AstIdent {
    AstType type;
    string name;

    AstIdent(string name) : type(AST_TYPE_IDENT), name(name) {}
};

struct AstBinaryOp {
    AstType type;
    AstBinOpType op_type;
    AstExpr* left;
    AstExpr* right;

    AstBinaryOp(TokenType type)
        : type(AST_TYPE_BIN_OP), op_type(Token_to_bin_op(type)) {}
};

struct AstFunctionCall {
    AstType type;
    string name;
    AstExpr* arg;

    AstFunctionCall(string name) : type(AST_TYPE_FUNCTION_CALL), name(name) {}
};

string to_string(AstNumber* ast);
string to_string(AstExpr* ast);
string to_string(AstStmt* ast);
string to_string(AstStmts* ast);
string to_string(AstAssign* ast);
string to_string(AstIdent* ast);
string to_string(AstBinaryOp* ast);
string to_string(AstFunctionCall* ast);

string to_string(AstNumber* ast) {
    return "AstNumber(" + std::to_string(ast->value) + ")";
}

string to_string(AstIdent* ast) { return "AstIdent(" + ast->name + ")"; }

string to_string(AstBinaryOp* ast) {
    return "AstBinaryOp(op_type=" + std::to_string(ast->op_type) +
           " left=" + to_string(ast->left) + " right=" + to_string(ast->right) +
           ")";
}

string to_string(AstFunctionCall* ast) {
    return "AstFunctionCall(name=" + ast->name + " arg=" + to_string(ast->arg) +
           ")";
}

string to_string(AstStmt* ast) {
    return "AstStmt(expr=" + to_string(ast->expr) + ")";
}

string to_string(AstStmts* ast) {
    int size = ast->stmts.size();
    string output = "AstStmts([";
    for (int i = 0; i < size; i++) {
        auto stmt = ast->stmts[i];
        output += to_string(stmt);
        if (i < size - 1) {
            output += ", ";
        }
    }
    output += "])";
    return output;
}

string to_string(AstAssign* ast) {
    return "AstAssign(name=" + ast->name + " expr=" + to_string(ast->expr) +
           ")";
}

string to_string(AstExpr* ast) {
    switch (ast->type) {
        case AST_TYPE_EXPR:
            return "AstExpr";
        case AST_TYPE_STMTS:
            return to_string((AstStmts*)ast);
        case AST_TYPE_STMT:
            return to_string((AstStmt*)ast);
        case AST_TYPE_ASSIGN:
            return to_string((AstAssign*)ast);
        case AST_TYPE_NUMBER:
            return to_string((AstNumber*)ast);
        case AST_TYPE_IDENT:
            return to_string((AstIdent*)ast);
        case AST_TYPE_BIN_OP:
            return to_string((AstBinaryOp*)ast);
        case AST_TYPE_FUNCTION_CALL:
            return to_string((AstFunctionCall*)ast);
    }
    return "AstExpr";
}

string to_expr_string(AstExpr* ast);
string to_expr_string(AstStmts* ast);
string to_expr_string(AstStmt* ast);
string to_expr_string(AstAssign* ast);
string to_expr_string(AstNumber* ast);
string to_expr_string(AstIdent* ast);
string to_expr_string(AstBinaryOp* ast);
string to_expr_string(AstFunctionCall* ast);
string to_expr_string(AstBinOpType type);

string to_expr_string(AstExpr* ast) {
    switch (ast->type) {
        case AST_TYPE_EXPR:
            return "AstExpr";
        case AST_TYPE_STMT:
            return to_expr_string((AstStmt*)ast);
        case AST_TYPE_STMTS:
            return to_expr_string((AstStmts*)ast);
        case AST_TYPE_ASSIGN:
            return to_expr_string((AstAssign*)ast);
        case AST_TYPE_NUMBER:
            return to_expr_string((AstNumber*)ast);
        case AST_TYPE_IDENT:
            return to_expr_string((AstIdent*)ast);
        case AST_TYPE_BIN_OP:
            return to_expr_string((AstBinaryOp*)ast);
        case AST_TYPE_FUNCTION_CALL:
            return to_expr_string((AstFunctionCall*)ast);
    }
    return "AstExpr";
}

string to_expr_string(AstStmts* ast) {
    string output;
    for (auto stmt : ast->stmts) {
        output += to_expr_string(stmt) + "\n";
    }
    return output;
}
string to_expr_string(AstStmt* ast) { return to_expr_string(ast->expr) + ";"; }
string to_expr_string(AstAssign* ast) {
    return ast->name + "=" + to_expr_string(ast->expr);
}

string to_expr_string(AstNumber* ast) { return std::to_string(ast->value); }

string to_expr_string(AstIdent* ast) { return ast->name; }

string to_expr_string(AstBinaryOp* ast) {
    return "(" + to_expr_string(ast->left) + to_expr_string(ast->op_type) +
           to_expr_string(ast->right) + ")";
}

string to_expr_string(AstFunctionCall* ast) {
    return ast->name + "(" + to_expr_string(ast->arg) + ")";
}

string to_expr_string(AstBinOpType type) {
    switch (type) {
        case AST_BIN_OP_TYPE_ADD:
            return "+";
        case AST_BIN_OP_TYPE_SUB:
            return "-";
        case AST_BIN_OP_TYPE_MUL:
            return "*";
        case AST_BIN_OP_TYPE_DIV:
            return "/";
        case AST_BIN_OP_TYPE_MOD:
            return "%";
        case AST_BIN_OP_TYPE_EXP:
            return "**";
        case AST_BIN_OP_TYPE_UNKNOWN:
            return "??";
    }
    return "AstBinOpType";
}

int precedence(AstBinOpType type) {
    switch (type) {
        case AST_BIN_OP_TYPE_ADD:
        case AST_BIN_OP_TYPE_SUB:
            return 1;

        case AST_BIN_OP_TYPE_MUL:
        case AST_BIN_OP_TYPE_DIV:
        case AST_BIN_OP_TYPE_MOD:
            return 2;

        case AST_BIN_OP_TYPE_EXP:
            return 3;

        default:
            return -1;
    }
}

AstBinOpType Token_to_bin_op(TokenType type) {
    switch (type) {
        case TOKEN_ADD:
            return AST_BIN_OP_TYPE_ADD;
        case TOKEN_SUB:
            return AST_BIN_OP_TYPE_SUB;
        case TOKEN_MUL:
            return AST_BIN_OP_TYPE_MUL;
        case TOKEN_DIV:
            return AST_BIN_OP_TYPE_DIV;
        case TOKEN_MOD:
            return AST_BIN_OP_TYPE_MOD;
        case TOKEN_EXP:
            return AST_BIN_OP_TYPE_EXP;
        default:
            return AST_BIN_OP_TYPE_UNKNOWN;
    }
}