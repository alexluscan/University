%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
int yyerror(const char *s);
%}

/* Terminals */
%token LOAD SAVE PRINT DELETE APPEND IF THEN ENDIF
%token INTO TO
%token EQUALS CONTAINS STARTSWITH ENDSWITH
%token ID STRING

%%
program:
      stmt_list
    ;

stmt_list:
      stmt stmt_list
    | /* empty */
    ;

stmt:
      LOAD STRING INTO ID
    | SAVE ID TO STRING
    | PRINT ID
    | DELETE ID
    | APPEND STRING TO ID
    | IF cond THEN stmt_list ENDIF
    ;

cond:
      ID comp STRING
    ;

comp:
      EQUALS
    | CONTAINS
    | STARTSWITH
    | ENDSWITH
    ;
%%
int yyerror(const char *s) {
    printf("Syntax error\n");
    return 0;
}

int main(void) {
    return yyparse();
}
