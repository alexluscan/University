%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct State {
    int is_accept;
    struct State* transitions[256];
} State;

State* createState(int is_accept) {
    State* s = (State*)malloc(sizeof(State));
    s->is_accept = is_accept;
    for (int i = 0; i < 256; i++) s->transitions[i] = NULL;
    return s;
}

// <identifier>  ::= <letter> { <letter> | <digit> }
// <letter>      ::= "a" | ... | "z" | "A" | ... | "Z"
// <digit>       ::= "0" | ... | "9"

State* build_identifier_fa() {
    State* s0 = createState(0);
    State* s1 = createState(1);

    for (char c = 'a'; c <= 'z'; c++) {
        s0->transitions[(int)c] = s1;
        s1->transitions[(int)c] = s1;
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        s0->transitions[(int)c] = s1;
        s1->transitions[(int)c] = s1;
    }
    for (char c = '0'; c <= '9'; c++) {
        s1->transitions[(int)c] = s1;
    }
    return s0;
}

// <number>      ::= <digit> { <digit> }
// <digit>       ::= "0" | ... | "9"

State* build_number_fa() {
    State* s0 = createState(0);
    State* s1 = createState(1);

    for (char c = '0'; c <= '9'; c++) {
        s0->transitions[(int)c] = s1;
        s1->transitions[(int)c] = s1;
    }
    return s0;
}

int dfa_traverse(State* start, const char* input) {
    State* current = start;
    for (int i = 0; input[i]; i++) {
        unsigned char ch = input[i];
        if (current->transitions[ch])
            current = current->transitions[ch];
        else
            return 0;
    }
    return current->is_accept;
}

State* id_fa;
State* num_fa;

%}

%%
"load"             { printf("TOKEN: LOAD\n"); }
"save"             { printf("TOKEN: SAVE\n"); }
"replace"          { printf("TOKEN: REPLACE\n"); }                     
"print"            { printf("TOKEN: PRINT\n"); }
"delete"           { printf("TOKEN: DELETE\n"); }
"count"            { printf("TOKEN: COUNT\n"); }
"append"           { printf("TOKEN: APPEND\n"); }
"if"               { printf("TOKEN: IF\n"); }
"then"             { printf("TOKEN: THEN\n"); }
"endif"            { printf("TOKEN: ENDIF\n"); }
"into"             { printf("TOKEN: INTO\n"); }
"to"               { printf("TOKEN: TO\n"); }
"from"             { printf("TOKEN: FROM\n"); }
"where"            { printf("TOKEN: WHERE\n"); }
"equals"           { printf("TOKEN: EQUALS\n"); }
"contains"         { printf("TOKEN: CONTAINS\n"); }
"startswith"       { printf("TOKEN: STARTSWITH\n"); }
"endswith"         { printf("TOKEN: ENDSWITH\n"); }
"="                { printf("TOKEN: ASSIGN\n"); }
"=="               { printf("TOKEN: EQ\n"); }
"!="               { printf("TOKEN: NEQ\n"); }
"+"                { printf("TOKEN: PLUS\n"); }
","                { printf("TOKEN: COMMA\n"); }
"("                { printf("TOKEN: LPAREN\n"); }
")"                { printf("TOKEN: RPAREN\n"); }
\"[a-zA-Z0-9._= ]*\" { printf("TOKEN: STRING <%s>\n", yytext); }

[a-zA-Z][a-zA-Z0-9]* {
    if (dfa_traverse(id_fa, yytext))
        printf("TOKEN: ID <%s>\n", yytext);
    else
        fprintf(stderr, "LEXICAL ERROR: Invalid identifier <%s>\n", yytext);
}

[0-9]+ {
    if (dfa_traverse(num_fa, yytext))
        printf("TOKEN: NUMBER <%s>\n", yytext);
    else
        fprintf(stderr, "LEXICAL ERROR: Invalid number <%s>\n", yytext);
}

%%

int yywrap(void) {
    return 1;
}

int main(int argc, char** argv) {
    id_fa = build_identifier_fa();
    num_fa = build_number_fa();

    yylex();
    return 0;
}