%{
    /* 此处省略#include部分 */
    #include <stdio.h>
    int chars = 0;
    int words = 0;
    int lines = 0;
%}
%option yylineno
letter [a-zA-Z]
%%
{letter}+ { words++; chars+= yyleng;printf("%d\n",yylineno); }
\n { chars++; lines++; }
. { chars++; }
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
    printf("%8d%8d%8d\n", lines, words, chars);
    return 0;
 }