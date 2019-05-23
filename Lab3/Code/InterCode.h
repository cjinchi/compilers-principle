#ifndef INTER_CODE_H
#define INTER_CODE_H

#include "TreeNode.h"
#include "SymbolNode.h"

typedef struct Operand_ Operand;
typedef struct InterCode_ InterCode;

struct Operand_
{
    enum
    {
        TEMP_VARIABLE,
        REAL_VARIABLE,
        CONSTANT,
        ADDRESS
    } kind;
    union {
        //for temp_var
        int var_no;
        //for constant
        int value;
        //for real_var
        SymbolNode *var_node;
        //for address
        Operand *addr;
    } u;
};
struct InterCode_
{
    enum
    {
        LABEL_CODE,
        FUNCTION_CODE,
        ASSIGN_CODE,
        PLUS_CODE,
        MINUS_CODE,
        STAR_CODE,
        DIV_CODE,
        // ASSIGN_AND_CODE,
        // ASSIGN_STAR_CODE,
        // STAR_ASSIGN_CODE,
        GOTO_CODE,
        IF_GOTO_CODE,
        RETURN_CODE,
        DEC_SIZE_CODE,
        ARG_CODE,
        CALL_CODE,
        PARAM_CODE,
        READ_CODE,
        WRITE_CODE

    } kind;
    union {
        struct
        {
            Operand *left, *right;
        } assign;
        struct
        {
            Operand *result, *left, *right;
        } binop;
        SymbolNode *node;
        Operand *op;
        struct
        {
            Operand *left;
            char *relop;
            Operand *right;
            Operand *dst;
        } if_goto;
        struct
        {
            Operand *left;
            Operand *right;
        } call;
    } u;

    InterCode *next;
};

extern InterCode *ic_list_head;
extern InterCode *ic_list_tail;

extern int temp_var_id;

InterCode *new_inter_code(int kind);
InterCode *concat_inter_codes(int num, ...);
InterCode *new_assign_code(Operand *left, Operand *right);
InterCode *new_label_code(Operand *op);
InterCode *new_goto_code(Operand *op);
InterCode *new_return_code(Operand *op);
InterCode *new_read_code(Operand *op);
InterCode *new_call_code(Operand *left, Operand *right);

Operand *new_temp_op();
Operand *new_constant_op(int n);
Operand *new_real_var_op(char *name);
Operand *new_label();

#endif