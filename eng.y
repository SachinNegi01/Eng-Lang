%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
extern FILE* yyin;

void yyerror(const char *s);
int yylex();

ASTNode* root = NULL;
%}

%union {
    int num;
    char* id;
    struct ASTNode* node;
    struct {
        struct ASTNode** stmts;
        int count;
    } nodelist;
}

%token <num> NUMBER
%token <id> IDENTIFIER
%token SET PRINT LOOP FROM TO WITH UNTIL INC DEC
%token PLUS MINUS MUL DIV LPAREN RPAREN
%token LEQ GEQ LT GT EQ NEQ
%token SEMICOLON
%token ASSIGN
%token IF ELSE THEN

%type <node> statement expression program block
%type <nodelist> statement_list

%left PLUS MINUS
%left MUL DIV
%left EQ NEQ LT GT LEQ GEQ

%start program

%%

program:
      statement_list { root = make_block_node($1.stmts, $1.count); $$ = root; }
    ;

block:
      '{' statement_list '}' { $$ = make_block_node($2.stmts, $2.count); }
    ;

statement_list:
      statement { struct ASTNode** stmts = malloc(sizeof(struct ASTNode*)); stmts[0] = $1; $$.stmts = stmts; $$.count = 1; }
    | statement_list SEMICOLON statement { struct ASTNode** stmts = realloc($1.stmts, ($1.count + 1) * sizeof(struct ASTNode*)); stmts[$1.count] = $3; $$.stmts = stmts; $$.count = $1.count + 1; }
    | statement_list statement { struct ASTNode** stmts = realloc($1.stmts, ($1.count + 1) * sizeof(struct ASTNode*)); stmts[$1.count] = $2; $$.stmts = stmts; $$.count = $1.count + 1; }
    | statement_list SEMICOLON { $$.stmts = $1.stmts; $$.count = $1.count; }
    ;

statement:
      SET IDENTIFIER TO expression { $$ = make_assign_node($2, $4); }
    | SET IDENTIFIER expression { $$ = make_assign_node($2, $3); }
    | SET IDENTIFIER ASSIGN expression { $$ = make_assign_node($2, $4); }
    | PRINT expression         { $$ = make_print_node($2); }
    | IF expression THEN block ELSE block { $$ = make_if_node($2, $4, $6); }
    | LOOP FROM IDENTIFIER ASSIGN expression TO expression WITH IDENTIFIER INC block {
        $$ = make_for_node($3, $5, $7, make_binop_node('+', make_var_node($9), make_number_node(1)), $11);
    }
    | LOOP FROM IDENTIFIER ASSIGN expression TO expression WITH IDENTIFIER DEC block {
        $$ = make_for_node($3, $5, $7, make_binop_node('-', make_var_node($9), make_number_node(1)), $11);
    }
    | LOOP UNTIL expression block {
        $$ = make_while_node($3, $4);
    }
    ;

expression:
      NUMBER                   { $$ = make_number_node($1); }
    | IDENTIFIER               { $$ = make_var_node($1); }
    | expression PLUS expression   { $$ = make_binop_node('+', $1, $3); }
    | expression MINUS expression  { $$ = make_binop_node('-', $1, $3); }
    | expression MUL expression    { $$ = make_binop_node('*', $1, $3); }
    | expression DIV expression    { $$ = make_binop_node('/', $1, $3); }
    | expression LT expression     { $$ = make_binop_node('<', $1, $3); }
    | expression GT expression     { $$ = make_binop_node('>', $1, $3); }
    | expression LEQ expression    { $$ = make_binop_node('L', $1, $3); }
    | expression GEQ expression    { $$ = make_binop_node('G', $1, $3); }
    | expression EQ expression     { $$ = make_binop_node('=', $1, $3); }
    | expression NEQ expression    { $$ = make_binop_node('!', $1, $3); }
    | LPAREN expression RPAREN     { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    if (feof(yyin)) return;
    fprintf(stderr, "Error: %s\n", s);
    fprintf(stderr, "[DEBUG] Parse error encountered.\n");
}