#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
void interpret(ASTNode* node);

extern int yyparse();
extern FILE* yyin;
extern ASTNode* root;

int main(int argc, char** argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("File open failed");
            return 1;
        }
    }
    yyparse();
    if (!root) {
        fprintf(stderr, "Parse error: AST root is NULL.\n");
        return 1;
    }
    interpret(root);
    free_ast(root);
    if (yyin && argc > 1) {
        fclose(yyin);
    }
    return 0;
}

int yywrap() { return 1; }