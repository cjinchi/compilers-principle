#include "analyse_tree.h"
#include "Type.h"
#include "SymbolNode.h"
#include "util.h"

void analyse_program(TreeNode *program)
{
    assert(CHECK_NON_TYPE(program, Program));

    TreeNode *ext_def_list = program->children[0];

    //loop in ext_def_list
    while (ext_def_list->num_of_children == 2)
    {
        TreeNode *ext_def = ext_def_list->children[0];
        CHECK_NON_TYPE(ext_def, ExtDef);

        //the following two var just for convenience
        int children_num = ext_def->num_of_children;
        TreeNode **childrens = ext_def->children;

        assert(children_num > 0 && CHECK_NON_TYPE(childrens[0], Specifier));
        Type *type = get_type_from_specifier(childrens[0]);

        if (children_num == 3 && CHECK_NON_TYPE(childrens[1], ExtDecList) && CHECK_TOKEN_TYPE(childrens[2], SEMI_T))
        {
            //Specifier ExtDecList SEMI
            //variable definition
            TreeNode *ext_dec_list = childrens[1];
            analyse_ext_dec_list(ext_dec_list, type);
        }
        else if (children_num == 2 && CHECK_TOKEN_TYPE(childrens[1], SEMI_T))
        {
            //Specifier SEMI

            //do nothing
            //struct definiton has been checked in get_type_from_specifier
        }
        else if (children_num == 3 && CHECK_NON_TYPE(childrens[1], FunDec) && CHECK_NON_TYPE(childrens[2], CompSt))
        {
            //Specifier FunDec CompSt
            //func definition
            TreeNode *fun_dec = childrens[1];

            //boundary added in the following function
            analyse_fun_dec(fun_dec, type);

            //dont't add boundary here
            analyse_comp_st(childrens[2], type);
            remove_scope_from_symbol_list();
        }
        else
        {
            assert(false);
        }

        ext_def_list = ext_def_list->children[1];
    }
}

void analyse_comp_st(TreeNode *comp_st, Type *return_type)
{
    assert(CHECK_NON_TYPE(comp_st, CompSt));

    //-- DefList -
    TreeNode *def_list = comp_st->children[1];
    FieldList *def_field_list = get_def_list(def_list);
    FieldList *p = def_field_list;
    while (p != NULL)
    {
        //has assigned
        if (p->assigned_with != NULL)
        {
            if (type_equal(p->type, analyse_exp(p->assigned_with)) == false)
            {
                print_semantic_error(5, p->first_line);
            }
        }

        //check if var name used
        if (look_up_variable_list(p->name, true) != NULL || look_up_struct_list(p->name) != NULL)
        {
            print_semantic_error(3, p->first_line);
        }
        else
        {
            // printf("addd %s to list\n", p->name);
            add_to_variable_list(p->name, p->type);
        }
        p = p->next;
    }

    //-- StmtList
    TreeNode *stmt_list = comp_st->children[2];
    while (stmt_list->num_of_children == 2)
    {
        TreeNode *stmt = stmt_list->children[0];

        analyse_stmt(stmt, return_type);

        stmt_list = stmt_list->children[1];
    }
}

