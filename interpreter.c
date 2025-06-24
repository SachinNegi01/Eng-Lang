#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#define MAX_VARS 100

typedef struct {
    char* name;
    int value;
} Var;

Var vars[MAX_VARS];
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

int eval_expr(ASTNode* node) {
    switch (node->type) {
        case AST_NUMBER:
            return node->num;
        case AST_VAR:
            return get_var_value(node->var_name);
        case AST_BINOP: {
            int left = eval_expr(node->binop.left);
            int right = eval_expr(node->binop.right);
            switch (node->binop.op) {
                case '+': return left + right;
                case '-': return left - right;
                case '*': return left * right;
                case '/': return left / right;
                case '<': return left < right;
                case '>': return left > right;
                case 'L': return left <= right;
                case 'G': return left >= right;
                case '=': return left == right;
                case '!': return left != right;
            }
        }
        default:
            fprintf(stderr, "Invalid expression node\n");
            exit(1);
    }
}

void interpret(ASTNode* node) {
    if (!node) return;
    switch (node->type) {
        case AST_ASSIGN:
            set_var_value(node->assign.var, eval_expr(node->assign.expr));
            break;
        case AST_PRINT:
            printf("%d\n", eval_expr(node->print.expr));
            break;
        case AST_BLOCK:
            for (int i = 0; i < node->block.count; ++i)
                interpret(node->block.stmts[i]);
            break;
        case AST_FOR: {
            int init_val = eval_expr(node->for_stmt.init);
            set_var_value(node->for_stmt.var, init_val);
            int iter = 0;
            while (1) {
                int cond_val = eval_expr(node->for_stmt.cond);
                if (!cond_val) break;
                interpret(node->for_stmt.body);
                set_var_value(node->for_stmt.var, eval_expr(node->for_stmt.step));
                iter++;
            }
            break;
        }
        case AST_WHILE: {
            int iter = 0;
            while (1) {
                int cond_val = eval_expr(node->while_stmt.cond);
                if (cond_val) break;
                interpret(node->while_stmt.body);
                iter++;
            }
            break;
        }
        case AST_IF:
            if (eval_expr(node->if_stmt.cond)) {
                interpret(node->if_stmt.then_branch);
            } else {
                interpret(node->if_stmt.else_branch);
            }
            break;
        default:
            fprintf(stderr, "Unknown statement type in interpreter\n");
    }
} 