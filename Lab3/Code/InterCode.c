#include "InterCode.h"
#include "Type.h"
#include <stdlib.h>
#include <stdarg.h>

InterCode *ic_list_head = NULL;
InterCode *ic_list_tail = NULL;
FILE *dst = NULL;

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
    ret->is_struct_arg = false;
    temp_var_id++;
    ret->next = NULL;
    return ret;
}

Operand *new_constant_op(int n)
{
    Operand *ret = malloc(sizeof(*ret));
    ret->kind = CONSTANT;
    ret->u.value = n;
    ret->is_struct_arg = false;
    ret->next = NULL;
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
        ret->is_struct_arg = false;
        ret->next = NULL;
        temp->op = ret;
        return ret;
    }
}

Operand *new_label()
{
    return new_temp_op();
}

InterCode *new_label_code(Operand *op)
{
    InterCode *code = new_inter_code(LABEL_CODE);
    code->u.op = op;
    return code;
}
InterCode *new_goto_code(Operand *op)
{
    InterCode *code = new_inter_code(GOTO_CODE);
    code->u.op = op;
    return code;
}
InterCode *new_return_code(Operand *op)
{
    InterCode *code = new_inter_code(RETURN_CODE);
    code->u.op = op;
    return code;
}

InterCode *new_read_code(Operand *op)
{
    InterCode *code = new_inter_code(READ_CODE);
    code->u.op = op;
    return code;
}

InterCode *new_call_code(Operand *left, char *name)
{
    InterCode *code = new_inter_code(CALL_CODE);
    code->u.call.left = left;
    code->u.call.name = name;
    return code;
}

InterCode *new_write_code(Operand *op)
{
    InterCode *code = new_inter_code(WRITE_CODE);
    code->u.op = op;
    return code;
}

InterCode *new_arg_code(Operand *op)
{
    InterCode *code = new_inter_code(ARG_CODE);
    code->u.op = op;
    return code;
}

InterCode *new_function_code(char *name)
{
    InterCode *ret = new_inter_code(FUNCTION_CODE);
    ret->u.node = look_up_function_list(name);
    return ret;
}

InterCode *new_param_code(char *name)
{
    InterCode *ret = new_inter_code(PARAM_CODE);
    ret->u.node = look_up_variable_list(name);
    return ret;
}

InterCode *new_arithmetic_code(int kind, Operand *result, Operand *left, Operand *right)
{
    InterCode *ret = new_inter_code(kind);
    ret->u.binop.result = result;
    ret->u.binop.left = left;
    ret->u.binop.right = right;
    return ret;
}

InterCode *new_if_goto_code(Operand *left, Operand *right, char *relop, Operand *dst)
{
    InterCode *ret = new_inter_code(IF_GOTO_CODE);
    ret->u.if_goto.left = left;
    ret->u.if_goto.right = right;
    ret->u.if_goto.relop = relop;
    ret->u.if_goto.dst = dst;
    return ret;
}

