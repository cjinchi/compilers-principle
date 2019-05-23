#ifndef INTER_CODE_H
#define INTER_CODE_H

#include "TreeNode.h"
#include "Operand.h"

typedef struct InterCode_ InterCode;

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
            char *name;
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

Operand *new_temp_op();

Operand *new_constant_op(int n);

Operand *new_real_var_op(char *name);

Operand *new_label();

InterCode *new_label_code(Operand *op);
InterCode *new_goto_code(Operand *op);
InterCode *new_return_code(Operand *op);

InterCode *new_read_code(Operand *op);

InterCode *new_call_code(Operand *left, char *name);

InterCode *new_write_code(Operand *op);

InterCode *new_arg_code(Operand *op);

InterCode *new_function_code(char *name);

InterCode *new_param_code(char *name);

InterCode *new_arithmetic_code(int kind, Operand *result, Operand *left, Operand *right);

InterCode *new_if_goto_code(Operand *left, Operand *right, char *relop, Operand *dst);
void print_codes(InterCode *codes);

#endif