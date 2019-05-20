%{
    #define YYDEBUG 1
    #define YYSTYPE TreeNode* 
    #include "TreeNode.h"
    #include <stdio.h>
    #include "lex.yy.c"
    #include "util.h"
    #include "analyse_tree.h"
    int yylineno;
    int yylex();
    bool has_error = false;
    void yyerror(char *s){has_error = true;printf("Error type B at Line %d: %s.\n",yylineno,s);}
    int fileno(FILE* stream); 
    TreeNode* head = NULL;

%}
%locations

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
Program : ExtDefList {$$ = create_nonterminal_node(Program,@$.first_line,1,$1);  head = $$; if(has_error==false)analyse_program(head);}
    ;
ExtDefList : ExtDef ExtDefList {$$ = create_nonterminal_node(ExtDefList,@$.first_line,2,$1,$2);}
    | /* empty */ {$$ = create_nonterminal_node(ExtDefList,@$.first_line,0);}
    ;
ExtDef : Specifier ExtDecList SEMI {$$ = create_nonterminal_node(ExtDef,@$.first_line,3,$1,$2,$3);}
    | Specifier SEMI {$$ = create_nonterminal_node(ExtDef,@$.first_line,2,$1,$2);}
    | Specifier FunDec CompSt {$$ = create_nonterminal_node(ExtDef,@$.first_line,3,$1,$2,$3);}
    | Specifier FunDec error {$$ = create_nonterminal_node(ExtDef,@$.first_line,3,$1,$2,create_error_node(@$.first_line)); yyerrok;}
    | error SEMI { $$ = create_nonterminal_node(ExtDef,@$.first_line,2,create_error_node(@$.first_line),$2); yyerrok; }
    | Specifier error { $$ = create_nonterminal_node(ExtDef,@$.first_line,2,$1,create_error_node(@$.first_line)); yyerrok; }
    | Specifier ExtDecList error { $$ = create_nonterminal_node(ExtDef,@$.first_line,3,$1,$2,create_error_node(@$.first_line)); yyerrok; }
    
    ;
ExtDecList : VarDec {$$ = create_nonterminal_node(ExtDecList,@$.first_line,1,$1);}
    | VarDec COMMA ExtDecList {$$ = create_nonterminal_node(ExtDecList,@$.first_line,3,$1,$2,$3);}
    | VarDec COMMA error {$$ = create_nonterminal_node(ExtDecList,@$.first_line,3,$1,$2,create_error_node(@$.first_line));yyerrok;}
    ;
Specifier : TYPE {$$ = create_nonterminal_node(Specifier,@$.first_line,1,$1);}
    | StructSpecifier {$$ = create_nonterminal_node(Specifier,@$.first_line,1,$1);}
    ;
StructSpecifier : STRUCT OptTag LC DefList RC {$$ = create_nonterminal_node(StructSpecifier,@$.first_line,5,$1,$2,$3,$4,$5);}
    | STRUCT Tag {$$ = create_nonterminal_node(StructSpecifier,@$.first_line,2,$1,$2);}
    ;
OptTag : ID {$$ = create_nonterminal_node(OptTag,@$.first_line,1,$1);}
    | /* empty */ {$$ = create_nonterminal_node(OptTag,@$.first_line,0);}
    ;
Tag : ID {$$ = create_nonterminal_node(Tag,@$.first_line,1,$1);}
    ;
VarDec : ID {$$ = create_nonterminal_node(VarDec,@$.first_line,1,$1);}
    | VarDec LB INT RB {$$ = create_nonterminal_node(VarDec,@$.first_line,4,$1,$2,$3,$4);}
    ;
FunDec : ID LP VarList RP {$$ = create_nonterminal_node(FunDec,@$.first_line,4,$1,$2,$3,$4);}
    |ID LP RP {$$ = create_nonterminal_node(FunDec,@$.first_line,3,$1,$2,$3);}
    ;
VarList : ParamDec COMMA VarList {$$ = create_nonterminal_node(VarList,@$.first_line,3,$1,$2,$3);}
    | ParamDec {$$ = create_nonterminal_node(VarList,@$.first_line,1,$1);}
    | ParamDec error VarList {$$ = create_nonterminal_node(VarList,@$.first_line,3,$1,create_error_node(@$.first_line),$3);yyerrok;}
    ;
ParamDec : Specifier VarDec {$$ = create_nonterminal_node(ParamDec,@$.first_line,2,$1,$2);}
    ;
CompSt : LC DefList StmtList RC {$$ = create_nonterminal_node(CompSt,@$.first_line,4,$1,$2,$3,$4);}
    | error RC { {$$ = create_nonterminal_node(CompSt,@$.first_line,2,create_error_node(@$.first_line),$2);yyerrok;}}
    ;
StmtList : Stmt StmtList {$$ = create_nonterminal_node(StmtList,@$.first_line,2,$1,$2);}
    | /* empty */ {$$ = create_nonterminal_node(StmtList,@$.first_line,0);}
    ;
