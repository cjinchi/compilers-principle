#include "InterCode.h"
#include "type.h"
#include "stdlib.h"
#include <stdarg.h>

InterCode *ic_list_head = NULL;
InterCode *ic_list_tail = NULL;

int temp_var_id = 0;

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
