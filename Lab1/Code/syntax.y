%{
    #define YYDEBUG 1
    #include <stdio.h>
    int yylex();
    void yyerror(char *s){printf("%s\n",s);}
    int fileno(FILE* stream); 
%}
%locations
%union {
    int int_val;
    float float_val;
    char* str_val;
}
%token <int_val> INT 
%token <float_val> FLOAT
%token SEMI COMMA ASSIGNOP 
%token <str_val> RELOP
%token <str_val> ID 
%token <str_val> TYPE
%token DOT
%token PLUS MINUS STAR DIV
%token AND OR NOT
%token LP RP LB RB LC RC
%token STRUCT
%token RETURN IF ELSE WHILE
%%
Program : ExtDefList {printf("program\n");}
    ;
ExtDefList : ExtDef ExtDefList
    | /* empty */
    ;
ExtDef : Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    ;
ExtDecList : VarDec
    | VarDec COMMA ExtDecList
    ;
Specifier : TYPE
    | StructSpecifier
    ;
StructSpecifier : STRUCT OptTag LC DefList RC
    | STRUCT Tag 
    ;
OptTag : ID
    | /* empty */
    ;
Tag : ID
    ;
VarDec : ID
    | VarDec LB INT RB
    ;
FunDec : ID LP VarList RP
    |ID LP RP
    ;
VarList : ParamDec COMMA VarList
    | ParamDec
    ;
ParamDec : Specifier VarDec
    ;
CompSt : LC DefList StmtList RC
    ;
StmtList : Stmt StmtList
    | /* empty */
    ;
Stmt : Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    ;
DefList : Def DefList
    | /* empty */
    ;
Def : Specifier DecList SEMI
    ;
DecList : Dec 
    | Dec COMMA DecList
    ;
Dec : VarDec
    | VarDec ASSIGNOP Exp
    ;
Exp : Exp ASSIGNOP Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp RELOP Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp STAR Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID
    | INT
    | FLOAT
    ;
Args : Exp COMMA Args
    | Exp
    ;
%%
#include "lex.yy.c"