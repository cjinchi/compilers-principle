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
%}
%option yylineno
positive-digit [1-9]
digit [0-9]
letter [a-zA-Z]
letterOrUs [a-zA-Z_]
delim [ \t\n\r]
%%
int|float {TOKEN_ASSIGN_YYLVAL(TYPE);return TYPE;}
struct {TOKEN_ASSIGN_YYLVAL(STRUCT);return STRUCT;}
return {TOKEN_ASSIGN_YYLVAL(RETURN);return RETURN;}
if {TOKEN_ASSIGN_YYLVAL(IF);return IF;}
else {TOKEN_ASSIGN_YYLVAL(ELSE);return ELSE;}
while {TOKEN_ASSIGN_YYLVAL(WHILE);return WHILE;}
\( {TOKEN_ASSIGN_YYLVAL(LP);return LP;}
\) {TOKEN_ASSIGN_YYLVAL(RP);return RP;}
\[ {TOKEN_ASSIGN_YYLVAL(LB);return LB;}
\] {TOKEN_ASSIGN_YYLVAL(RB);return RB;}
\{ {TOKEN_ASSIGN_YYLVAL(LC);return LC;}
\} {TOKEN_ASSIGN_YYLVAL(RC);return RC;}
; {TOKEN_ASSIGN_YYLVAL(SEMI);return SEMI;}
, {TOKEN_ASSIGN_YYLVAL(COMMA);return COMMA;}
= {TOKEN_ASSIGN_YYLVAL(ASSIGNOP);return ASSIGNOP;}
>|<|(>=)|(<=)|(==)|(!=) {TOKEN_ASSIGN_YYLVAL(RELOP);yylval->value.str_val = strdup(yytext);return RELOP;}
\+ {TOKEN_ASSIGN_YYLVAL(PLUS);return PLUS;}
- {TOKEN_ASSIGN_YYLVAL(MINUS);return MINUS;}
\* {TOKEN_ASSIGN_YYLVAL(STAR);return STAR;}
\/ {TOKEN_ASSIGN_YYLVAL(DIV);return DIV;}
&& {TOKEN_ASSIGN_YYLVAL(AND);return AND;}
\|\| {TOKEN_ASSIGN_YYLVAL(OR);return OR;}
! {TOKEN_ASSIGN_YYLVAL(NOT);return NOT;}
(0)|({positive-digit}{digit}*) {TOKEN_ASSIGN_YYLVAL(INT);yylval->value.int_val = atoi(yytext);return INT;}
({digit}+\.{digit}+)|((({digit}*\.{digit}+)|({digit}+\.{digit}*))(e|E)((\+|-)?)({digit}+)) {TOKEN_ASSIGN_YYLVAL(FLOAT);yylval->value.float_val = atof(yytext);return FLOAT;}
\. {TOKEN_ASSIGN_YYLVAL(DOT);return DOT;}
({letterOrUs})({letterOrUs}|{digit})* {TOKEN_ASSIGN_YYLVAL(ID);yylval->value.str_val = strdup(yytext); return ID;}
\n {yycolumn = 1;}
{delim} {  }
. { printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno, yytext); }
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
