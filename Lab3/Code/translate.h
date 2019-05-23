#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "TreeNode.h"
#include "InterCode.h"

#include "translate.h"
#include <stdlib.h>

InterCode *translate_Program(TreeNode *program);

InterCode *translate_Ext_def_list(TreeNode *ext_def_list);

InterCode *translate_Ext_def(TreeNode *ext_def);

InterCode *translate_Fun_dec(TreeNode *fun_dec);

InterCode *translate_Comp_st(TreeNode *comp_st);

InterCode *translate_Var_list(TreeNode *var_list);

InterCode *translate_Stmt_list(TreeNode *stmt_list);

InterCode *translate_Stmt(TreeNode *stmt);

InterCode *translate_Exp(TreeNode *exp, Operand *place);

InterCode *translate_binary_arithmetic(TreeNode *exp, Operand *place);

InterCode *handle_Cond(TreeNode *exp, Operand *place);

InterCode *translate_Cond(TreeNode *exp, Operand *label_true, Operand *label_false);

InterCode *translate_Args(TreeNode *args, OperandWrapper *list);

InterCode *translate_Def_list(TreeNode *def_list);

InterCode *translate_Def(TreeNode *def);

InterCode *translate_Dec_list(TreeNode *dec_list);

InterCode *translate_Dec(TreeNode *dec);

InterCode *translate_Var_dec(TreeNode *var_dec, OperandWrapper *wrapper);

#endif