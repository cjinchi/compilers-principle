#include "translate.h"

InterCode *translate_Program(TreeNode *program)
{
    assert(CHECK_NON_TYPE(program, Program));

    TreeNode *ext_def_list = program->children[0];

    //todo print here
    return translate_Ext_def_list(ext_def_list);
}

InterCode *translate_Ext_def_list(TreeNode *ext_def_list)
{
    assert(CHECK_NON_TYPE(ext_def_list, ExtDefList));

    InterCode *ret = NULL;

    //loop in ext_def_list
    while (ext_def_list->num_of_children == 2)
    {
        TreeNode *ext_def = ext_def_list->children[0];
        assert(CHECK_NON_TYPE(ext_def, ExtDef));

        ret = concat_inter_codes(2, ret, translate_Ext_def(ext_def));

        ext_def_list = ext_def_list->children[1];
    }
    return ret;
}

InterCode *translate_Ext_def(TreeNode *ext_def)
{
    assert(CHECK_NON_TYPE(ext_def, ExtDefList));

    if (CHECK_NON_TYPE(ext_def->children[1], FunDec) && CHECK_NON_TYPE(ext_def->children[2], CompSt))
    {
        return concat_inter_codes(2, translate_Fun_dec(ext_def->children[1]), translate_Comp_st(ext_def->children[2]));
    }
    else
    {
        //unnecessary to handle here
        return NULL;
    }
}

InterCode *translate_Fun_dec(TreeNode *fun_dec)
{
    assert(CHECK_NON_TYPE(fun_dec, FunDec));

    InterCode *ret = new_inter_code(FUNCTION_CODE);
    ret->u.node = look_up_function_list(fun_dec->children[0]->value.str_val);

    if (fun_dec->num_of_children == 4)
    {
        //has var_list
        ret = concat_inter_codes(2, ret, translate_Var_list(fun_dec->children[2]));
    }

    return ret;
}

InterCode *translate_Comp_st(TreeNode *comp_st)
{
    assert(CHECK_NON_TYPE(comp_st, CompSt));

    return translate_Stmt_list(comp_st->children[2]);
}

InterCode *translate_Var_list(TreeNode *var_list)
{
    assert(CHECK_NON_TYPE(var_list, VarList));

    TreeNode *ret = NULL;

    while (var_list->num_of_children == 1 || var_list->num_of_children == 3)
    {

        TreeNode *var_dec = var_list->children[0]->children[1];
        assert(var_dec->num_of_children == 1);
        TreeNode *id = var_dec->children[0];

        //core code
        InterCode *code = new_inter_code(PARAM_CODE);
        code->u.node = look_up_variable_list(id->value.str_val);
        ret = concat_inter_codes(2, ret, code);

        if (var_list->num_of_children == 3)
        {
            var_list = var_list->children[2];
        }
        else
        {
            break;
        }
    }
    return ret;
}

InterCode *translate_Stmt_list(TreeNode *stmt_list)
{
    assert(CHECK_NON_TYPE(stmt_list, StmtList));

    InterCode *ret = NULL;

    while (stmt_list->num_of_children == 2)
    {
        ret = concat_inter_codes(2, ret, translate_Stmt(stmt_list->children[0]));

        stmt_list = stmt_list->children[1];
    }
    return ret;
}

InterCode *translate_Stmt(TreeNode *stmt)
{
    assert(CHECK_NON_TYPE(stmt, Stmt));

    InterCode *ret = NULL;

    if (stmt->num_of_children == 2 && CHECK_NON_TYPE(stmt->children[0], Exp))
    {
        ret = concat_inter_codes(2, ret, translate_Exp(stmt->children[0]));
    }
    else
    {
        // todo
        assert(0);
    }
    return ret;
}

