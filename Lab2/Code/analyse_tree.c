#include "analyse_tree.h"
#include <assert.h>
#include "Type.h"
#include "SymbolNode.h"

void analyse_tree(TreeNode *node)
{
    //should I check EMPTY node first???

    if (node->is_token)
    {
        return;
    }

    switch (node->type)
    {
        int children_num = node->num_of_children;
        TreeNode **childrens = node->children;
    case ExtDef:
        assert(children_num > 0 && CHECK_NON_TYPE(childrens[0], Specifier));
        Type *type = get_type_from_specifier(childrens[0]);

        if (children_num == 3 && CHECK_NON_TYPE(childrens[1], ExtDecList) && CHECK_NON_TYPE(childrens[2], SEMI_T))
        {
            TreeNode *ext_dec_list = childrens[1];
            while (ext_dec_list->num_of_children == 1 || ext_dec_list->num_of_children == 3)
            {
                TreeNode *var_dec = ext_dec_list->children[0];

                FieldList *temp = get_var_dec(var_dec, type);
                if (look_up_variable_list(temp->name) != NULL)
                {
                    //TODOERROR
                }
                else
                {
                    add_to_variable_list(temp->name, temp->type);
                }
            }
        }
        else if (children_num == 2 && CHECK_NON_TYPE(childrens[1], SEMI_T))
        {
            //do nothing
            //struct definiton has been checked in get_type_from_specifier
        }
        else if (children_num == 3 && CHECK_NON_TYPE(childrens[1], FunDec) && CHECK_NON_TYPE(childrens[2], CompSt))
        {
            TreeNode *fun_dec = childrens[1];
            if (look_up_function_list(fun_dec->children[0]->value.str_val) != NUll)
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
                    //TODO look up list first
                    add_to_variable_list(var_list->name, var_list->type);
                }
            }
        }
        else
        {
            assert(false);
        }

        break;

    default:
        break;
    }
}