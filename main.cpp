#include <iostream>
#include <string.h>

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

struct AstExpr
{
    AstType type;
};

struct AstNumber
{
    AstType type;
    int value;
};

struct AstBinaryOp
{
    AstType type;
    AstBinOpType op_type;
    AstExpr *left;
    AstExpr *right;
};

typedef struct AstNumber AstNumber;

AstNumber *createNumber(int value)
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

int eval(AstExpr *expr)
{
    if (expr->type == AST_TYPE_NUMBER)
    {
        return ((AstNumber *)expr)->value;
    }
    else if (expr->type == AST_TYPE_BIN_OP)
    {
        AstBinaryOp *bin_op_expr = (AstBinaryOp *)expr;
        if (bin_op_expr->op_type == AST_BIN_OP_TYPE_ADD)
        {
            return eval(bin_op_expr->left) + eval(bin_op_expr->right);
        }
        else if (bin_op_expr->op_type == AST_BIN_OP_TYPE_SUB)
        {
            return eval(bin_op_expr->left) - eval(bin_op_expr->right);
        }
        else if (bin_op_expr->op_type == AST_BIN_OP_TYPE_MUL)
        {
            return eval(bin_op_expr->left) * eval(bin_op_expr->right);
        }
        else if (bin_op_expr->op_type == AST_BIN_OP_TYPE_DIV)
        {
            return eval(bin_op_expr->left) / eval(bin_op_expr->right);
        }
        else
        {
            error("Unknown binary expression type: " + bin_op_expr->op_type);
        }
    }
    else
    {
        error("Unknown expression type: " + expr->type);
    }
}

class Parser
{
    char *buff;
    int size = 0;
    int i = 0;

public:
    Parser(string input)
    {
        size = input.size();
        buff = (char *)malloc(sizeof(char) * size);
        memcpy(buff, input.c_str(), size);
    }

    ~Parser()
    {
        free(buff);
    }

    void reset() { i = 0; }

    char peek()
    {
        return buff[i];
    }

    bool peekDigit()
    {
        char c = peek();
        return c >= '0' && c <= '9';
    }

    bool peekBinOp()
    {
        char c = peek();
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    char consume()
    {
        return buff[i++];
    }

    void consumeWhiteSpace()
    {
        char c = peek();
        //@TODO: better whitespace
        while (c == ' ' || c == '\t')
        {
            i++;
            c = peek();
        }
    }

    bool isEnd()
    {
        return i == size;
    }

    AstNumber *parseNumber()
    {
        int value = 0;
        while (peekDigit())
        {
            int digit = consume() - '0';
            value *= 10;
            value += digit;
        }

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

        char bin_op_char = consume();
        if (bin_op_char == '+')
        {
            bin_op = createBinOp(AST_BIN_OP_TYPE_ADD);
        }
        else if (bin_op_char == '-')
        {
            bin_op = createBinOp(AST_BIN_OP_TYPE_SUB);
        }
        else if (bin_op_char == '*')
        {
            bin_op = createBinOp(AST_BIN_OP_TYPE_MUL);
        }
        else if (bin_op_char == '/')
        {
            bin_op = createBinOp(AST_BIN_OP_TYPE_DIV);
        }
        else
        {
            error("Unexpected character, expecting binary operator :" + bin_op_char);
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

int main(int argc, char **argv)
{
    // AstBinaryOp *c = (AstBinaryOp *)createBinOp(AST_BIN_OP_TYPE_ADD, createNumber(10), createNumber(12));
    // AstBinaryOp *d = (AstBinaryOp *)createBinOp(AST_BIN_OP_TYPE_ADD, createNumber(30), (AstExpr *)c);

    // cout << "value: " << eval((AstExpr *)d) << endl;

    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " [expr]" << endl;
        exit(1);
    }

    string input = string(argv[1]);
    Parser p = Parser(input);
    AstExpr *a = p.parse();
    cout << eval(a) << endl;
}