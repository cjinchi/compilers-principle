#include "SymbolNode.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "util.h"

SymbolNode *symbol_list = NULL;

SymbolNode *look_up_variable_list(char *name)
{
    SymbolNode *p = symbol_list;
    while (p != NULL)
    {
        if (p->kind == VARIABLE && strcmp(name, p->name) == 0)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

void add_to_variable_list(char *name, Type *type)
{
    SymbolNode *temp = malloc(sizeof(*temp));
    temp->name = name;
    temp->kind = VARIABLE;
    temp->type = type;
    temp->op = NULL;

    if (symbol_list == NULL)
    {
        symbol_list = temp;
    }
    else
    {
        temp->next = symbol_list;
        symbol_list = temp;
    }
}

SymbolNode *look_up_function_list(char *name)
{
    SymbolNode *p = symbol_list;
    while (p != NULL)
    {
        if (p->kind == FUNCTION && strcmp(name, p->name) == 0)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

void add_to_function_list(char *name, Type *return_type, FieldList *paras)
{
    SymbolNode *temp = malloc(sizeof(*temp));
    temp->name = name;
    temp->kind = FUNCTION;
    temp->type = return_type;

    //count num of paras
    temp->num_of_paras = 0;
    FieldList *p = paras;
    while (p != NULL)
    {
        temp->num_of_paras++;

        p = p->next;
    }

    if (temp->num_of_paras != 0)
    {
        temp->paras = malloc(temp->num_of_paras * sizeof(Type *));
    }

    int index = temp->num_of_paras - 1;
    while (paras != NULL)
    {
        temp->paras[index] = paras->type;

        index--;
        paras = paras->next;
    }

    if (symbol_list == NULL)
    {
        symbol_list = temp;
    }
    else
    {
        temp->next = symbol_list;
        symbol_list = temp;
    }
}