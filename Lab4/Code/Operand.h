#ifndef OPERAND_H
#define OPERAND_H

#include "SymbolNode.h"

typedef struct Operand_ Operand;
typedef struct OperandWrapper_ OperandWrapper;

typedef struct SymbolNode_t SymbolNode;

struct Operand_
{
    enum
    {
        TEMP_VARIABLE,
        REAL_VARIABLE,
        CONSTANT
        // ADDRESS
    } kind;
    union {
        //for temp_var
        int var_no;
        //for constant
        int value;
        //for real_var
        SymbolNode *var_node;
        // //for address
        // Operand *addr;
    } u;

    bool is_struct_arg;

    //for some case
    Operand *next;
};

struct OperandWrapper_
{
    Operand *head;
};

Operand *concat_operands(int num, ...);
char *get_op_literal(Operand *op);

#endif