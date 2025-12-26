%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex(void);
extern int yyparse(void);
extern int yylineno;
extern int yylex_destroy(void);

#define GET_STR(v) (char*)(v)

void yyerror(const char *s);

%}

%token LOAD SAVE REPLACE PRINT DELETE COUNT APPEND IF THEN ENDIF INTO TO FROM WHERE
%token EQUALS CONTAINS STARTSWITH ENDSWITH
%token ASSIGN EQ NEQ PLUS COMMA LPAREN RPAREN
%token STRING ID NUMBER

%start program

%%

program
    : stmt_list
        { printf("production: program ::= stmt_list\n"); }
    ;

stmt_list
    : stmt stmt_list
        { printf("production: stmt_list ::= stmt stmt_list\n"); }
    | /* empty */
        { printf("production: stmt_list ::= <empty>\n"); }
    ;

stmt
    : load_stmt     { printf("production: stmt ::= load_stmt\n"); }
    | save_stmt     { printf("production: stmt ::= save_stmt\n"); }
    | replace_stmt  { printf("production: stmt ::= replace_stmt\n"); }
    | print_stmt    { printf("production: stmt ::= print_stmt\n"); }
    | delete_stmt   { printf("production: stmt ::= delete_stmt\n"); }
    | count_stmt    { printf("production: stmt ::= count_stmt\n"); }
    | append_stmt   { printf("production: stmt ::= append_stmt\n"); }
    | if_stmt       { printf("production: stmt ::= if_stmt\n"); }
    | expr_stmt     { printf("production: stmt ::= expr_stmt\n"); }
    ;

load_stmt
    : LOAD STRING opt_into_id
        { printf("production: load_stmt ::= LOAD STRING opt_into_id\n"); }
    ;

save_stmt
    : SAVE STRING opt_into_id
        { printf("production: save_stmt ::= SAVE STRING opt_into_id\n"); }
    ;

opt_into_id
    : INTO ID
        { printf("production: opt_into_id ::= INTO ID\n"); }
    | /* empty */
        { printf("production: opt_into_id ::= <empty>\n"); }
    ;

replace_stmt
    : REPLACE STRING TO STRING
        { printf("production: replace_stmt ::= REPLACE STRING TO STRING\n"); }
    ;

print_stmt
    : PRINT STRING
        { printf("production: print_stmt ::= PRINT STRING\n"); }
    | PRINT ID
        { printf("production: print_stmt ::= PRINT ID\n"); }
    ;

delete_stmt
    : DELETE FROM ID opt_where
        { printf("production: delete_stmt ::= DELETE FROM ID opt_where\n"); }
    ;

count_stmt
    : COUNT FROM ID opt_where
        { printf("production: count_stmt ::= COUNT FROM ID opt_where\n"); }
    ;

append_stmt
    : APPEND STRING INTO ID
        { printf("production: append_stmt ::= APPEND STRING INTO ID\n"); }
    ;

opt_where
    : WHERE cond
        { printf("production: opt_where ::= WHERE cond\n"); }
    | /* empty */
        { printf("production: opt_where ::= <empty>\n"); }
    ;

if_stmt
    : IF cond THEN stmt_list ENDIF
        { printf("production: if_stmt ::= IF cond THEN stmt_list ENDIF\n"); }
    ;

expr_stmt
    : ID ASSIGN expr
        { printf("production: expr_stmt ::= ID ASSIGN expr\n"); }
    ;

expr
    : term
        { printf("production: expr ::= term\n"); }
    | expr PLUS term
        { printf("production: expr ::= expr PLUS term\n"); }
    ;

term
    : ID
        { printf("production: term ::= ID\n"); }
    | NUMBER
        { printf("production: term ::= NUMBER\n"); }
    | STRING
        { printf("production: term ::= STRING\n"); }
    ;

cond
    : ID WHEREOP STRING
        { printf("production: cond ::= ID WHEREOP STRING\n"); }
    | ID EQ NUMBER
        { printf("production: cond ::= ID EQ NUMBER\n"); }
    | ID NEQ NUMBER
        { printf("production: cond ::= ID NEQ NUMBER\n"); }
    ;

WHEREOP
    : EQUALS
        { printf("production: WHEREOP ::= EQUALS\n"); }
    | CONTAINS
        { printf("production: WHEREOP ::= CONTAINS\n"); }
    | STARTSWITH
        { printf("production: WHEREOP ::= STARTSWITH\n"); }
    | ENDSWITH
        { printf("production: WHEREOP ::= ENDSWITH\n"); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
}

int main(int argc, char **argv) {
    printf("Starting parse...\n");
    yyparse();
    printf("Parsing finished.\n");
    return 0;
}
