#include <assert.h>

#include <iostream>

#include "Ast.hpp"
#include "Lexer.hpp"

/*


TODO
---------
[x] - Better whitespace
[x] - Nicer way of handling bin ops
[x] - Better numbers doubles
[x] - More maths operators
    [x] - MOD %
    [x] - EXP **
    [ ] - right assosiative operators
[ ] - Uniary operators + -
[x] - Maths functions sin cos tan log ln
[ ] - statements
[ ] - assignment
[ ] - strings
*/

using namespace std;

class Parser {
    vector<Token> input;
    int size = 0;
    int i = 0;

    void error(string msg);

   public:
    Parser(vector<Token> input) : i(0), input(input), size(input.size()) {}

    void reset();
    bool isEnd();

    Token peek(int d);
    Token peekNonWhitespace(int d);
    bool peekIs(TokenType type);

    int nextNonWhitespaceOffset(int d);

    Token consume();
    Token expect(TokenType type, string msg);
    void consumeWhiteSpace();

    AstStmts* parse();
    AstStmts* parseStatements();
    AstStmt* parseStatement();
    AstNumber* parseNumber();
    AstIdent* parseIdent();
    AstFunctionCall* parseFunctionCall();
    AstExpr* parseSubExpr();
    AstExpr* parseExpr(int precedence_level = -9999);
};