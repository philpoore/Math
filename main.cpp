#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>

#include "src/Interp.cpp"
#include "src/Lexer.cpp"
#include "src/Parser.cpp"

using namespace std;

Interp interp;

// Take an input string and run it
void run(string input) {
    Lexer lexer(input);
    auto tokens = lexer.tokenize();

#ifdef DEBUG
    // Debug tokens
    for (auto token : tokens) cerr << to_string(token) << endl;
#endif

    Parser parser(tokens);

    AstStmts* ast = parser.parse();

#ifdef DEBUG
    // Debug AST
    cerr << to_string(ast) << endl;
    cerr << to_expr_string(ast) << endl;
#endif

    interp.exec(ast);
    // cout << setprecision(16) << value << endl;
}

// Run a REPL in a loop, quit with "quit".
void runRepl() {
    while (true) {
        string input;
        cout << "> ";
        cin >> input;

        if (input == "quit") {
            exit(0);
        }

        run(input);
    }
}

// Read a file compleatly and return it as one big string.
// @TODO: this is super silly with large files!
string readFile(string filename) {
    ifstream is(filename);
    if (!is) {
        cerr << "Runtime Error: Coudln't open file " << filename;
        cerr << " for reading" << endl;
        exit(1);
    }
    return string((std::istreambuf_iterator<char>(is)),
                  std::istreambuf_iterator<char>());
    ;
}

int main(int argc, char** argv) {
    bool repl = argc == 1;

    if (repl) {
        runRepl();
    }

    string input;
    string firstArg = string(argv[1]);

    if (firstArg == "-f") {
        // Use a file as input.
        // E.g "./math -f examples/example-1.math"
        if (argc < 3) {
            cerr << "Runtime Error: Too few arguments\n";
            cerr << "-f flag missing filename" << endl;
            exit(1);
        }
        string filename = string(argv[2]);
        // Read file for input
        input = readFile(filename);
    } else {
        // Default to running from firstArg.
        // E.g "./math 10+10"
        input = string(firstArg);
    }

    run(input);
    exit(0);
}