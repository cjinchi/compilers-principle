#ifndef REG_H
#define REG_H

#include "Operand.h"

char *load_reg(Operand* op);
void save_reg(Operand* op);
int get_stack_offset(Operand* op);

#define REG_NUM 10
extern int reg_occupy[REG_NUM];
extern char* reg_name[REG_NUM];

extern FILE* dst;

typedef struct LocalVar_ LocalVar;
struct LocalVar_
{
    char *name;
    int offset;

    LocalVar* next;
};

extern LocalVar *local_vars;
extern int stack_offset;
extern int param_offset;

#endif