#include "analyse_tree.h"
#include <assert.h>
#include "Type.h"
#include "SymbolNode.h"
#include "util.h"

void analyse_tree(TreeNode *program)
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

        if (children_num == 3 && CHECK_NON_TYPE(childrens[1], ExtDecList) && CHECK_NON_TYPE(childrens[2], SEMI_T))
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

            //- CompSt
            TreeNode *comp_st = childrens[2];
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

                //TODO check stmt here

                stmt_list = stmt_list->children[1];
            }
        }
        else
        {
            assert(false);
        }

        ext_def_list = ext_def_list->children[1];
    }
}