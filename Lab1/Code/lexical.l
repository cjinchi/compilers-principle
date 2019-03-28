%{
    #include "syntax.tab.h"
    #include "TreeNode.h"
    #include <string.h>
    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;

    #define TOKEN_ASSIGN_YYLVAL(token_type) yylval=create_token_node(token_type,yylineno);
    extern bool has_error;
%}
%option yylineno
positive-digit [1-9]
digit [0-9]
letter [a-zA-Z]
letterOrUs [a-zA-Z_]
delim [ \t\n\r]
%%
int|float {TOKEN_ASSIGN_YYLVAL(TYPE_T);yylval->value.str_val = strdup(yytext);return TYPE;}
struct {TOKEN_ASSIGN_YYLVAL(STRUCT_T);return STRUCT;}
return {TOKEN_ASSIGN_YYLVAL(RETURN_T);return RETURN;}
if {TOKEN_ASSIGN_YYLVAL(IF_T);return IF;}
else {TOKEN_ASSIGN_YYLVAL(ELSE_T);return ELSE;}
while {TOKEN_ASSIGN_YYLVAL(WHILE_T);return WHILE;}
\( {TOKEN_ASSIGN_YYLVAL(LP_T);return LP;}
\) {TOKEN_ASSIGN_YYLVAL(RP_T);return RP;}
\[ {TOKEN_ASSIGN_YYLVAL(LB_T);return LB;}
\] {TOKEN_ASSIGN_YYLVAL(RB_T);return RB;}
\{ {TOKEN_ASSIGN_YYLVAL(LC_T);return LC;}
\} {TOKEN_ASSIGN_YYLVAL(RC_T);return RC;}
; {TOKEN_ASSIGN_YYLVAL(SEMI_T);return SEMI;}
, {TOKEN_ASSIGN_YYLVAL(COMMA_T);return COMMA;}
= {TOKEN_ASSIGN_YYLVAL(ASSIGNOP_T);return ASSIGNOP;}
>|<|(>=)|(<=)|(==)|(!=) {TOKEN_ASSIGN_YYLVAL(RELOP_T);yylval->value.str_val = strdup(yytext);return RELOP;}
\+ {TOKEN_ASSIGN_YYLVAL(PLUS_T);return PLUS;}
- {TOKEN_ASSIGN_YYLVAL(MINUS_T);return MINUS;}
\* {TOKEN_ASSIGN_YYLVAL(STAR_T);return STAR;}
\/ {TOKEN_ASSIGN_YYLVAL(DIV_T);return DIV;}
&& {TOKEN_ASSIGN_YYLVAL(AND_T);return AND;}
\|\| {TOKEN_ASSIGN_YYLVAL(OR_T);return OR;}
! {TOKEN_ASSIGN_YYLVAL(NOT_T);return NOT;}
(0)|({positive-digit}{digit}*) {TOKEN_ASSIGN_YYLVAL(INT_T);yylval->value.int_val = atoi(yytext);return INT;}
({digit}+\.{digit}+)|((({digit}*\.{digit}+)|({digit}+\.{digit}*))(e|E)((\+|-)?)({digit}+)) {TOKEN_ASSIGN_YYLVAL(FLOAT_T);yylval->value.float_val = atof(yytext);return FLOAT;}
\. {TOKEN_ASSIGN_YYLVAL(DOT_T);return DOT;}
({letterOrUs})({letterOrUs}|{digit})* {TOKEN_ASSIGN_YYLVAL(ID_T);yylval->value.str_val = strdup(yytext); return ID;}
\n {yycolumn = 1;}
{delim} {  }
. { printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno, yytext); has_error= true;}
%%
int main(int argc, char **argv)
{
    if (argc <= 1)
        return 1;
    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    // yydebug = 1;
    yyparse();
    return 0;
}
