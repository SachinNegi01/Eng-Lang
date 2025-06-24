#ifndef AST_H
#define AST_H

#include <stdio.h>

// AST Node Types
typedef enum {
    AST_ASSIGN,
    AST_PRINT,
    AST_IF,
    AST_FOR,
    AST_WHILE,
    AST_BLOCK,
    AST_NUMBER,
    AST_VAR,
    AST_BINOP
} ASTNodeType;

// Forward declaration
struct ASTNode;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        // Assignment: var = expr
        struct { char* var; struct ASTNode* expr; } assign;
        // Print: print expr
        struct { struct ASTNode* expr; } print;
        // If: if cond then then_branch else else_branch
        struct { struct ASTNode* cond; struct ASTNode* then_branch; struct ASTNode* else_branch; } if_stmt;
        // For: for (var = init; cond; step) body
        struct { char* var; struct ASTNode* init; struct ASTNode* cond; struct ASTNode* step; struct ASTNode* body; } for_stmt;
        // While: while (cond) body
        struct { struct ASTNode* cond; struct ASTNode* body; } while_stmt;
        // Block: sequence of statements
        struct { struct ASTNode** stmts; int count; } block;
        // Number literal
        int num;
        // Variable reference
        char* var_name;
        // Binary operation
        struct { int op; struct ASTNode* left; struct ASTNode* right; } binop;
    };
} ASTNode;

// Constructors
ASTNode* make_assign_node(char* var, ASTNode* expr);
ASTNode* make_print_node(ASTNode* expr);
ASTNode* make_if_node(ASTNode* cond, ASTNode* then_branch, ASTNode* else_branch);
ASTNode* make_for_node(char* var, ASTNode* init, ASTNode* cond, ASTNode* step, ASTNode* body);
ASTNode* make_while_node(ASTNode* cond, ASTNode* body);
ASTNode* make_block_node(ASTNode** stmts, int count);
ASTNode* make_number_node(int num);
ASTNode* make_var_node(char* var_name);
ASTNode* make_binop_node(int op, ASTNode* left, ASTNode* right);
void free_ast(ASTNode* node);
void print_ast(ASTNode* node, int indent);

#endif // AST_H 