Stmt : Exp SEMI {$$ = create_nonterminal_node(Stmt,@$.first_line,2,$1,$2);}
    | CompSt {$$ = create_nonterminal_node(Stmt,@$.first_line,1,$1);}
    | RETURN Exp SEMI {$$ = create_nonterminal_node(Stmt,@$.first_line,3,$1,$2,$3);}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$ = create_nonterminal_node(Stmt,@$.first_line,5,$1,$2,$3,$4,$5);}
    | IF LP Exp RP Stmt ELSE Stmt {$$ = create_nonterminal_node(Stmt,@$.first_line,7,$1,$2,$3,$4,$5,$6,$7);}
    | WHILE LP Exp RP Stmt {$$ = create_nonterminal_node(Stmt,@$.first_line,5,$1,$2,$3,$4,$5);}
    | error SEMI {$$ = create_nonterminal_node(Stmt,@$.first_line,2,create_error_node(@$.first_line),$2);yyerrok;}
    ;
DefList : Def DefList {$$ = create_nonterminal_node(DefList,@$.first_line,2,$1,$2);}
    | /* empty */ {$$ = create_nonterminal_node(DefList,@$.first_line,0);}
    ;
Def : Specifier DecList SEMI {$$ = create_nonterminal_node(Def,@$.first_line,3,$1,$2,$3);}
    | Specifier error SEMI {$$ = create_nonterminal_node(Def,@$.first_line,3,$1,create_error_node(@$.first_line),$3);yyerrok;}
    ;
DecList : Dec  {$$ = create_nonterminal_node(DecList,@$.first_line,1,$1);}
    | Dec COMMA DecList {$$ = create_nonterminal_node(DecList,@$.first_line,3,$1,$2,$3);}
    | Dec error DecList {$$ = create_nonterminal_node(DecList,@$.first_line,3,$1,create_error_node(@$.first_line),$3);yyerrok;}
    ;
Dec : VarDec {$$ = create_nonterminal_node(Dec,@$.first_line,1,$1);}
    | VarDec ASSIGNOP Exp {$$ = create_nonterminal_node(Dec,@$.first_line,3,$1,$2,$3);}
    | VarDec ASSIGNOP error {$$ = create_nonterminal_node(Dec,@$.first_line,3,$1,$2,create_error_node(@$.first_line));yyerrok;}
    ;
Exp : Exp ASSIGNOP Exp {$$ = create_nonterminal_node(Exp,@$.first_line,3,$1,$2,$3);}
    | Exp AND Exp {$$ = create_nonterminal_node(Exp,@$.first_line,3,$1,$2,$3);}
    | Exp OR Exp {$$ = create_nonterminal_node(Exp,@$.first_line,3,$1,$2,$3);}
    | Exp RELOP Exp {$$ = create_nonterminal_node(Exp,@$.first_line,3,$1,$2,$3);}
    | Exp PLUS Exp {$$ = create_nonterminal_node(Exp,@$.first_line,3,$1,$2,$3);}
    | Exp MINUS Exp {$$ = create_nonterminal_node(Exp,@$.first_line,3,$1,$2,$3);}
    | Exp STAR Exp {$$ = create_nonterminal_node(Exp,@$.first_line,3,$1,$2,$3);}
    | Exp DIV Exp {$$ = create_nonterminal_node(Exp,@$.first_line,3,$1,$2,$3);}
    | LP Exp RP {$$ = create_nonterminal_node(Exp,@$.first_line,3,$1,$2,$3);}
    | MINUS Exp %prec NEG {$$ = create_nonterminal_node(Exp,@$.first_line,2,$1,$2);}
    | NOT Exp {$$ = create_nonterminal_node(Exp,@$.first_line,2,$1,$2);}
    | ID LP Args RP {$$ = create_nonterminal_node(Exp,@$.first_line,4,$1,$2,$3,$4);}
    | ID LP RP {$$ = create_nonterminal_node(Exp,@$.first_line,3,$1,$2,$3);}
    | Exp LB Exp RB {$$ = create_nonterminal_node(Exp,@$.first_line,4,$1,$2,$3,$4);}
    | Exp DOT ID {$$ = create_nonterminal_node(Exp,@$.first_line,3,$1,$2,$3);}
    | ID {$$ = create_nonterminal_node(Exp,@$.first_line,1,$1);}
    | INT {$$ = create_nonterminal_node(Exp,@$.first_line,1,$1);}
    | FLOAT {$$ = create_nonterminal_node(Exp,@$.first_line,1,$1);}
    | ID LP error RP {$$ = create_nonterminal_node(Exp,@$.first_line,4,$1,$2,create_error_node(@$.first_line),$4);yyerrok;}
    | LB error RB {$$ = create_nonterminal_node(Exp,@$.first_line,3,$1,create_error_node(@$.first_line),$3);yyerrok;} 
    ;
Args : Exp COMMA Args {$$ = create_nonterminal_node(Args,@$.first_line,3,$1,$2,$3);} 
    | Exp {$$ = create_nonterminal_node(Args,@$.first_line,1,$1);}
    ;
%%
