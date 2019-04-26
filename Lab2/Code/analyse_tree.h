#ifndef ANALYSE_TREE_H
#define ANALYSE_TREE_H

#include "TreeNode.h"
#include "Type.h"
void analyse_program(TreeNode *node);

void analyse_comp_st(TreeNode *comp_st, Type *return_type);

Type *analyse_exp(TreeNode *exp);

void analyse_stmt(TreeNode *stmt, Type *return_type);

void analyse_ext_dec_list(TreeNode *ext_dec_list, Type *type);

//This func will add boundary to symbol list
void analyse_fun_dec(TreeNode *fun_dec, Type *return_type);

Type *check_assign(TreeNode *left, TreeNode *right, int first_line);

Type *check_unary_logic(TreeNode *right, int first_line);

Type *check_binary_logic(TreeNode *left, TreeNode *right, int first_line);

Type *check_binary_arithmetic(TreeNode *left, TreeNode *right, int first_line);

Type *check_unary_arithmetic(TreeNode *right, int first_line);

Type *check_func_call(TreeNode *id, TreeNode *args);

Type *check_array_call(TreeNode *arr, TreeNode *num);

Type *check_dot(TreeNode *left, TreeNode *right);
Type *check_id(TreeNode *id);
#endif