#include "reg.h"
#include "mips_translate.h"
#include "Operand.h"
#include <stdlib.h>
#include <stdarg.h>

void print_mips_code(InterCode *codes)
{
    print_header_code();

    char *x, *y, *z, *f, *relop, *left_reg, *right_reg, *result_reg, *x_reg, *y_reg;
    SymbolNode *symbol;
    while (codes != NULL)
    {
        printf("here %d\n", codes->kind);
        switch (codes->kind)
        {
        case LABEL_CODE:
            x = get_op_literal(codes->u.op);
            fprintf(dst, "%s :\n", x);
            free(x);
            break;
        case ASSIGN_CODE:
            if (codes->u.assign.right->kind == CONSTANT)
            {
                printf("here2 \n");
                left_reg = load_reg(codes->u.assign.left);
                fprintf(dst, "li %s, %d\n", left_reg, codes->u.assign.right->u.value);
                save_reg(codes->u.assign.left);
            }
            else
            {
                printf("here3 \n");
                left_reg = load_reg(codes->u.assign.left);
                printf("here3.1 \n");
                right_reg = load_reg(codes->u.assign.right);
                printf("here4 \n");
                fprintf(dst, "move %s, %s\n", left_reg, right_reg);
                printf("here5 \n");
                save_reg(codes->u.assign.left);
                printf("here6 \n");
                save_reg(codes->u.assign.right);
            }
            break;
        case PLUS_CODE:

            assert(codes->u.binop.left->kind != CONSTANT);
            if (codes->u.binop.right->kind == CONSTANT)
            {
                result_reg = load_reg(codes->u.binop.result);
                left_reg = load_reg(codes->u.binop.left);
                fprintf(dst, "addi %s, %s, %d\n", result_reg, left_reg, codes->u.binop.right->u.value);
                save_reg(codes->u.binop.result);
                save_reg(codes->u.binop.left);
            }
            else
            {
                result_reg = load_reg(codes->u.binop.result);
                left_reg = load_reg(codes->u.binop.left);
                right_reg = load_reg(codes->u.binop.right);
                fprintf(dst, "add %s, %s, %s\n", result_reg, left_reg, right_reg);
                save_reg(codes->u.binop.result);
                save_reg(codes->u.binop.left);
                save_reg(codes->u.binop.right);
            }
            break;
        case MINUS_CODE:
            assert(codes->u.binop.left->kind != CONSTANT);
            if (codes->u.binop.right->kind == CONSTANT)
            {
                result_reg = load_reg(codes->u.binop.result);
                left_reg = load_reg(codes->u.binop.left);
                fprintf(dst, "addi %s, %s, -%d\n", result_reg, left_reg, codes->u.binop.right->u.value);
                save_reg(codes->u.binop.result);
                save_reg(codes->u.binop.left);
            }
            else
            {
                result_reg = load_reg(codes->u.binop.result);
                left_reg = load_reg(codes->u.binop.left);
                right_reg = load_reg(codes->u.binop.right);
                fprintf(dst, "sub %s, %s, %s\n", result_reg, left_reg, right_reg);
                save_reg(codes->u.binop.result);
                save_reg(codes->u.binop.left);
                save_reg(codes->u.binop.right);
            }
            break;
        case STAR_CODE:
            assert(codes->u.binop.left->kind != CONSTANT);
            result_reg = load_reg(codes->u.binop.result);
            left_reg = load_reg(codes->u.assign.left);
            right_reg = load_reg(codes->u.assign.right);
            fprintf(dst, "mul %s, %s, %s\n", result_reg, left_reg, right_reg);
            save_reg(codes->u.binop.result);
            save_reg(codes->u.binop.left);
            save_reg(codes->u.binop.right);
            break;
        case DIV_CODE:
            assert(codes->u.binop.left->kind != CONSTANT);
            result_reg = load_reg(codes->u.binop.result);
            left_reg = load_reg(codes->u.binop.left);
            right_reg = load_reg(codes->u.binop.right);
            fprintf(dst, "div %s, %s\n", left_reg, right_reg);
            fprintf(dst, "mflo %s\n", result_reg);
            save_reg(codes->u.binop.result);
            save_reg(codes->u.binop.left);
            save_reg(codes->u.binop.right);
            break;
        case ASSIGN_STAR_CODE:
            left_reg = load_reg(codes->u.assign.left);
            right_reg = load_reg(codes->u.assign.right);
            fprintf(dst, "lw %s, 0(%s)\n", left_reg, right_reg);
            save_reg(codes->u.assign.left);
            save_reg(codes->u.assign.right);
            break;
        case STAR_ASSIGN_CODE:
            left_reg = load_reg(codes->u.assign.left);
            right_reg = load_reg(codes->u.assign.right);
            fprintf(dst, "sw %s, 0(%s)\n", right_reg, left_reg);
            save_reg(codes->u.assign.left);
            save_reg(codes->u.assign.right);
            break;
        case GOTO_CODE:
            x = get_op_literal(codes->u.op);
            fprintf(dst, "j %s\n", x);
            free(x);
            break;
        case CALL_CODE:
            // x = get_op_literal(codes->u.call.left);
            x_reg = load_reg(codes->u.call.left);
            f = codes->u.call.name;
            // //write
            // if (strcmp(f, "write") == 0)
            // {
            //     fprintf(dst, "lw $a0, 0($sp)\n");
            // }

            fprintf(dst, "jal %s\n", f);
            fprintf(dst, "lw $ra, 0($sp)\n");
            fprintf(dst, "addi $sp, $sp, 4\n");
            fprintf(dst, "move %s, $v0\n", x_reg);
            save_reg(codes->u.call.left);

            break;
        case RETURN_CODE:
            fprintf(dst, "addi $sp, $fp, -8\n");
            fprintf(dst, "lw $ra, 4($sp)\n");
            fprintf(dst, "lw $fp, 0($sp)\n");
            fprintf(dst, "addi $sp, $sp, 8\n");
            x_reg = load_reg(codes->u.op);
            fprintf(dst, "move $v0, %s\n", x_reg);
            fprintf(dst, "jr $ra\n");
            save_reg(codes->u.op);
            break;
        case IF_GOTO_CODE:
            relop = codes->u.if_goto.relop;
            char *instruction = NULL;
            if (strcmp(relop, "==") == 0)
            {
                instruction = "beq";
            }
            else if (strcmp(relop, "!=") == 0)
            {
                instruction = "bne";
            }
            else if (strcmp(relop, ">") == 0)
            {
                instruction = "bgt";
            }
            else if (strcmp(relop, "<") == 0)
            {
                instruction = "blt";
            }
            else if (strcmp(relop, ">=") == 0)
            {
                instruction = "bge";
            }
            else if (strcmp(relop, "<=") == 0)
            {
                instruction = "ble";
            }
            else
            {
                assert(false);
            }

            z = get_op_literal(codes->u.if_goto.dst);
            x_reg = load_reg(codes->u.if_goto.left);
            y_reg = load_reg(codes->u.if_goto.right);

            fprintf(dst, "%s %s, %s, %s\n", instruction, x_reg, y_reg, z);

            save_reg(codes->u.if_goto.left);
            save_reg(codes->u.if_goto.right);
            free(z);
            break;
        case FUNCTION_CODE:
            x = codes->u.node->name;
            fprintf(dst, "\n%s:\n", x);

            fprintf(dst, "addi $sp, $sp, -8\n");
            fprintf(dst, "sw $ra, 4($sp)\n");
            fprintf(dst, "sw $fp, 0($sp)\n");
            fprintf(dst, "addi $fp, $sp, 8\n");

            stack_offset = -12;
            param_offset = 0;
            local_vars = NULL;
            break;
        case PARAM_CODE:
            symbol = look_up_variable_list(codes->u.node->name);
            Operand *op = malloc(sizeof(*op));
            op->kind = VARIABLE;
            op->u.var_node = symbol;
            int offset = get_stack_offset(op);
            fprintf(dst, "lw $s0, %d($fp)\n", param_offset);
            param_offset += 4;
            fprintf(dst, "sw %d($fp), $s0\n", offset);
            break;
        case ARG_CODE:
            //ignore num, all in stack
            x_reg = load_reg(codes->u.op);
            fprintf(dst, "addi $sp, $sp, -4\n");
            fprintf(dst, "sw %s, 0($sp)\n", x_reg);
            save_reg(codes->u.op);
            break;
        case READ_CODE:
            fprintf(dst, "jal read\n");
            char *x_reg = load_reg(codes->u.op);
            fprintf(dst, "move %s, $v0\n", x_reg);
            save_reg(codes->u.op);
            break;
        case WRITE_CODE:
            fprintf(dst, "lw $a0, 0($sp)\n");
            fprintf(dst, "jal write\n");
            break;
        default:
        break;
        }
        codes = codes->next;
    }
}

