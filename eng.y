%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex();

typedef struct {
    char* name;
    int value;
} Var;

Var vars[100];
int var_count = 0;

int get_var_value(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0)
            return vars[i].value;
    }
    fprintf(stderr, "Undefined variable: %s\n", name);
    exit(1);
}

void set_var_value(const char* name, int value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            vars[i].value = value;
            return;
        }
    }
    vars[var_count].name = strdup(name);
    vars[var_count].value = value;
    var_count++;
}
%}

%union {
    int num;
    char* id;
}

%token <num> NUMBER
%token <id> IDENTIFIER
%token SET TO PRINT
%token PLUS MINUS MUL DIV LPAREN RPAREN

%type <num> expression

%left PLUS MINUS
%left MUL DIV

%start program

%%

program:
    | program statement
    ;

statement:
      SET IDENTIFIER TO expression   { set_var_value($2, $4); }
    | PRINT expression               { printf("%d\n", $2); }
    ;

expression:
      NUMBER                         { $$ = $1; }
    | IDENTIFIER                     { $$ = get_var_value($1); }
    | expression PLUS expression     { $$ = $1 + $3; }
    | expression MINUS expression    { $$ = $1 - $3; }
    | expression MUL expression      { $$ = $1 * $3; }
    | expression DIV expression      { $$ = $1 / $3; }
    | LPAREN expression RPAREN       { $$ = $2; }
    ;

%%


void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}