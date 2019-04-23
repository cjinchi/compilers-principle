#include "analyse_tree.h"
#include <assert.h>
#include "Type.h"

#define RIGHT_NON_TYPE(node, type) (check_nonterminal_type(node, type) == true)

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
        assert(children_num > 0 && RIGHT_NON_TYPE(childrens[0], Specifier));
        Type *type = get_type_from_specifier(childrens[0]);

        //TODO

        if (type->kind == STRUCTURE && type->u.structure->name != NULL)
        {
            /* code */
        }

        if (children_num == 3 && RIGHT_NON_TYPE(childrens[1], ExtDecList) && RIGHT_NON_TYPE(childrens[2], SEMI_T))
        {
            TreeNode *ext_dec_list = childrens[1];
            while (ext_dec_list->num_of_children == 3)
            {
                TreeNode *var_dec = ext_dec_list->children[0];

                char *var_name =

                    ext_dec_list = ext_dec_list->children[2];
            }
        }
        else if (children_num == 2 && RIGHT_NON_TYPE(childrens[1], SEMI_T))
        {
        }
        else if (children_num == 3 && RIGHT_NON_TYPE(childrens[1], FunDec) && RIGHT_NON_TYPE(childrens[2], CompSt))
        {
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