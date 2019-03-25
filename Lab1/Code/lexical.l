%{
    #include "syntax.tab.h"
    #include <string.h>
    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;
%}
%option yylineno
positive-digit [1-9]
digit [0-9]
letter [a-zA-Z]
letterOrUs [a-zA-Z_]
delim [ \t\n\r]
%%
int|float {printf("type\n");return TYPE;}
struct {return STRUCT;}
return {return RETURN;}
if {return IF;}
else {return ELSE;}
while {return WHILE;}
\( {return LP;}
\) {return RP;}
\[ {return LB;}
\] {return RB;}
\{ {return LC;}
\} {return RC;}
; {return SEMI;}
, {return COMMA;}
= {printf("assignop\n");return ASSIGNOP;}
>|<|(>=)|(<=)|(==)|(!=) {yylval.str_val = strdup(yytext);return RELOP;}
\+ {return PLUS;}
- {return MINUS;}
\* {return STAR;}
\/ {return DIV;}
&& {return AND;}
\|\| {return OR;}
! {return NOT;}
(0)|({positive-digit}{digit}*) { yylval.int_val = atoi(yytext);return INT;}
{digit}+\.{digit}+ {yylval.float_val = atof(yytext);return FLOAT;}
\. {return DOT;}
({letterOrUs})({letterOrUs}|{digit})* { printf("%s\n",yytext);yylval.str_val = strdup(yytext);return ID;}
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
    yydebug = 1;
    yyparse();
    return 0;
}
