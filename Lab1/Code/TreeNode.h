#ifndef TREENODE_H
#define TREENODE_H

#include "syntax.tab.h"
struct TreeNode_t
{
    union {
        int int_val;
        float float_val;
        char *str_val;
    } value;
    char *name;
    int num_of_children;
    struct TreeNode_t *children;
};

typedef struct TreeNode_t TreeNode;

TreeNode *create_node(char *name);

// TreeNode *create_node(YYTOKENTYPE type);

#endif
