#include "util.h"
#include "Type.h"
#include <stdlib.h>
#include <string.h>
#include "analyse_tree.h"
#include "InterCode.h"
#include "translate.h"

void setup()
{
    TYPE_INT = malloc(sizeof(*TYPE_INT));
    TYPE_INT->kind = BASIC;
    TYPE_INT->u.basic = 0;

    TYPE_FLOAT = malloc(sizeof(*TYPE_FLOAT));
    TYPE_FLOAT->kind = BASIC;
    TYPE_FLOAT->u.basic = 1;

    add_to_function_list(READ, TYPE_INT, NULL);
    FieldList *para = malloc(sizeof(*para));
    para->name = strdup("value");
    para->type = TYPE_INT;
    para->next = 0;
    para->assigned_with = NULL;
    para->first_line = 0;
    add_to_function_list(WRITE, TYPE_INT, para);
}

void print_semantic_error(int error_id, int line_num)
{
    printf("Error type %d at Line %d: Semantic error.\n", error_id, line_num);
}

char *RELOP_NO_EQUAL = "!=";
char *READ = "read";
char *WRITE = "write";

char *strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *result = (char *)malloc(len);
    if (result == (char *)0)
        return (char *)0;
    return (char *)memcpy(result, s, len);
}
