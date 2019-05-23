#include "Operand.h"
#include "util.h"
#include <stdarg.h>
#include <string.h>

Operand *concat_operands(int num, ...)
{
    Operand *head = NULL, *tail = NULL;

    va_list ap;
    va_start(ap, num);

    for (int i = 0; i < num; i++)
    {
        Operand *op = va_arg(ap, Operand *);
        if (op == NULL)
        {
            continue;
        }

        if (head == NULL)
        {
            head = op;
            tail = op;
        }
        else
        {
            tail->next = op;
        }
        while (tail->next != NULL)
        {
            tail = tail->next;
        }
    }

    va_end(ap);

    return head;
}

char op_literal_buffer[100];
char *get_op_literal(Operand *op)
{
    switch (op->kind)
    {
    case TEMP_VARIABLE:
        sprintf(op_literal_buffer, "t%d", op->u.var_no);
        break;
    case REAL_VARIABLE:
        sprintf(op_literal_buffer, "r%s", op->u.var_node->name);
        break;
    case CONSTANT:
        sprintf(op_literal_buffer, "#%d", op->u.value);
        break;
    case ADDRESS:
        assert(false);
        break;

    default:
        assert(false);
        break;
    }
    return strdup(op_literal_buffer);
}