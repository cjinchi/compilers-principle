#include "SymbolNode.h"
#include "string.h"

SymbolNode *symbol_list = NULL;

SymbolNode *look_up_symbol_list(char *name)
{
    SymbolNode *p = symbol_list;
    while (p != NULL)
    {
        if (strcmp(name, p->name) == 0)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}