#include "translate.h"
#include <stdlib.h>

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

    InterCode *ret = new_function_code(fun_dec->children[0]->value.str_val);

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

    return concat_inter_codes(2, translate_Def_list(comp_st->children[1]), translate_Stmt_list(comp_st->children[2]));
}

InterCode *translate_Var_list(TreeNode *var_list)
{
    assert(CHECK_NON_TYPE(var_list, VarList));

    InterCode *ret = NULL;

    while (var_list->num_of_children == 1 || var_list->num_of_children == 3)
    {

        TreeNode *var_dec = var_list->children[0]->children[1];
        assert(var_dec->num_of_children == 1);
        TreeNode *id = var_dec->children[0];

        //core code
        InterCode *code = new_param_code(id->value.str_val);
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
    Operand *place = new_temp_op();

    if (stmt->num_of_children == 2 && CHECK_NON_TYPE(stmt->children[0], Exp))
    {
        //Exp SEMI
        ret = concat_inter_codes(2, ret, translate_Exp(stmt->children[0], place));
    }
    else if (stmt->num_of_children == 1 && CHECK_NON_TYPE(stmt->children[0], CompSt))
    {
        //CompSt
        ret = translate_Comp_st(stmt->children[0]);
    }
    else if (stmt->num_of_children == 3 && CHECK_TOKEN_TYPE(stmt->children[0], RETURN_T) && CHECK_NON_TYPE(stmt->children[1], Exp) && CHECK_TOKEN_TYPE(stmt->children[2], SEMI_T))
    {
        //RETURN Exp SEMI
        Operand *t1 = new_temp_op();
        InterCode *code1 = translate_Exp(stmt->children[1], t1);
        InterCode *code2 = new_return_code(t1);
        ret = concat_inter_codes(2, code1, code2);
    }
    else if (stmt->num_of_children == 5 && CHECK_TOKEN_TYPE(stmt->children[0], IF_T))
    {
        //IF LP Exp RP Stmt

        Operand *label1 = new_label();
        Operand *label2 = new_label();
        InterCode *code1 = translate_Cond(stmt->children[2], label1, label2);
        InterCode *code2 = translate_Stmt(stmt->children[4]);
        ret = concat_inter_codes(4, code1, new_label_code(label1), code2, new_label_code(label2));
    }
    else if (stmt->num_of_children == 7 && CHECK_TOKEN_TYPE(stmt->children[0], IF_T))
    {
        //IF LP Exp RP Stmt ELSE Stmt
        Operand *label1 = new_label();
        Operand *label2 = new_label();
        Operand *label3 = new_label();
        InterCode *code1 = translate_Cond(stmt->children[2], label1, label2);
        InterCode *code2 = translate_Stmt(stmt->children[4]);
        InterCode *code3 = translate_Stmt(stmt->children[6]);

        ret = concat_inter_codes(7, code1, new_label_code(label1), code2, new_goto_code(label3), new_label_code(label2), code3, new_label_code(label3));
    }
    else if (stmt->num_of_children == 5 && CHECK_TOKEN_TYPE(stmt->children[0], WHILE_T))
    {
        //WHILE LP Exp RP Stmt
        Operand *label1 = new_label();
        Operand *label2 = new_label();
        Operand *label3 = new_label();

        InterCode *code1 = translate_Cond(stmt->children[2], label2, label3);
        InterCode *code2 = translate_Stmt(stmt->children[4]);

        ret = concat_inter_codes(6, new_label_code(label1), code1, new_label_code(label2), code2, new_goto_code(label1), new_label_code(label3));
    }
    else
    {
        assert(false);
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
            Operand *left = new_real_var_op(exp1->children[0]->value.str_val);
            Operand *t1 = new_temp_op();
            InterCode *code1 = translate_Exp(exp2, t1);
            InterCode *code2 = new_assign_code(left, t1);
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

        ret = handle_Cond(exp, place);
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

        InterCode *code2 = new_arithmetic_code(MINUS_CODE,place,zero,t1);

        ret = concat_inter_codes(2, code1, code2);
    }
    else if (exp->num_of_children == 2 && CHECK_TOKEN_TYPE(exp->children[0], NOT_T))
    {
        assert(CHECK_NON_TYPE(exp->children[1], Exp));
        assert(false);
    }
    else if (exp->num_of_children == 4 && CHECK_TOKEN_TYPE(exp->children[0], ID_T) && CHECK_TOKEN_TYPE(exp->children[1], LP_T) && CHECK_NON_TYPE(exp->children[2], Args) && CHECK_TOKEN_TYPE(exp->children[3], RP_T))
    {
        TreeNode *id = exp->children[0];
        OperandWrapper *arg_list = malloc(sizeof(*arg_list));
        //ID LP Args RP
        InterCode *code1 = translate_Args(exp->children[2], arg_list);
        if (strcmp(WRITE, id->value.str_val) == 0)
        {
            ret = concat_inter_codes(2, code1, new_write_code(arg_list->head));
        }
        else
        {
            InterCode *code2 = NULL;
            Operand *args = arg_list->head;
            while (args != NULL)
            {
                code2 = concat_inter_codes(2, args, code2);
                args = args->next;
            }
            ret = concat_inter_codes(3, code1, code2, new_call_code(place, id->value.str_val));
        }
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[0], ID_T) && CHECK_TOKEN_TYPE(exp->children[1], LP_T) && CHECK_TOKEN_TYPE(exp->children[2], RP_T))
    {
        //ID LP RP
        TreeNode *id = exp->children[0];
        if (strcmp(READ, id->value.str_val) == 0)
        {
            ret = new_read_code(place);
        }
        else
        {
            ret = new_call_code(place, id->value.str_val);
        }
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

    InterCode *code3 = new_arithmetic_code(kind,place,left,right);

    return concat_inter_codes(3, code1, code2, code3);
}

InterCode *handle_Cond(TreeNode *exp, Operand *place)
{
    Operand *label_true = new_label();
    Operand *label_false = new_label();
    Operand *zero = new_constant_op(0);
    InterCode *code0 = new_assign_code(place, zero);
    InterCode *code1 = translate_Cond(exp, label_true, label_false);
    InterCode *code2 = new_label_code(label_true);
    Operand *one = new_constant_op(1);
    InterCode *code3 = new_assign_code(place, one);
    InterCode *code4 = new_label_code(label_false);

    return concat_inter_codes(5, code0, code1, code2, code3, code4);
}

InterCode *translate_Cond(TreeNode *exp, Operand *label_true, Operand *label_false)
{
    InterCode *ret = NULL;

    if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], RELOP_T))
    {
        Operand *t1 = new_temp_op();
        Operand *t2 = new_temp_op();
        InterCode *code1 = translate_Exp(exp->children[0], t1);
        InterCode *code2 = translate_Exp(exp->children[2], t2);
        InterCode *code3 = new_if_goto_code(t1,t2,exp->children[1]->value.str_val,label_true);
        InterCode *code4 = new_goto_code(label_false);
        ret = concat_inter_codes(4, code1, code2, code3, code4);
    }
    else if (exp->num_of_children == 2 && CHECK_TOKEN_TYPE(exp->children[0], NOT_T))
    {
        ret = translate_Cond(exp, label_false, label_true);
    }
    else if (exp->num_of_children == 3 && CHECK_NON_TYPE(exp->children[1], AND_T))
    {
        Operand *label1 = new_label();
        InterCode *code1 = translate_Cond(exp->children[0], label1, label_false);
        InterCode *code2 = new_label_code(label1);
        InterCode *code3 = translate_Cond(exp->children[2], label_true, label_false);
        ret = concat_inter_codes(3, code1, code2, code3);
    }
    else if (exp->num_of_children == 3 && CHECK_NON_TYPE(exp->children[1], OR_T))
    {
        Operand *label1 = new_label();
        InterCode *code1 = translate_Cond(exp->children[0], label_true, label1);
        InterCode *code2 = new_label_code(label1);
        InterCode *code3 = translate_Cond(exp->children[2], label_true, label_false);
        ret = concat_inter_codes(3, code1, code2, code3);
    }
    else
    {
        Operand *t1 = new_temp_op();
        Operand *zero = new_constant_op(0);
        InterCode *code1 = translate_Exp(exp, t1);
        InterCode *code2 = new_if_goto_code(t1,zero,RELOP_NO_EQUAL,label_true);
        InterCode *code3 = new_goto_code(label_false);
        ret = concat_inter_codes(3, code1, code2, code3);
    }

    return ret;
}