Type *analyse_exp(TreeNode *exp)
{
    assert(CHECK_NON_TYPE(exp, Exp));

    if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], ASSIGNOP_T))
    {
        //Exp ASSIGNOP Exp
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_NON_TYPE(exp->children[2], Exp));

        return check_assign(exp->children[0], exp->children[2], exp->children[1]->first_line);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], AND_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_NON_TYPE(exp->children[2], Exp));
        return check_binary_logic(exp->children[0], exp->children[2], exp->children[1]->first_line);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], OR_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_NON_TYPE(exp->children[2], Exp));
        return check_binary_logic(exp->children[0], exp->children[2], exp->children[1]->first_line);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], RELOP_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_NON_TYPE(exp->children[2], Exp));
        return check_binary_logic(exp->children[0], exp->children[2], exp->children[1]->first_line);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], PLUS_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_NON_TYPE(exp->children[2], Exp));

        return check_binary_arithmetic(exp->children[0], exp->children[2], exp->children[1]->first_line);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], MINUS_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_NON_TYPE(exp->children[2], Exp));

        return check_binary_arithmetic(exp->children[0], exp->children[2], exp->children[1]->first_line);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], STAR_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_NON_TYPE(exp->children[2], Exp));

        return check_binary_arithmetic(exp->children[0], exp->children[2], exp->children[1]->first_line);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], DIV_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_NON_TYPE(exp->children[2], Exp));

        return check_binary_arithmetic(exp->children[0], exp->children[2], exp->children[1]->first_line);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[0], LP_T) && CHECK_TOKEN_TYPE(exp->children[2], RP_T))
    {
        assert(CHECK_NON_TYPE(exp->children[1], Exp));
        return analyse_exp(exp->children[1]);
    }
    else if (exp->num_of_children == 2 && CHECK_TOKEN_TYPE(exp->children[0], MINUS_T))
    {
        assert(CHECK_NON_TYPE(exp->children[1], Exp));
        return check_unary_arithmetic(exp->children[1], exp->children[0]->first_line);
    }
    else if (exp->num_of_children == 2 && CHECK_TOKEN_TYPE(exp->children[0], NOT_T))
    {
        assert(CHECK_NON_TYPE(exp->children[1], Exp));
        return check_unary_logic(exp->children[1], exp->children[0]->first_line);
    }
    else if (exp->num_of_children == 4 && CHECK_TOKEN_TYPE(exp->children[0], ID_T) && CHECK_TOKEN_TYPE(exp->children[1], LP_T) && CHECK_NON_TYPE(exp->children[2], Args) && CHECK_TOKEN_TYPE(exp->children[3], RP_T))
    {
        return check_func_call(exp->children[0], exp->children[2]);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[0], ID_T) && CHECK_TOKEN_TYPE(exp->children[1], LP_T) && CHECK_TOKEN_TYPE(exp->children[2], RP_T))
    {
        return check_func_call(exp->children[0], NULL);
    }
    else if (exp->num_of_children == 4 && CHECK_NON_TYPE(exp->children[0], Exp) && CHECK_TOKEN_TYPE(exp->children[1], LB_T) && CHECK_NON_TYPE(exp->children[2], Exp) && CHECK_TOKEN_TYPE(exp->children[3], RB_T))
    {
        return check_array_call(exp->children[0], exp->children[2]);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], DOT_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_TOKEN_TYPE(exp->children[2], ID_T));

        return check_dot(exp->children[0], exp->children[2]);
    }
    else if (exp->num_of_children == 1 && CHECK_TOKEN_TYPE(exp->children[0], ID_T))
    {
        return check_id(exp->children[0]);
    }
    else if (exp->num_of_children == 1 && CHECK_TOKEN_TYPE(exp->children[0], INT_T))
    {
        return TYPE_INT;
    }
    else if (exp->num_of_children == 1 && CHECK_TOKEN_TYPE(exp->children[0], FLOAT_T))
    {
        return TYPE_FLOAT;
    }
    else
    {
        assert(false);
    }
}

