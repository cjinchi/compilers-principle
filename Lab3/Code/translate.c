#include "translate.h"

InterCode *translate_Program(TreeNode *program)
{
    assert(CHECK_NON_TYPE(program, Program));

    TreeNode *ext_def_list = program->children[0];

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

    // todo
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