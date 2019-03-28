%{
    // #define YYDEBUG 1
    #include "TreeNode.h"
    #include <stdio.h>
    #include "lex.yy.c"
    int yylineno;
    int yylex();
    void yyerror(char *s){printf("%s at line %d\n",s,yylineno);}
    int fileno(FILE* stream); 
    
%}
// %define api.value.type {TreeNode*}
%locations

// %union {
//     int int_val;
//     float float_val;
//     char* str_val;
// }


%token INT 
%token FLOAT
%token SEMI COMMA ASSIGNOP 
%token RELOP
%token ID 
%token TYPE
%token DOT
%token PLUS MINUS
%token STAR DIV
%token AND OR NOT
%token LP RP LB RB LC RC
%token STRUCT
%token RETURN IF ELSE WHILE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT NEG
%left LP RP LB RB DOT


%%
Program : ExtDefList { printf("!!![program]!!!\n"); }
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
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    | error SEMI 
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
    | MINUS Exp %prec NEG
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
