#ifndef UTIL_H
#define UTIL_H

typedef enum
{
    false,
    true
} bool;

typedef enum
{
    Program,
    ExtDefList,
    ExtDef,
    ExtDecList,
    Specifier,
    StructSpecifier,
    OptTag,
    Tag,
    VarDec,
    FunDec,
    VarList,
    ParamDec,
    CompSt,
    StmtList,
    Stmt,
    DefList,
    Def,
    DecList,
    Dec,
    Exp,
    Args,

    Error
} nonterminal_symbol_t;

typedef enum
{
    INT_T,
    FLOAT_T,
    SEMI_T,
    COMMA_T,
    ASSIGNOP_T,
    RELOP_T,
    ID_T,
    TYPE_T,
    DOT_T,
    PLUS_T,
    MINUS_T,
    STAR_T,
    DIV_T,
    AND_T,
    OR_T,
    NOT_T,
    LP_T,
    RP_T,
    LB_T,
    RB_T,
    LC_T,
    RC_T,
    STRUCT_T,
    RETURN_T,
    IF_T,
    ELSE_T,
    WHILE_T
} token_symbol_t;

void setup();

#endif