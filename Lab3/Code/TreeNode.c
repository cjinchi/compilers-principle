#include "TreeNode.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "util.h"
#include "analyse_tree.h"
#include "translate.h"

TreeNode *create_token_node(int t_type, int first_line)
{
    TreeNode *node = malloc(sizeof(*node));
    node->is_token = true;
    node->type = (int)t_type;
    node->first_line = first_line;
    return node;
}

TreeNode *create_nonterminal_node(int n_type, int first_line, int num_of_children, ...)
{
    TreeNode *node = malloc(sizeof(*node));
    node->num_of_children = num_of_children;
    node->is_token = false;
    node->type = n_type;
    node->first_line = first_line;
    node->children = malloc(num_of_children * sizeof(TreeNode *));

    va_list ap;
    va_start(ap, num_of_children);
    for (int i = 0; i < num_of_children; i++)
    {
        node->children[i] = va_arg(ap, TreeNode *);
    }
    va_end(ap);

    return node;
}

TreeNode *create_error_node(int first_line)
{
    return create_nonterminal_node(Error, first_line, 0);
}

void print_tree(TreeNode *head)
{
    assert(head->is_token == false && head->type == Program);

    print_node(head, 0);
}

void print_node(TreeNode *node, int depth)
{
    if (node->num_of_children == 0 && !node->is_token)
    {
        //maybe empty,check
        int t = node->type;
        if (t == ExtDefList || t == OptTag || t == StmtList || t == DefList)
        {
            return;
        }
    }

    //main part
    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }
    printf("%s\n", get_literal(node));

    for (int i = 0; i < node->num_of_children; i++)
    {
        print_node(node->children[i], depth + 1);
    }
}

char buffer[200];

char *get_literal(TreeNode *node)
{
    buffer[0] = '\0';
    if (node->is_token)
    {
        switch (node->type)
        {
        case INT_T:
            sprintf(buffer, "INT: %d", node->value.int_val);
            break;
        case FLOAT_T:
            sprintf(buffer, "FLOAT: %f", node->value.float_val);
            break;
        case SEMI_T:
            sprintf(buffer, "SEMI");
            break;
        case COMMA_T:
            sprintf(buffer, "COMMA");
            break;
        case ASSIGNOP_T:
            sprintf(buffer, "ASSIGNOP");
            break;
        case RELOP_T:
            sprintf(buffer, "RELOP");
            break;
        case ID_T:
            sprintf(buffer, "ID: %s", node->value.str_val);
            break;
        case TYPE_T:
            sprintf(buffer, "TYPE: %s", node->value.str_val);
            break;
        case DOT_T:
            sprintf(buffer, "DOT");
            break;
        case PLUS_T:
            sprintf(buffer, "PLUS");
            break;
        case MINUS_T:
            sprintf(buffer, "MINUS");
            break;
        case STAR_T:
            sprintf(buffer, "STAR");
            break;
        case DIV_T:
            sprintf(buffer, "DIV");
            break;
        case AND_T:
            sprintf(buffer, "AND");
            break;
        case OR_T:
            sprintf(buffer, "OR");
            break;
        case NOT_T:
            sprintf(buffer, "NOT");
            break;
        case LP_T:
            sprintf(buffer, "LP");
            break;
        case RP_T:
            sprintf(buffer, "RP");
            break;
        case LB_T:
            sprintf(buffer, "LB");
            break;
        case RB_T:
            sprintf(buffer, "RB");
            break;
        case LC_T:
            sprintf(buffer, "LC");
            break;
        case RC_T:
            sprintf(buffer, "RC");
            break;
        case STRUCT_T:
            sprintf(buffer, "STRUCT");
            break;
        case RETURN_T:
            sprintf(buffer, "RETURN");
            break;
        case IF_T:
            sprintf(buffer, "IF");
            break;
        case ELSE_T:
            sprintf(buffer, "ELSE");
            break;
        case WHILE_T:
            sprintf(buffer, "WHILE");
            break;

        default:
            assert(0);
            break;
        }
    }
    else
    {
        switch (node->type)
        {
        case Program:
            sprintf(buffer, "Program (%d)", node->first_line);
            break;
        case ExtDefList:
            sprintf(buffer, "ExtDefList (%d)", node->first_line);
            break;
        case ExtDef:
            sprintf(buffer, "ExtDef (%d)", node->first_line);
            break;
        case ExtDecList:
            sprintf(buffer, "ExtDecList (%d)", node->first_line);
            break;
        case Specifier:
            sprintf(buffer, "Specifier (%d)", node->first_line);
            break;
        case StructSpecifier:
            sprintf(buffer, "StructSpecifier (%d)", node->first_line);
            break;
        case OptTag:
            sprintf(buffer, "OptTag (%d)", node->first_line);
            break;
        case Tag:
            sprintf(buffer, "Tag (%d)", node->first_line);
            break;
        case VarDec:
            sprintf(buffer, "VarDec (%d)", node->first_line);
            break;
        case FunDec:
            sprintf(buffer, "FunDec (%d)", node->first_line);
            break;
        case VarList:
            sprintf(buffer, "VarList (%d)", node->first_line);
            break;
        case ParamDec:
            sprintf(buffer, "ParamDec (%d)", node->first_line);
            break;
        case CompSt:
            sprintf(buffer, "CompSt (%d)", node->first_line);
            break;
        case StmtList:
            sprintf(buffer, "StmtList (%d)", node->first_line);
            break;
        case Stmt:
            sprintf(buffer, "Stmt (%d)", node->first_line);
            break;
        case DefList:
            sprintf(buffer, "DefList (%d)", node->first_line);
            break;
        case Def:
            sprintf(buffer, "Def (%d)", node->first_line);
            break;
        case DecList:
            sprintf(buffer, "DecList (%d)", node->first_line);
            break;
        case Dec:
            sprintf(buffer, "Dec (%d)", node->first_line);
            break;
        case Exp:
            sprintf(buffer, "Exp (%d)", node->first_line);
            break;
        case Args:
            sprintf(buffer, "Args (%d)", node->first_line);
            break;
        default:
            assert(0);
            break;
        }
    }
    return buffer;
}