char code_buffer[300];
void print_codes(InterCode *codes)
{
    char *x, *y, *z, *f, *relop;
    while (codes != NULL)
    {
        switch (codes->kind)
        {
        case LABEL_CODE:
            x = get_op_literal(codes->u.op);
            sprintf(code_buffer, "LABEL %s :", x);
            free(x);
            break;
        case FUNCTION_CODE:
            x = codes->u.node->name;
            sprintf(code_buffer, "FUNCTION %s :", x);

            break;
        case ASSIGN_CODE:
            x = get_op_literal(codes->u.assign.left);
            y = get_op_literal(codes->u.assign.right);
            sprintf(code_buffer, "%s := %s", x, y);
            free(x);
            free(y);
            break;
        case PLUS_CODE:
            x = get_op_literal(codes->u.binop.result);
            y = get_op_literal(codes->u.binop.left);
            z = get_op_literal(codes->u.binop.right);
            sprintf(code_buffer, "%s := %s + %s", x, y, z);
            free(x);
            free(y);
            free(z);
            break;
        case MINUS_CODE:
            x = get_op_literal(codes->u.binop.result);
            y = get_op_literal(codes->u.binop.left);
            z = get_op_literal(codes->u.binop.right);
            sprintf(code_buffer, "%s := %s - %s", x, y, z);
            free(x);
            free(y);
            free(z);
            break;
        case STAR_CODE:
            x = get_op_literal(codes->u.binop.result);
            y = get_op_literal(codes->u.binop.left);
            z = get_op_literal(codes->u.binop.right);
            sprintf(code_buffer, "%s := %s * %s", x, y, z);
            free(x);
            free(y);
            free(z);
            break;
        case DIV_CODE:
            x = get_op_literal(codes->u.binop.result);
            y = get_op_literal(codes->u.binop.left);
            z = get_op_literal(codes->u.binop.right);
            sprintf(code_buffer, "%s := %s / %s", x, y, z);
            free(x);
            free(y);
            free(z);
            break;
            break;
        case GOTO_CODE:
            x = get_op_literal(codes->u.op);
            sprintf(code_buffer, "GOTO %s", x);
            free(x);
            break;
        case IF_GOTO_CODE:
            x = get_op_literal(codes->u.if_goto.left);
            relop = codes->u.if_goto.relop;
            y = get_op_literal(codes->u.if_goto.right);
            z = get_op_literal(codes->u.if_goto.dst);
            sprintf(code_buffer, "IF %s %s %s GOTO %s", x, relop, y, z);
            free(x);
            free(y);
            free(z);
            break;
        case RETURN_CODE:
            x = get_op_literal(codes->u.op);
            sprintf(code_buffer, "RETURN %s", x);
            free(x);
            break;
        case DEC_SIZE_CODE:
            x = get_op_literal(codes->u.dec.op);
            sprintf(code_buffer, "DEC %s %d", x, codes->u.dec.size);
            free(x);
            break;
        case ARG_CODE:
            x = get_op_literal(codes->u.op);
            if (codes->u.op->is_struct_arg == true)
            {
                sprintf(code_buffer, "ARG &%s", x);
            }
            else
            {
                sprintf(code_buffer, "ARG %s", x);
            }

            free(x);
            break;
        case CALL_CODE:
            x = get_op_literal(codes->u.call.left);
            f = codes->u.call.name;
            sprintf(code_buffer, "%s := CALL %s", x, f);
            free(x);
            break;
        case PARAM_CODE:
            x = codes->u.node->name;
            //'r' for real varible (not temp), same implement in get_op_literal
            sprintf(code_buffer, "PARAM r%s", x);
            break;
        case READ_CODE:
            x = get_op_literal(codes->u.op);
            sprintf(code_buffer, "READ %s", x);
            free(x);
            break;
        case WRITE_CODE:
            x = get_op_literal(codes->u.op);
            sprintf(code_buffer, "WRITE %s", x);
            free(x);
            break;
        case ASSIGN_AND_CODE:
            x = get_op_literal(codes->u.binop.result);
            y = get_op_literal(codes->u.binop.left);
            z = get_op_literal(codes->u.binop.right);

            sprintf(code_buffer, "%s := &%s + %s", x, y, z);
            free(x);
            free(y);
            free(z);

            break;
        case STAR_ASSIGN_CODE:
            x = get_op_literal(codes->u.assign.left);
            y = get_op_literal(codes->u.assign.right);
            sprintf(code_buffer, "*%s := %s", x, y);
            free(x);
            free(y);
            break;
        case ASSIGN_STAR_CODE:
            x = get_op_literal(codes->u.assign.left);
            y = get_op_literal(codes->u.assign.right);
            sprintf(code_buffer, "%s := *%s", x, y);
            free(x);
            free(y);
            break;
        default:
            assert(false);
            break;
        }
        fprintf(dst, "%s\n", code_buffer);
        codes = codes->next;
    }
}

InterCode *new_dec_size_code(Operand *op, int size)
{
    InterCode *ret = new_inter_code(DEC_SIZE_CODE);
    ret->u.dec.op = op;
    ret->u.dec.size = size;
    return ret;
}

InterCode *new_assign_and_code(Operand *result, Operand *addr, Operand *offset)
{
    InterCode *ret = new_inter_code(ASSIGN_AND_CODE);
    ret->u.binop.result = result;
    ret->u.binop.left = addr;
    ret->u.binop.right = offset;
    return ret;
}

InterCode *new_star_assign_code(Operand *left, Operand *right)
{
    InterCode *ret = new_inter_code(STAR_ASSIGN_CODE);
    ret->u.assign.left = left;
    ret->u.assign.right = right;
    return ret;
}

InterCode *new_assign_star_code(Operand *left, Operand *right)
{
    InterCode *ret = new_inter_code(ASSIGN_STAR_CODE);
    ret->u.assign.left = left;
    ret->u.assign.right = right;
    return ret;
}