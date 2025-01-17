#ifndef SYMBOL_NODE_H
#define SYMBOL_NODE_H

#include "Type.h"
#include "Operand.h"

typedef struct FieldList_ FieldList;
typedef struct Operand_ Operand;
typedef struct SymbolNode_t SymbolNode;
typedef struct Type_ Type;

struct SymbolNode_t
{
    //Part one: common
    char *name;
    enum
    {
        VARIABLE,
        FUNCTION
    } kind;
    Type *type;                //type for variable OR return type for function
    struct SymbolNode_t *next; //for linked-list

    //Part two: function only
    int num_of_paras;
    Type **paras; //array, paras[i] means i-th para

    //Part three: variable only
    Operand *op;
};

extern SymbolNode *symbol_list;

SymbolNode *look_up_variable_list(char *name);

void add_to_variable_list(char *name, Type *type);

SymbolNode *look_up_function_list(char *name);

void add_to_function_list(char *name, Type *return_type, FieldList *paras);
#endif
