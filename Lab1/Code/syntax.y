%{
    #include <stdio.h>
    int yylex();
    void yyerror(char *s){printf("%s\n","yyerror");}
    int fileno(FILE* stream); 
%}
%token INT FLOAT
%token SEMI COMMA ASSIGNOP RELOP
%token ID TYPE
%token DOT
%token PLUS MINUS STAR DIV
%token AND OR NOT
%token LP RP LB RB LC RC
%token STRUCT
%token RETURN IF ELSE WHILE
%%
Num : INT {printf("num is %d\n",$1);}
    ;
%%
#include "lex.yy.c"