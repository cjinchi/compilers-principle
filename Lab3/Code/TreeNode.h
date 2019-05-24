#ifndef TREENODE_H
#define TREENODE_H

// //for strdup bug
// #define _POSIX_C_SOURCE 200809L

#include "util.h"
#include "string.h"
#include "Type.h"
#include "Operand.h"

typedef struct Type_ Type;
typedef struct OperandWrapper_ OperandWrapper;

struct TreeNode_t
{
    union {
        int int_val;
        float float_val;
        char *str_val;
    } value;

    //if is_token, int = YYTOKENTYPE
    //if !is_token, int = nonterminal_symbol_t
    int type;

    bool is_token;
    int num_of_children;

    int first_line;

    //array, children[i] is a pointer to ith-child
    struct TreeNode_t **children;
};

typedef struct TreeNode_t TreeNode;

TreeNode *create_token_node(int t_type, int first_line);
TreeNode *create_nonterminal_node(int n_type, int first_line, int num_of_children, ...);
TreeNode *create_error_node(int first_line);

void print_tree(TreeNode *head);
void print_node(TreeNode *node, int depth);
char *get_literal(TreeNode *node);

bool check_nonterminal_type(TreeNode *node, int type);
bool check_token_type(TreeNode *node, int type);

int get_size(Type *type);
int get_struct_offset(OperandWrapper *addr, TreeNode *exp, TreeNode *id);

#define CHECK_NON_TYPE(node, type) (check_nonterminal_type(node, type) == true)
#define CHECK_TOKEN_TYPE(node, type) (check_token_type(node, type) == true)

void handle(TreeNode *root);

#endif
