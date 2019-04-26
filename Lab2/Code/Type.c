#include "Type.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>
#include "SymbolNode.h"
#include "TreeNode.h"

Type *struct_list = NULL;

Type *TYPE_INT = NULL;
Type *TYPE_FLOAT = NULL;

FieldList *struct_field_list = NULL;

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
                print_semantic_error(17, struct_specifier->children[1]->first_line);
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
                //no name,unnecessary to check
                type->u.structure->name = NULL;
            }
            else if (opt_tag->num_of_children == 1)
            {
                //with name, process redefinition check
                if (look_up_struct_list(opt_tag->children[0]->value.str_val) != NULL || look_up_variable_list(opt_tag->children[0]->value.str_val, true) != NULL)
                {
                    print_semantic_error(16, opt_tag->first_line);
                }
                else
                {
                    type->u.structure->name = opt_tag->children[0]->value.str_val;
                    add_to_struct_list(type);
                }
            }

            TreeNode *def_list = struct_specifier->children[3];
            FieldList *def_field_list = get_def_list(def_list);
            FieldList *p = def_field_list;

            struct_field_list = NULL;

            while (p != NULL)
            {
                if (look_up_struct_field_list(p->name) != NULL)
                {
                    print_semantic_error(15, p->first_line);
                }
                else
                {
                    add_to_struct_field_list(p);
                }

                if (p->init == true)
                {
                    print_semantic_error(15, p->first_line);
                }
                p = p->next;
            }

            type->u.structure->next = def_field_list;

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
        //the struct should be just created and not traversing, so it's safe to do so
        type->next = struct_list;
        struct_list = type;
    }
}

FieldList *get_def_list(TreeNode *def_list)
{
    assert(CHECK_NON_TYPE(def_list, DefList));

    FieldList *head = NULL;
    FieldList *tail = NULL;
    while (def_list->num_of_children == 2)
    {
        TreeNode *def = def_list->children[0];

        Type *def_type = get_type_from_specifier(def->children[0]);

        TreeNode *dec_list = def->children[1];
        assert(dec_list != NULL);
        while (dec_list->num_of_children == 3 || dec_list->num_of_children == 1)
        {

            TreeNode *dec = dec_list->children[0];

            FieldList *dec_field = get_dec(dec, def_type);
            if (head == NULL)
            {
                head = dec_field;
                tail = head;
            }
            else
            {
                tail->next = dec_field;
                tail = dec_field;
            }

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
    FieldList *ret = get_var_dec(dec->children[0], def_type);
    if (dec->num_of_children == 3)
    {
        //TODO assginop check

        ret->init = true;
    }
    else
    {
        ret->init = false;
    }
    return ret;
}

FieldList *get_var_dec(TreeNode *var_dec, Type *def_type)
{
    assert(var_dec != NULL && CHECK_NON_TYPE(var_dec, VarDec));
    FieldList *ret = malloc(sizeof(*ret));
    Type *type_list = def_type;
    ret->first_line = var_dec->first_line;

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

void add_to_struct_field_list(FieldList *p)
{
    FieldList *temp = malloc(sizeof(*temp));
    memcpy(temp, p, sizeof(*temp));
    temp->next = NULL;

    if (struct_field_list == NULL)
    {
        struct_field_list = temp;
    }
    else
    {
        temp->next = struct_field_list;
        struct_field_list = temp;
    }
}

FieldList *look_up_struct_field_list(char *name)
{
    FieldList *p = struct_field_list;
    while (p != NULL)
    {
        if (strcmp(p->name, name) == 0)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}