void print_header_code()
{
    fprintf(dst, ".data\n");
    fprintf(dst, "_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(dst, "_ret: .asciiz \"\\n\"\n");
    fprintf(dst, ".globl main\n");
    fprintf(dst, ".text\n");
    fprintf(dst, "read:\n");
    // fprintf(dst, "  addi $sp, $sp, -8\n");
    // fprintf(dst, "  sw $ra, 4($sp)\n");
    // fprintf(dst, "  sw $fp, 0($sp)\n");
    // fprintf(dst, "  addi $fp, $sp, 8\n");
    fprintf(dst, "  li $v0, 4\n");
    fprintf(dst, "  la $a0, _prompt\n");
    fprintf(dst, "  syscall\n");
    fprintf(dst, "  li $v0, 5\n");
    fprintf(dst, "  syscall\n");
    // fprintf(dst, "  move $sp, $fp, -8\n");
    // fprintf(dst, "  lw $ra, 4($sp)\n");
    // fprintf(dst, "  lw $fp, 0($sp)\n");
    // fprintf(dst, "  addi $sp, $sp, 8\n");
    fprintf(dst, "  jr $ra\n");
    fprintf(dst, "write:\n");
    fprintf(dst, "  li $v0, 1\n");
    fprintf(dst, "  syscall\n");
    fprintf(dst, "  li $v0, 4\n");
    fprintf(dst, "  la $a0, _ret\n");
    fprintf(dst, "  syscall\n");
    fprintf(dst, "  move $v0, $0\n");
    fprintf(dst, "  jr $ra\n");
}