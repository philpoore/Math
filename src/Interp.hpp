#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <vector>

#include "Ast.hpp"

using namespace std;

const map<string, double> globals = {
    {"PI", 3.141592},
    {"a", 110},
    {"phil", 2806},
};

const map<string, function<double(double)>> functions = {
    {"abs", fabsl},    {"floor", floorl}, {"ceil", ceill},   {"round", roundl},
    {"sqrt", sqrtl},   {"cbrt", cbrtl},   {"exp", expl},     {"exp2", exp2l},
    {"log", logl},     {"log10", log10l}, {"log2", log2l},   {"sin", sinl},
    {"cos", cosl},     {"tan", tanl},     {"asin", asinl},   {"acos", acosl},
    {"atan", atanl},   {"sinh", sinhl},   {"cosh", coshl},   {"tanh", tanhl},
    {"asinh", asinhl}, {"acosh", acoshl}, {"atanh", atanhl},
};

class Interp {
    map<string, double> variables;

    void error(string msg);

   public:
    double evalBinOp(AstBinaryOp* bin_op);
    double evalIdent(AstIdent* ast);
    double evalFunctionCall(AstFunctionCall* ast);
    double eval(AstExpr* expr);

    void exec(AstStmts* ast);
};