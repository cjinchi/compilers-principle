#include "TreeNode.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

TreeNode *create_token_node(int t_type, int first_line)
{
    TreeNode *node = malloc(sizeof(*node));
    node->is_token = true;
    node->type = (int)t_type;
    node->first_line = first_line;
    return node;
}

TreeNode *create_nonterminal_node(int n_type, int first_line, int num_of_children, ...)
{
    TreeNode *node = malloc(sizeof(*node));
    node->num_of_children = num_of_children;
    node->is_token = false;
    node->type = n_type;
    node->first_line = first_line;
    node->children = malloc(num_of_children * sizeof(TreeNode *));

    va_list ap;
    va_start(ap, num_of_children);
    for (int i = 0; i < num_of_children; i++)
    {
        node->children[i] = va_arg(ap, TreeNode *);
    }
    va_end(ap);

    return node;
}