bool check_nonterminal_type(TreeNode *node, int type)
{
    if (node->is_token == false && node->type == type)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool check_token_type(TreeNode *node, int type)
{
    if (node->is_token == true && node->type == type)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void handle(TreeNode *root)
{
    analyse_program(root);

    print_codes(translate_Program(root));
}

int get_size(Type *type)
{
    assert(type != NULL);
    switch (type->kind)
    {
    case BASIC:
        return 4;
    case ARRAY:
        return type->u.array.size * 4;
    case STRUCTURE:;
        //The SEMI above is to disable c error
        FieldList *fields = type->u.structure->next;
        int total = 0;
        while (fields != 0)
        {
            total += get_size(fields->type);
            fields = fields->next;
        }
        return total;
    default:
        assert(false);
    }
}

int get_struct_offset(OperandWrapper *addr, TreeNode *exp, TreeNode *id)
{
    assert(CHECK_NON_TYPE(exp, Exp));
    assert(CHECK_TOKEN_TYPE(id, ID_T));

    if (exp->num_of_children == 1 && CHECK_TOKEN_TYPE(exp->children[0], ID_T))
    {
        int ret = 0;
        SymbolNode *node = look_up_variable_list(exp->children[0]->value.str_val);
        addr->head = new_real_var_op(node->name);
        FieldList *fields = node->type->u.structure->next;
        while (fields != NULL && strcmp(fields->name, id->value.str_val) != 0)
        {
            ret += get_size(fields->type);
            fields = fields->next;
        }
        return ret;
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[0], LP_T))
    {
        return get_struct_offset(addr, exp->children[1], id);
    }
    else if (exp->num_of_children == 3 && CHECK_TOKEN_TYPE(exp->children[1], DOT_T))
    {
        int ret = get_struct_offset(addr, exp->children[0], exp->children[2]);
        Type *type = analyse_exp(exp);
        FieldList *fields = type->u.structure->next;
        while (fields != NULL && strcmp(fields->name, id->value.str_val) != 0)
        {
            ret += get_size(fields->type);
            fields = fields->next;
        }
        return ret;
    }
    else
    {
        assert(false);
    }
}