void analyse_stmt(TreeNode *stmt, Type *return_type)
{
    assert(CHECK_NON_TYPE(stmt, Stmt));

    if (stmt->num_of_children == 2 && CHECK_NON_TYPE(stmt->children[0], Exp) && CHECK_TOKEN_TYPE(stmt->children[1], SEMI_T))
    {
        //Exp SEMI
        analyse_exp(stmt->children[0]);
    }
    else if (stmt->num_of_children == 1 && CHECK_NON_TYPE(stmt->children[0], CompSt))
    {
        //CompSt
        add_boundary_to_symbol_list();
        analyse_comp_st(stmt->children[0], return_type);
        remove_scope_from_symbol_list();
    }
    else if (stmt->num_of_children == 3 && CHECK_TOKEN_TYPE(stmt->children[0], RETURN_T) && CHECK_NON_TYPE(stmt->children[1], Exp) && CHECK_TOKEN_TYPE(stmt->children[2], SEMI_T))
    {
        //RETURN Exp SEMI
        Type *type = analyse_exp(stmt->children[1]);
        if (type != NULL && type != return_type)
        {
            print_semantic_error(8, stmt->children[1]->first_line);
        }
    }
    else if (stmt->num_of_children == 5 && CHECK_TOKEN_TYPE(stmt->children[0], IF_T))
    {
        //IF LP Exp RP Stmt
        assert(CHECK_NON_TYPE(stmt->children[2], Exp));
        assert(CHECK_NON_TYPE(stmt->children[4], Stmt));
        analyse_exp(stmt->children[2]);
        analyse_stmt(stmt->children[4], return_type);
    }
    else if (stmt->num_of_children == 7 && CHECK_TOKEN_TYPE(stmt->children[0], IF_T))
    {
        //IF LP Exp RP Stmt ELSE Stmt
        assert(CHECK_NON_TYPE(stmt->children[2], Exp));
        assert(CHECK_NON_TYPE(stmt->children[4], Stmt));
        assert(CHECK_NON_TYPE(stmt->children[6], Stmt));
        analyse_exp(stmt->children[2]);
        analyse_stmt(stmt->children[4], return_type);
        analyse_stmt(stmt->children[6], return_type);
    }
    else if (stmt->num_of_children == 5 && CHECK_TOKEN_TYPE(stmt->children[0], WHILE_T))
    {
        //WHILE LP Exp RP Stmt
        assert(CHECK_NON_TYPE(stmt->children[2], Exp));
        assert(CHECK_NON_TYPE(stmt->children[4], Stmt));
        analyse_exp(stmt->children[2]);
        analyse_stmt(stmt->children[4], return_type);
    }
    else
    {
        assert(false);
    }
}

void analyse_ext_dec_list(TreeNode *ext_dec_list, Type *type)
{
    while (ext_dec_list->num_of_children == 1 || ext_dec_list->num_of_children == 3)
    {
        TreeNode *var_dec = ext_dec_list->children[0];

        FieldList *temp = get_var_dec(var_dec, type);
        if (look_up_variable_list(temp->name, true) != NULL || look_up_struct_list(temp->name) != NULL)
        {
            print_semantic_error(3, temp->first_line);
        }
        else
        {
            add_to_variable_list(temp->name, temp->type);
        }

        if (ext_dec_list->num_of_children == 1)
        {
            break;
        }
        else
        {
            ext_dec_list = ext_dec_list->children[2];
        }
    }
}

void analyse_fun_dec(TreeNode *fun_dec, Type *return_type)
{
    FieldList *var_list = NULL;
    if (fun_dec->num_of_children == 4) //has varlist
    {
        var_list = get_var_list(fun_dec->children[2]);
    }

    if (look_up_function_list(fun_dec->children[0]->value.str_val) != NULL)
    {
        print_semantic_error(4, fun_dec->children[0]->first_line);
    }
    else
    {
        add_to_function_list(fun_dec->children[0]->value.str_val, return_type, var_list);
    }

    add_boundary_to_symbol_list();

    while (var_list != NULL)
    {
        if (look_up_variable_list(var_list->name, true) != NULL || look_up_struct_list(var_list->name) != NULL)
        {
            print_semantic_error(3, var_list->first_line);
        }
        else
        {
            add_to_variable_list(var_list->name, var_list->type);
        }

        var_list = var_list->next;
    }
}

Type *check_assign(TreeNode *left, TreeNode *right, int first_line)
{
    //Lvalue check
    if ((left->num_of_children == 1 && CHECK_TOKEN_TYPE(left->children[0], ID_T)) || (left->num_of_children == 4 && CHECK_TOKEN_TYPE(left->children[1], LB_T)) || (left->num_of_children == 3 && CHECK_TOKEN_TYPE(left->children[1], DOT_T)))
    {
        //correct lvalue
    }
    else
    {
        print_semantic_error(6, first_line);
        return NULL;
    }

    Type *temp1 = analyse_exp(left);
    Type *temp2 = analyse_exp(right);
    if (temp1 != NULL && temp2 != NULL && !type_equal(temp1, temp2))
    {
        print_semantic_error(5, first_line);
        return NULL;
    }

    return temp1;
}

