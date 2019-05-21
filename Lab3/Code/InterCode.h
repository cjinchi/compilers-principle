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
        int var_no;
        int value;
        SymbolNode *var_node;
    } u;
};
struct InterCode_
{
    enum
    {
        LABEL_CODE,
        FUNCTION_CODE,
        ASSIGN_CODE,
        ADD_CODE,
        SUB_CODE,
        MUL_CODE,
        DIV_CODE,
        ASSIGN_AND_CODE,
        ASSIGN_STAR_CODE,
        STAR_ASSIGN_CODE,
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
            Operand *right, *left;
        } assign;
        struct
        {
            Operand *result, *op1, *op2;
        } binop;
        SymbolNode *node;
    } u;

    InterCode *next;
};

extern InterCode *ic_list_head;
extern InterCode *ic_list_tail;

extern int temp_var_id;

InterCode *new_inter_code(int kind);
InterCode *concat_inter_codes(int num, ...);

#endif