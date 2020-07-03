#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>

#include "src/Lexer.cpp"
#include "src/Parser.cpp"
#include "src/Interp.cpp"

using namespace std;

Interp interp;

void run(string input)
{
    Lexer lexer(input);
    auto tokens = lexer.tokenize();

#ifdef DEBUG
    // Debug tokens
    for (auto token : tokens)
        cerr << to_string(token) << endl;
#endif

    Parser parser(tokens);

    AstExpr *ast = parser.parse();

#ifdef DEBUG
    // Debug AST
    cerr << to_string(ast) << endl;
    cerr << to_expr_string(ast) << endl;
#endif

    double value = interp.eval(ast);
    cout << setprecision(16) << value << endl;
}

int main(int argc, char **argv)
{
    bool repl = argc == 1;

    if (!repl)
    {
        string input = string(argv[1]);
        run(input);
        exit(0);
    }

    while (true)
    {
        string input;
        cout << "> ";
        cin >> input;

        if (input == "quit")
        {
            exit(0);
        }

        run(input);
    }
}