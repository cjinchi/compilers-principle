#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "TreeNode.h"
#include "InterCode.h"

InterCode *translate_Program(TreeNode *exp);

InterCode *translate_Ext_def_list(TreeNode *ext_def_list);

InterCode *translate_Ext_def(TreeNode *ext_def);

InterCode *translate_Fun_dec(TreeNode *fun_dec);

InterCode *translate_Comp_st(TreeNode *comp_st);

#endif