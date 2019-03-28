#include "TreeNode.h"
#include <stdlib.h>
#include <string.h>

TreeNode *create_node(char *name)
{
    TreeNode *node = malloc(sizeof(*node));
    node->name = strdup(name);
    return node;
}