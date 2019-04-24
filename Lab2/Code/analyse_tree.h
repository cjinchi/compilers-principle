#ifndef ANALYSE_TREE_H
#define ANALYSE_TREE_H

#include "TreeNode.h"
#include "Type.h"
void analyse_program(TreeNode *node);

void analyse_comp_st(TreeNode *comp_st);

Type *analyse_exp(TreeNode *exp);

#endif