#include "Type.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "SymbolNode.h"
#include "TreeNode.h"

Type *struct_list = NULL;

Type *TYPE_INT = NULL;
Type *TYPE_FLOAT = NULL;

Type *get_type_from_specifier(TreeNode *node)
{
    assert(node->is_token == false && node->type == Specifier && node->num_of_children == 1);
    //Basic type
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
    //Struct type
    else if (node->children[0]->type == StructSpecifier)
    {
        TreeNode *struct_specifier = node->children[0];
        //Struct Tag (it was defined and reused here)
        if (struct_specifier->num_of_children == 2 && CHECK_NON_TYPE(struct_specifier->children[1], Tag))
        {
            char *tag_name = struct_specifier->children[1]->children[0]->value.str_val;
            Type *type = look_up_struct_list(tag_name);
            if (type == NULL)
            {
                //TODOERROR

                //regard it as basic type int due to error
                return TYPE_INT;
            }
            else
            {
                return type;
            }
        }
        //Stuct OptTag { ... } (it's defined here)
        else if (struct_specifier->num_of_children == 5 && CHECK_NON_TYPE(struct_specifier->children[1], OptTag))
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
                //with name, so process redefinition checking
                Type *defined_past = look_up_struct_list(type->u.structure->name);
                if (defined_past != NULL)
                {
                    //TODOERROR

                    //return the one defined earlier due to error
                    return defined_past;
                }
                else
                {
                    type->u.structure->name = opt_tag->children[0]->value.str_val;
                    add_to_struct_list(type);
                }
            }

            TreeNode *def_list = struct_specifier->children[3];
            FieldList *def_filed_list = get_def_list(def_list);
            FieldList *p = def_filed_list;
            while (p != NULL)
            {
                if (look_up_variable_list(p->name) != NULL)
                {
                    //TODOERROR
                    //It seems that can't happen here
                }
                else
                {
                    add_to_variable_list(p->name, p->type);
                }
                p = p->next;
            }

            type->u.structure->next = def_filed_list;

            return type;
        }
        else
        {
            assert(false);
        }
    }
}

Type *look_up_struct_list(char *name)
{
    Type *p = struct_list;
    while (p != NULL)
    {
        if (p->u.structure->name != NULL && strcmp(name, p->u.structure->name) == 0)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

void add_to_struct_list(Type *type)
{
    if (struct_list == NULL)
    {
        struct_list = type;
    }
    else
    {
        type->next = struct_list;
        struct_list = type;
    }
}

FieldList *get_def_list(TreeNode *def_list)
{
    assert(CHECK_NON_TYPE(def_list, DefList));

    FieldList *head = NULL;
    while (def_list->num_of_children == 2)
    {
        TreeNode *def = def_list->children[0];
        Type *def_type = get_type_from_specifier(def->children[0]);

        TreeNode *dec_list = def_list->children[1];
        assert(dec_list != NULL);
        while (dec_list->num_of_children == 3 || dec_list->num_of_children == 1)
        {
            TreeNode *dec = dec_list->children[0];

            FieldList *dec_field = get_dec(dec, def_type);
            if (head == NULL)
            {
                head = dec_field;
            }
            else
            {
                dec_field->next = head;
                head = dec_field;
            }
            // if (look_up_variable_list(dec_field->name) != NULL)
            // {
            //     //TODOERROR
            // }
            // else
            // {
            //     add_to_variable_list(dec_field->name, dec_field->type);

            // }

            if (dec_list->num_of_children == 3)
            {
                dec_list = dec_list->children[2];
            }
            else
            {
                break;
            }
        }

        def_list = def_list->children[1];
    }

    return head;
}

FieldList *get_dec(TreeNode *dec, Type *def_type)
{
    assert(dec != NULL && CHECK_NON_TYPE(dec, Dec));
    if (dec->num_of_children == 3)
    {
        //TODO assginop check
    }
    return get_var_dec(dec->children[0], def_type);
}

FieldList *get_var_dec(TreeNode *var_dec, Type *def_type)
{
    assert(var_dec != NULL && CHECK_NON_TYPE(var_dec, VarDec));
    FieldList *ret = malloc(sizeof(*ret));
    Type *type_list = def_type;

    while (var_dec->num_of_children == 4)
    {
        Type *temp = malloc(sizeof(*temp));
        temp->kind = ARRAY;
        temp->u.array.size = var_dec->children[2]->value.int_val;
        temp->u.array.elem = type_list;

        type_list = temp;

        var_dec = var_dec->children[0];
    }
    ret->name = var_dec->children[0]->value.str_val;
    ret->type = type_list;
    return ret;
}

FieldList *get_var_list(TreeNode *var_list)
{
    assert(CHECK_NON_TYPE(var_list, VarList));

    FieldList *ret = NULL;
    while (var_list->num_of_children == 1 || var_list->num_of_children == 3)
    {
        TreeNode *param_dec = var_list->children[0];
        Type *para_type = get_type_from_specifier(param_dec->children[0]);
        FieldList *temp = get_var_dec(param_dec->children[1], para_type);
        if (ret == NULL)
        {
            ret = temp;
        }
        else
        {
            temp->next = ret;
            ret = temp;
        }

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