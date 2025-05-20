#include <stdio.h>

extern int yyparse();
extern FILE* yyin;

int main(int argc, char** argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("File open failed");
            return 1;
        }
    }
    yyparse();
    return 0;
}