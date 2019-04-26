#ifndef SYMBOL_NODE_H
#define SYMBOL_NODE_H

#include "type.h"

struct SymbolNode_t
{
    //Part one: common
    char *name;
    enum
    {
        VARIABLE,
        FUNCTION,
        BOUNDARY
    } kind;
    Type *type;                //type for variable OR return type for function
    struct SymbolNode_t *next; //for linked-list

    //Part two: function only
    int num_of_paras;
    Type **paras; //array, paras[i] means i-th para
};

typedef struct SymbolNode_t SymbolNode;

extern SymbolNode *symbol_list;

SymbolNode *look_up_variable_list(char *name, bool bound_sensitive);

void add_to_variable_list(char *name, Type *type);

SymbolNode *look_up_function_list(char *name);

void add_to_function_list(char *name, Type *return_type, FieldList *paras);

void add_boundary_to_symbol_list();

void remove_scope_from_symbol_list();
#endif