Type *check_binary_logic(TreeNode *left, TreeNode *right, int first_line)
{
    Type *one = analyse_exp(left);
    Type *two = analyse_exp(right);
    if (one == NULL || two == NULL)
    {
        return NULL;
    }
    if (one != TYPE_INT || two != TYPE_INT)
    {
        print_semantic_error(7, first_line);
        return NULL;
    }
    return one;
}

Type *check_binary_arithmetic(TreeNode *left, TreeNode *right, int first_line)
{
    Type *one = analyse_exp(left);
    Type *two = analyse_exp(right);
    if (one == NULL || two == NULL)
    {
        return NULL;
    }
    if ((one != two) || (one != TYPE_INT && one != TYPE_FLOAT))
    {
        print_semantic_error(7, first_line);
        return NULL;
    }
    return one;
}

Type *check_unary_arithmetic(TreeNode *right, int first_line)
{
    Type *one = analyse_exp(right);
    if (one == NULL)
    {
        return NULL;
    }

    if (one != TYPE_INT && one != TYPE_FLOAT)
    {
        print_semantic_error(7, first_line);
        return NULL;
    }

    return one;
}

Type *check_unary_logic(TreeNode *right, int first_line)
{
    Type *one = analyse_exp(right);
    if (one == NULL)
    {
        return NULL;
    }
    if (one != TYPE_INT)
    {
        print_semantic_error(7, first_line);
        return NULL;
    }
    return one;
}

Type *check_func_call(TreeNode *id, TreeNode *args)
{
    SymbolNode *temp = look_up_function_list(id->value.str_val);

    //check redefinition
    if (temp == NULL)
    {
        SymbolNode *var = look_up_variable_list(id->value.str_val, false);
        if (var == NULL)
        {
            print_semantic_error(2, id->first_line);
        }
        else
        {
            print_semantic_error(11, id->first_line);
        }

        return NULL;
    }

    //check args
    if (args == NULL)
    {
        if (temp->num_of_paras != 0)
        {
            print_semantic_error(9, id->first_line);
        }
    }
    else
    {
        bool mis_match = false;
        for (int i = 0; i < temp->num_of_paras; i++)
        {
            if (args == NULL || type_equal(temp->paras[i], analyse_exp(args->children[0])) == false)
            {
                mis_match = true;
                break;
            }

            if (args->num_of_children == 3)
            {
                args = args->children[2];
            }
            else
            {
                args = NULL;
            }
        }
        if (args != NULL)
        {
            mis_match = true;
        }

        if (mis_match == true)
        {
            print_semantic_error(9, args->first_line);
        }
    }
    return temp->type;
}

Type *check_array_call(TreeNode *arr, TreeNode *num)
{
    Type *temp = analyse_exp(arr);
    if (temp == NULL || temp->kind != ARRAY)
    {
        print_semantic_error(10, arr->first_line);
        return NULL;
    }

    Type *index = analyse_exp(num);
    if (index == NULL || index->kind != BASIC || index->u.basic != 0)
    {
        print_semantic_error(12, num->first_line);
        return NULL;
    }

    return temp->u.array.elem;
}

Type *check_dot(TreeNode *left, TreeNode *right)
{
    Type *temp = analyse_exp(left);
    if (temp == NULL || temp->kind != STRUCTURE)
    {
        print_semantic_error(13, left->first_line);
        return NULL;
    }

    FieldList *struct_fields = temp->u.structure->next;

    while (struct_fields != NULL)
    {
        if (strcmp(struct_fields->name, right->value.str_val) == 0)
        {
            return struct_fields->type;
        }

        struct_fields = struct_fields->next;
    }

    print_semantic_error(14, right->first_line);
    return NULL;
}

Type *check_id(TreeNode *id)
{
    SymbolNode *temp = look_up_variable_list(id->value.str_val, false);
    if (temp == NULL)
    {
        print_semantic_error(1, id->first_line);
        return NULL;
    }

    return temp->type;
}