InterCode *translate_Exp(TreeNode *exp, Operand *place)
{
    assert(CHECK_NON_TYPE(exp, Exp));
    InterCode *ret = NULL;

    if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], ASSIGNOP_T))
    {
        //Exp -> Exp1 ASSIGNOP Exp2
        TreeNode *exp1 = exp->children[0];
        TreeNode *exp2 = exp->children[2];

        if (exp1->num_of_children == 1 && CHECK_TOKEN_TYPE(exp1->children[0], ID_T))
        {
            //Exp1 -> ID
            Operand *right = new_temp_op();
            InterCode *code1 = translate_Exp(exp2, t1);
            Operand *left = new_real_var_op(exp1->children[0]->value.str_val);
            InterCode *code2 = new_assign_code(left, right);
            InterCode *code3 = new_assign_code(place, left);

            ret = concat_inter_codes(4, ret, code1, code2, code3);
        }
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], AND_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_NON_TYPE(exp->children[2], Exp));
        assert(false);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], OR_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_NON_TYPE(exp->children[2], Exp));
        assert(false);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], RELOP_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_NON_TYPE(exp->children[2], Exp));
        assert(false);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], PLUS_T))
    {
        ret = translate_binary_arithmetic(exp, place);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], MINUS_T))
    {
        ret = translate_binary_arithmetic(exp, place);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], STAR_T))
    {
        ret = translate_binary_arithmetic(exp, place);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], DIV_T))
    {
        ret = translate_binary_arithmetic(exp, place);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[0], LP_T) && CHECK_TOKEN_TYPE(exp->children[2], RP_T))
    {
        assert(CHECK_NON_TYPE(exp->children[1], Exp));
        assert(false);
    }
    else if (exp->num_of_children == 2 && CHECK_TOKEN_TYPE(exp->children[0], MINUS_T))
    {
        assert(CHECK_NON_TYPE(exp->children[1], Exp));

        Operand *zero = new_constant_op(0);
        Operand *t1 = new_temp_op();
        InterCode *code1 = translate_Exp(exp->children[1], t1);

        InterCode *code2 = new_inter_code(MINUS_T);
        code2->u.binop.result = place;
        code2->u.binop.left = zero;
        code2->u.binop.right = t1;

        ret = concat_inter_codes(2, code1, code2);
    }
    else if (exp->num_of_children == 2 && CHECK_TOKEN_TYPE(exp->children[0], NOT_T))
    {
        assert(CHECK_NON_TYPE(exp->children[1], Exp));
        assert(false);
    }
    else if (exp->num_of_children == 4 && CHECK_TOKEN_TYPE(exp->children[0], ID_T) && CHECK_TOKEN_TYPE(exp->children[1], LP_T) && CHECK_NON_TYPE(exp->children[2], Args) && CHECK_TOKEN_TYPE(exp->children[3], RP_T))
    {
        assert(false);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[0], ID_T) && CHECK_TOKEN_TYPE(exp->children[1], LP_T) && CHECK_TOKEN_TYPE(exp->children[2], RP_T))
    {
        assert(false);
    }
    else if (exp->num_of_children == 4 && CHECK_NON_TYPE(exp->children[0], Exp) && CHECK_TOKEN_TYPE(exp->children[1], LB_T) && CHECK_NON_TYPE(exp->children[2], Exp) && CHECK_TOKEN_TYPE(exp->children[3], RB_T))
    {
        assert(false);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], DOT_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_TOKEN_TYPE(exp->children[2], ID_T));

        assert(false);
    }
    else if (exp->num_of_children == 1 && CHECK_TOKEN_TYPE(exp->children[0], ID_T))
    {
        //Exp -> ID
        ret = new_assign_code(place, new_real_var_op(exp->children[0]->value.str_val));
    }
    else if (exp->num_of_children == 1 && CHECK_TOKEN_TYPE(exp->children[0], INT_T))
    {
        //Exp -> INT
        ret = new_assign_code(place, new_constant_op(exp->children[0]->value.int_val));
    }
    else if (exp->num_of_children == 1 && CHECK_TOKEN_TYPE(exp->children[0], FLOAT_T))
    {
        //Exp -> FLOAT
        assert(false);
    }
    else
    {
        assert(false);
    }
    return ret;
}

InterCode *translate_binary_arithmetic(TreeNode *exp, Operand *place)
{
    TreeNode *exp1 = exp->children[0];
    TreeNode *exp2 = exp->children[2];

    assert(CHECK_NON_TYPE(exp1, Exp));
    assert(CHECK_NON_TYPE(exp2, Exp));

    Operand *left = new_temp_op();
    Operand *right = new_temp_op();
    InterCode *code1 = translate_Exp(exp1, left);
    InterCode *code2 = translate_Exp(exp2, right);

    TreeNode *token = exp->children[1];
    int kind = -1;
    if (CHECK_TOKEN_TYPE(token, PLUS_T))
    {
        kind = PLUS_CODE;
    }
    else if (CHECK_TOKEN_TYPE(token, MINUS_T))
    {
        kind = MINUS_CODE;
    }
    else if (CHECK_TOKEN_TYPE(token, STAR_T))
    {
        kind = STAR_CODE;
    }
    else if (CHECK_TOKEN_TYPE(token, DIV_T))
    {
        kind = DIV_CODE;
    }
    InterCode *code3 = new_inter_code(kind);
    code3->u.binop.result = place;
    code3->u.binop.left = left;
    code3->u.binop.right = right;

    return concat_inter_codes(3, code1, code2, code3);
}