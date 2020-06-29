#include <iostream>
#include "src/Lexer.cpp"
#include "src/Parser.cpp"
#include "src/Interp.cpp"

using namespace std;

int main(int argc, char **argv)
{
    bool repl = argc == 1;

    Interp interp;
    Lexer lexer;

    if (!repl)
    {
        string input = string(argv[1]);
        lexer.setInput(input);
        Parser parser(lexer.tokenize());

        AstExpr *ast = parser.parse();

        cout << interp.eval(ast) << endl;
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

        lexer.setInput(input);
        Parser parser(lexer.tokenize());

        AstExpr *ast = parser.parse();

        cout << interp.eval(ast) << endl;
    }
}