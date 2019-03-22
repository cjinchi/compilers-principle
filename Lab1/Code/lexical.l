%{
    typedef enum
    {
        INT, FLOAT, ID,
        SEMI, COMMA, ASSIGNOP, RELOP,
        PLUS, MINUS, STAR, DIV,
        AND, OR, DOT, NOT,
        TYPE, LP, RP, LB, RB, LC, RC,
        STRUCT, RETURN, IF, ELSE, WHILE
    } type_t;
    typedef struct item_struct
    {
        type_t type;
        unsigned char buffer[100];
        struct item_struct *next;
    } item_t;
    typedef enum
    {
        FALSE,TRUE
    }boolean;

    item_t* head = NULL;
    item_t* tail = NULL;
    boolean has_error = FALSE;
    void error_type_A(int line_num,char* yytext);
    void push_item(type_t type,char* yytext);
%}
%option yylineno
positive-digit [1-9]
digit [0-9]
letter [a-zA-Z]
letterOrUs [a-zA-Z_]
delim [ \t\n\r]
%%
int|float {push_item(TYPE,yytext);}
struct {push_item(STRUCT,yytext);}
return {push_item(RETURN,yytext);}
if {push_item(IF,yytext);}
else {push_item(ELSE,yytext);}
while {push_item(WHILE,yytext);}
\( {push_item(LP,yytext);}
\) {push_item(RP,yytext);}
\[ {push_item(LB,yytext);}
\] {push_item(RB,yytext);}
\{ {push_item(LC,yytext);}
\} {push_item(RC,yytext);}
; {push_item(SEMI,yytext);}
, {push_item(COMMA,yytext);}
= {push_item(ASSIGNOP,yytext);}
>|<|(>=)|(<=)|(==)|(!=) {push_item(RELOP,yytext);}
\+ {push_item(PLUS,yytext);}
- {push_item(MINUS,yytext);}
\* {push_item(STAR,yytext);}
\/ {push_item(DIV,yytext);}
&& {push_item(AND,yytext);}
\|\| {push_item(OR,yytext);}
! {push_item(NOT,yytext);}
(0)|({positive-digit}{digit}*) { push_item(INT,yytext);}
{digit}+\.{digit}+ {push_item(FLOAT,yytext);}
\. {push_item(DOT,yytext);}
({letterOrUs})({letterOrUs}|{digit})* { push_item(ID,yytext);}
{delim} {  }
. { error_type_A(yylineno,yytext); }
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
    if(has_error == FALSE)
    {
        printf("【print tree here】\n");
    }
    return 0;
}

void error_type_A(int line_num,char* yytext)
{
    has_error = TRUE;
    printf("Error type A at Line %d: Mysterious character \"%s\".\n",line_num,yytext);
}

void push_item(type_t type,char* yytext)
{
    item_t* item = (item_t*)malloc(sizeof(item_t));
    item -> type = type;
    strncpy(item->buffer,yytext,100);

    if(head == NULL)
    {
        head = item;
        tail = item;
    }
    else
    {
        tail->next = item;
        tail = item;
    }
}
