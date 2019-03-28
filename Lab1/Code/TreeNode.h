#ifndef TREENODE_H
#define TREENODE_H

//for strdup bug
#define _POSIX_C_SOURCE 200809L
#include "util.h"

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
// TreeNode *create_error_node(int first_line);
// TreeNode *create_node(YYTOKENTYPE type);

#endif