InterCode *translate_Args(TreeNode *args, OperandWrapper *list)
{
    Operand *t1 = new_temp_op();
    InterCode *code1 = translate_Exp(args->children[0], t1);
    list->head = concat_operands(2, list->head, t1);
    if (args->num_of_children == 3)
    {
        return concat_inter_codes(2, code1, translate_Args(args->children[2], list));
    }
    else
    {
        return code1;
    }
}

InterCode *translate_Def_list(TreeNode *def_list)
{
    if (def_list->num_of_children == 0)
    {
        return NULL;
    }
    return concat_inter_codes(2, translate_Def(def_list->children[0]), translate_Def_list(def_list->children[1]));
}

InterCode *translate_Def(TreeNode *def)
{
    return translate_Dec_list(def->children[1]);
}

InterCode *translate_Dec_list(TreeNode *dec_list)
{
    InterCode *code1 = translate_Dec(dec_list->children[0]);
    if (dec_list->num_of_children == 1)
    {
        return code1;
    }
    else
    {
        return concat_inter_codes(2, code1, translate_Dec_list(dec_list->children[2]));
    }
}

InterCode *translate_Dec(TreeNode *dec)
{
    OperandWrapper *wrapper = malloc(sizeof(*wrapper));
    InterCode *code1 = translate_Var_dec(dec->children[0], wrapper);
    if (dec->num_of_children == 1)
    {
        return code1;
    }
    else
    {
        Operand *place = new_temp_op();
        InterCode *code2 = translate_Exp(dec->children[2], place);
        InterCode *code3 = new_assign_code(wrapper->head, place);
        return concat_inter_codes(3, code1, code2, code3);
    }
}

InterCode *translate_Var_dec(TreeNode *var_dec, OperandWrapper *wrapper)
{
    if (var_dec->num_of_children == 1)
    {
        wrapper->head = new_real_var_op(var_dec->children[0]->value.str_val);
        return NULL;
    }
    else
    {
        assert(false);
    }
}