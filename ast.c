#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

ASTNode* make_assign_node(char* var, ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_ASSIGN;
    node->assign.var = strdup(var);
    node->assign.expr = expr;
    return node;
}

ASTNode* make_print_node(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_PRINT;
    node->print.expr = expr;
    return node;
}

ASTNode* make_if_node(ASTNode* cond, ASTNode* then_branch, ASTNode* else_branch) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_IF;
    node->if_stmt.cond = cond;
    node->if_stmt.then_branch = then_branch;
    node->if_stmt.else_branch = else_branch;
    return node;
}

ASTNode* make_for_node(char* var, ASTNode* init, ASTNode* cond, ASTNode* step, ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_FOR;
    node->for_stmt.var = strdup(var);
    node->for_stmt.init = init;
    node->for_stmt.cond = cond;
    node->for_stmt.step = step;
    node->for_stmt.body = body;
    return node;
}

ASTNode* make_while_node(ASTNode* cond, ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_WHILE;
    node->while_stmt.cond = cond;
    node->while_stmt.body = body;
    return node;
}

ASTNode* make_block_node(ASTNode** stmts, int count) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_BLOCK;
    if (count == 0 && stmts) {
        // Count until NULL
        int real_count = 0;
        while (stmts[real_count]) real_count++;
        node->block.stmts = stmts;
        node->block.count = real_count;
    } else {
        node->block.stmts = stmts;
        node->block.count = count;
    }
    return node;
}

ASTNode* make_number_node(int num) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_NUMBER;
    node->num = num;
    return node;
}

ASTNode* make_var_node(char* var_name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_VAR;
    node->var_name = strdup(var_name);
    return node;
}

ASTNode* make_binop_node(int op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = AST_BINOP;
    node->binop.op = op;
    node->binop.left = left;
    node->binop.right = right;
    return node;
}

void free_ast(ASTNode* node) {
    if (!node) return;
    switch (node->type) {
        case AST_ASSIGN:
            free(node->assign.var);
            free_ast(node->assign.expr);
            break;
        case AST_PRINT:
            free_ast(node->print.expr);
            break;
        case AST_IF:
            free_ast(node->if_stmt.cond);
            free_ast(node->if_stmt.then_branch);
            free_ast(node->if_stmt.else_branch);
            break;
        case AST_FOR:
            free(node->for_stmt.var);
            free_ast(node->for_stmt.init);
            free_ast(node->for_stmt.cond);
            free_ast(node->for_stmt.step);
            free_ast(node->for_stmt.body);
            break;
        case AST_WHILE:
            free_ast(node->while_stmt.cond);
            free_ast(node->while_stmt.body);
            break;
        case AST_BLOCK:
            for (int i = 0; i < node->block.count; ++i)
                free_ast(node->block.stmts[i]);
            free(node->block.stmts);
            break;
        case AST_NUMBER:
            break;
        case AST_VAR:
            free(node->var_name);
            break;
        case AST_BINOP:
            free_ast(node->binop.left);
            free_ast(node->binop.right);
            break;
    }
    free(node);
}

void print_ast(ASTNode* node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; ++i) printf("  ");
    switch (node->type) {
        case AST_ASSIGN:
            printf("ASSIGN %s\n", node->assign.var);
            print_ast(node->assign.expr, indent + 1);
            break;
        case AST_PRINT:
            printf("PRINT\n");
            print_ast(node->print.expr, indent + 1);
            break;
        case AST_BLOCK:
            printf("BLOCK\n");
            for (int i = 0; i < node->block.count; ++i)
                print_ast(node->block.stmts[i], indent + 1);
            break;
        case AST_FOR:
            printf("FOR %s\n", node->for_stmt.var);
            print_ast(node->for_stmt.init, indent + 1);
            print_ast(node->for_stmt.cond, indent + 1);
            print_ast(node->for_stmt.step, indent + 1);
            print_ast(node->for_stmt.body, indent + 1);
            break;
        case AST_WHILE:
            printf("WHILE\n");
            print_ast(node->while_stmt.cond, indent + 1);
            print_ast(node->while_stmt.body, indent + 1);
            break;
        case AST_NUMBER:
            printf("NUMBER %d\n", node->num);
            break;
        case AST_VAR:
            printf("VAR %s\n", node->var_name);
            break;
        case AST_BINOP:
            printf("BINOP %c\n", node->binop.op);
            print_ast(node->binop.left, indent + 1);
            print_ast(node->binop.right, indent + 1);
            break;
    }
} 