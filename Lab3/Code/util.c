#include "util.h"
#include "Type.h"
#include <stdlib.h>

void setup()
{
    TYPE_INT = malloc(sizeof(*TYPE_INT));
    TYPE_INT->kind = BASIC;
    TYPE_INT->u.basic = 0;

    TYPE_FLOAT = malloc(sizeof(*TYPE_FLOAT));
    TYPE_FLOAT->kind = BASIC;
    TYPE_FLOAT->u.basic = 1;
}

void print_semantic_error(int error_id, int line_num)
{
    printf("Error type %d at Line %d: Semantic error.\n", error_id, line_num);
}

char *RELOP_NO_EQUAL = "!=";
char *READ = "read";
char *WRITE = "write";
