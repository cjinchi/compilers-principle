#include "analyse_tree.h"
#include <assert.h>
#include "Type.h"
#include "SymbolNode.h"
#include "util.h"

void analyse_program(TreeNode *program)
{
    //should I check EMPTY node first???
    printf("analyse begin\n");

    CHECK_NON_TYPE(program, Program);

    TreeNode *ext_def_list = program->children[0];

    while (ext_def_list->num_of_children == 2)
    {
        printf("in extdeflist loop\n");
        TreeNode *node = ext_def_list->children[0];
        CHECK_NON_TYPE(node, ExtDef);

        printf("in case extdef\n");
        int children_num = node->num_of_children;
        TreeNode **childrens = node->children;

        assert(children_num > 0 && CHECK_NON_TYPE(childrens[0], Specifier));
        Type *type = get_type_from_specifier(childrens[0]);

        if (children_num == 3 && CHECK_NON_TYPE(childrens[1], ExtDecList) && CHECK_TOKEN_TYPE(childrens[2], SEMI_T))
        {
            //variable definition
            TreeNode *ext_dec_list = childrens[1];
            while (ext_dec_list->num_of_children == 1 || ext_dec_list->num_of_children == 3)
            {
                TreeNode *var_dec = ext_dec_list->children[0];

                FieldList *temp = get_var_dec(var_dec, type);
                if (look_up_variable_list(temp->name) != NULL || look_up_struct_list(temp->name) != NULL)
                {
                    printf("error 3\n");
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
        else if (children_num == 2 && CHECK_TOKEN_TYPE(childrens[1], SEMI_T))
        {
            //do nothing
            //struct definiton has been checked in get_type_from_specifier
        }
        else if (children_num == 3 && CHECK_NON_TYPE(childrens[1], FunDec) && CHECK_NON_TYPE(childrens[2], CompSt))
        {
            //func definition
            printf("in func\n");
            //FuncDec
            TreeNode *fun_dec = childrens[1];
            if (look_up_function_list(fun_dec->children[0]->value.str_val) != NULL)
            {
                //TODOERROR
                printf("error 4\n");
            }
            else
            {
                FieldList *var_list = NULL;
                if (fun_dec->num_of_children == 4) //has varlist
                {
                    var_list = get_var_list(fun_dec->children[2]);
                }
                add_to_function_list(fun_dec->children[0]->value.str_val, type, var_list);

                //TODO:mark as boundary here
                while (var_list != NULL)
                {
                    if (look_up_variable_list(var_list->name) != NULL)
                    {
                        //TODOERROR
                    }
                    else
                    {
                        add_to_variable_list(var_list->name, var_list->type);
                    }
                }
            }

            //dont't mark boundary here
            analyse_comp_st(childrens[2]);
        }
        else
        {
            assert(false);
        }

        ext_def_list = ext_def_list->children[1];
    }
}

void analyse_comp_st(TreeNode *comp_st)
{
    CHECK_NON_TYPE(comp_st, CompSt);
    //-- DefList -
    TreeNode *def_list = comp_st->children[1];
    FieldList *def_field_list = get_def_list(def_list);
    FieldList *p = def_field_list;
    while (p != NULL)
    {
        if (look_up_variable_list(p->name) != NULL || look_up_struct_list(p->name) != NULL)
        {
            printf("error 3\n");
            //TODOERROR
        }
        else
        {
            add_to_variable_list(p->name, p->type);
        }
        p = p->next;
    }
    //-- StmtList
    TreeNode *stmt_list = comp_st->children[2];
    while (stmt_list->num_of_children == 2)
    {
        TreeNode *stmt = stmt_list->children[0];

        CHECK_NON_TYPE(stmt, Stmt);
        if (stmt->num_of_children == 2 && CHECK_NON_TYPE(stmt->children[0], Exp) && CHECK_TOKEN_TYPE(stmt->children[1], SEMI_T))
        {
            analyse_exp(stmt->children[0]);
        }
        else if (stmt->num_of_children == 1 && CHECK_NON_TYPE(stmt->children[0], CompSt))
        {
        }
        else if (stmt->num_of_children == 3 && CHECK_TOKEN_TYPE(stmt->children[0], RETURN_T) && CHECK_NON_TYPE(stmt->children[1], Exp) && CHECK_TOKEN_TYPE(stmt->children[2], SEMI_T))
        {
        }
        else
        {
            assert(false);
        }

        stmt_list = stmt_list->children[1];
    }
}

Type *analyse_exp(TreeNode *exp)
{
    assert(CHECK_NON_TYPE(exp, Exp));

    if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], ASSIGNOP_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_NON_TYPE(exp->children[2], Exp));
        if (analyse_exp(exp->children[0]) != analyse_exp(exp->children[2]))
        {
            printf("error 5\n");
        }
        return analyse_exp(exp->children[0]);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], AND_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        return analyse_exp(exp->children[0]);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], OR_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        return analyse_exp(exp->children[0]);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], RELOP_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        return analyse_exp(exp->children[0]);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], PLUS_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        return analyse_exp(exp->children[0]);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], MINUS_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        return analyse_exp(exp->children[0]);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], STAR_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        return analyse_exp(exp->children[0]);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], DIV_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        return analyse_exp(exp->children[0]);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[0], LP_T) && CHECK_TOKEN_TYPE(exp->children[2], RP_T))
    {
        assert(CHECK_NON_TYPE(exp->children[1], Exp));
        return analyse_exp(exp->children[1]);
    }
    else if (exp->num_of_children == 2 && CHECK_TOKEN_TYPE(exp->children[0], MINUS_T))
    {
        assert(CHECK_NON_TYPE(exp->children[1], Exp));
        return analyse_exp(exp->children[1]);
    }
    else if (exp->num_of_children == 2 && CHECK_TOKEN_TYPE(exp->children[0], NOT_T))
    {
        assert(CHECK_NON_TYPE(exp->children[1], Exp));
        return analyse_exp(exp->children[1]);
    }
    else if (exp->num_of_children == 4 && CHECK_TOKEN_TYPE(exp->children[0], ID_T) && CHECK_TOKEN_TYPE(exp->children[1], LP_T) && CHECK_NON_TYPE(exp->children[2], Args) && CHECK_TOKEN_TYPE(exp->children[3], RP_T))
    {
        //TODO check func call
        SymbolNode *temp = look_up_function_list(exp->children[0]->value.str_val);
        if (temp == NULL)
        {
            SymbolNode *var = look_up_variable_list(exp->children[0]->value.str_val);
            if (var == NULL)
            {
                printf("error 2\n");
            }
            else
            {
                printf("error 11\n");
            }

            return NULL;
        }
        //TODO check

        return temp->type;
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[0], ID_T) && CHECK_TOKEN_TYPE(exp->children[1], LP_T) && CHECK_TOKEN_TYPE(exp->children[2], RP_T))
    {
        //TODO check func call
        SymbolNode *temp = look_up_function_list(exp->children[0]->value.str_val);
        if (temp == NULL)
        {
            SymbolNode *var = look_up_variable_list(exp->children[0]->value.str_val);
            if (var == NULL)
            {
                printf("error 2\n");
            }
            else
            {
                printf("error 11\n");
            }

            return NULL;
        }
        //TODO check

        return temp->type;
    }
    else if (exp->num_of_children == 4 && CHECK_NON_TYPE(exp->children[0], Exp) && CHECK_TOKEN_TYPE(exp->children[1], LB_T) && CHECK_NON_TYPE(exp->children[2], Exp) && CHECK_TOKEN_TYPE(exp->children[3], RB_T))
    {
        Type *temp = analyse_exp(exp->children[0]);
        if (temp == NULL || temp->kind != ARRAY)
        {
            printf("error 10\n");
        }

        Type *index = analyse_exp(exp->children[2]);
        if (index == NULL || index->kind != BASIC || index->u.basic != 0)
        {
            printf("error 12\n");
        }

        //TODO check temp's type
        return temp->u.array.elem;
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], DOT_T))
    {
        assert(CHECK_NON_TYPE(exp->children[0], Exp));
        assert(CHECK_TOKEN_TYPE(exp->children[2], ID_T));

        Type *temp = analyse_exp(exp->children[0]);
        if (temp == NULL || temp->kind != STRUCTURE)
        {
            printf("error 13\n");
            //todo can i return?
            return NULL;
        }

        FieldList *struct_fields = temp->u.structure;
        while (struct_fields != NULL)
        {
            if (strcmp(struct_fields->name, exp->children[2]->value.str_val) == 0)
            {
                return struct_fields->type;
            }

            struct_fields = struct_fields->next;
        }

        printf("error 14\n");
        return NULL;
    }
    else if (exp->num_of_children == 1 && CHECK_TOKEN_TYPE(exp->children[0], ID_T))
    {
        //todo check
        SymbolNode *temp = look_up_variable_list(exp->children[0]->value.str_val);
        if (temp == NULL)
        {
            printf("error 1\n");
            return NULL;
        }
        // todo check

        return temp->type;
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