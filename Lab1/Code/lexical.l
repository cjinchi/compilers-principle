%{
    //声明变量
%}
%option yylineno
positive-digit [1-9]
digit [0-9]
letter [a-zA-Z]
letterOrUs [a-zA-Z_]
%%
int|float {printf("TYPE\n");}
struct {printf("STRUCT\n");}
return {printf("RETURN\n");}
if {printf("IF\n");}
else {printf("ELSE\n");}
while {printf("WHILE\n");}
\( {printf("LP\n");}
\) {printf("RP\n");}
\[ {printf("LB\n");}
\] {printf("RB\n");}
\{ {printf("LC\n");}
\} {printf("RC\n");}
; {printf("SEMI\n");}
, {printf("COMMA\n");}
= {printf("ASSIGNOP\n");}
>|<|(>=)|(<=)|(==)|(!=) {printf("RELOP\n");}
\+ {printf("PLUS\n");}
- {printf("MINUS\n");}
\* {printf("STAR\n");}
\/ {printf("DIV\n");}
&& {printf("AND\n");}
\|\| {printf("OR\n");}
! {printf("NOT\n");}
(0)|({positive-digit}{digit}*) { printf("INT: %d\n",atoi(yytext)); }
{digit}+\.{digit}+ {printf("FLOAT: %f\n",atof(yytext));}
\. {printf("DOT\n");}
({letterOrUs})({letterOrUs}|{digit})* { printf("ID: %s\n",yytext);}
\n {  }
. {  }
%%
int main(int argc, char** argv)
{
    if (argc > 1)
    {
        if (!(yyin = fopen(argv[1], "r")))
        {
            perror(argv[1]);
            return 1;
        }
    }
    yylex();
    return 0;
 }
