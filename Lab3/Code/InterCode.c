#include "InterCode.h"
#include "type.h"
#include "stdlib.h"
#include <stdarg.h>

InterCode *ic_list_head = NULL;
InterCode *ic_list_tail = NULL;

int temp_var_id = 1;

InterCode *new_inter_code(int kind)
{
    InterCode *ret = malloc(sizeof(*ret));
    ret->kind = kind;
    ret->next = NULL;
    return ret;
}

InterCode *concat_inter_codes(int num, ...)
{
    InterCode *head = NULL, *tail = NULL;

    va_list ap;
    va_start(ap, num);

    for (int i = 0; i < num; i++)
    {
        InterCode *code = va_arg(ap, InterCode *);
        if (code == NULL)
        {
            continue;
        }

        if (head == NULL)
        {
            head = code;
            tail = code;
        }
        else
        {
            tail->next = code;
        }
        while (tail->next != NULL)
        {
            tail = tail->next;
        }
    }

    va_end(ap);

    return head;
}

InterCode *new_assign_code(Operand *left, Operand *right)
{
    InterCode *ret = new_inter_code(ASSIGN_CODE);
    ret->u.assign.left = left;
    ret->u.assign.right = right;
    return ret;
}

Operand *new_temp_op()
{
    Operand *ret = malloc(sizeof(*ret));
    ret->kind = TEMP_VARIABLE;
    ret->u.var_no = temp_var_id;
    temp_var_id++;
    return ret;
}

Operand *new_constant_op(int n)
{
    Operand *ret = malloc(sizeof(*ret));
    ret->kind = CONSTANT;
    ret->u.value = n;
    return ret;
}

Operand *new_real_var_op(char *name)
{
    SymbolNode *temp = look_up_variable_list(name);
    if (temp->op != NULL)
    {
        //Singleton
        return temp->op;
    }
    else
    {
        Operand *ret = malloc(sizeof(*ret));
        ret->kind = REAL_VARIABLE;
        ret->u.var_node = temp;
        temp->op = ret;
        return ret;
    }
}