#include "reg.h"
#include <stdlib.h>
#include <string.h>
#include "util.h"

LocalVar *local_vars = NULL;
int stack_offset = -12;
int param_offset = 0;

int reg_occupy[REG_NUM] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
char *reg_name[REG_NUM] = {"$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9"};
int reg_offset[REG_NUM] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

char *load_reg(Operand *op)
{
    int addr = (int)op;
    for (int i = 0; i < REG_NUM; i++)
    {
        printf("loop\n");
        if (reg_occupy[i] < 0)
        {
            printf("inside \n");
            reg_occupy[i] = addr;
            if (op->kind == CONSTANT)
            {
                printf("i1 \n");
                fprintf(dst, "li %s, %d\n", reg_name[i], op->u.value);
            }
            else
            {
                printf("i2 \n");
                reg_offset[i] = get_stack_offset(op);
                fprintf(dst, "lw %s, %d($fp)\n", reg_name[i], reg_offset[i]);
            }
            printf("return \n");
            return reg_name[i];
        }
    }
}
void save_reg(Operand *op)
{
    int addr = (int)op;
    for (int i = 0; i < REG_NUM; i++)
    {
        if (reg_occupy[i] == addr)
        {
            reg_occupy[i] = -1;
            if (op->kind != CONSTANT)
            {
                fprintf(dst, "sw %s, %d($fp)\n", reg_name[i], reg_offset[i]);
            }
            break;
        }
    }
}

int get_stack_offset(Operand *op)
{
    assert(op->kind != CONSTANT);

    char *name = get_op_literal(op);
    printf("h1\n");
    while (local_vars != NULL)
    {
        if (strcmp(local_vars->name, name) == 0)
        {
            free(name);
            return local_vars->offset;
        }
        local_vars = local_vars->next;
    }
    printf("h2\n");
    

    //Not found
    LocalVar *temp = malloc(sizeof(*temp));
    temp->next = NULL;
    temp->name = name;
    temp->offset = stack_offset;
    stack_offset -= 4;
    temp->next = local_vars;
    local_vars = temp;
    fprintf(dst, "addi $sp, $sp, -4\n");

    return temp->offset;
}
