#include "Type.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "SymbolNode.h"

Type *struct_list = NULL;

Type *TYPE_INT = NULL;
Type *TYPE_FLOAT = NULL;

Type *get_type_from_specifier(TreeNode *node)
{
    assert(node->is_token == false && node->type == Specifier && node->num_of_children == 1);
    if (node->children[0]->type == TYPE_T)
    {
        if (strcmp(node->children[0]->value.str_val, "int") == 0)
        {
            return TYPE_INT;
        }
        else
        {
            return TYPE_FLOAT;
        }
    }
    else if (node->children[0]->type == StructSpecifier)
    {
        TreeNode *struct_specifier = node->children[0];
        if (struct_specifier->num_of_children == 2 && struct_specifier->children[1]->is_token == false && struct_specifier->children[1]->type == Tag)
        {
            char *tag_name = struct_specifier->children[1]->children[0]->value.str_val;
            Type *type = look_up_struct_list(tag_name);
            if (type == NULL)
            {
                //TODOERROR
            }
            else
            {
                return type;
            }
        }
        else if (struct_specifier->num_of_children == 5 && struct_specifier->children[1]->is_token == false && struct_specifier->children[1]->type == OptTag)
        {
            TreeNode *opt_tag = struct_specifier->children[1];
            Type *type = malloc(sizeof(*type));
            type->kind = STRUCTURE;
            type->u.structure = malloc(sizeof(*(type->u.structure)));
            type->u.structure->type = NULL; //first field is name of structure
            type->u.structure->next = NULL;
            if (opt_tag->num_of_children == 0)
            {
                //no name,unnecessary to check and record
                type->u.structure->name = NULL;
            }
            else if (opt_tag->num_of_children == 1)
            {
                type->u.structure->name = opt_tag->children[0]->value.str_val;
            }

            //TODO
            TreeNode *def_list = NULL;

            return type;
        }
        else
        {
            assert(false);
        }
    }
}