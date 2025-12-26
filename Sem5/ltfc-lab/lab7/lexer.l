%{
#include "parser.tab.h"
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
        unsigned char ch = (unsigned char)input[i];
        if (current->transitions[ch])
            current = current->transitions[ch];
        else
            return 0;
    }
    return current->is_accept;
}

State* id_fa = NULL;
State* num_fa = NULL;

extern int yylval;
%}

%%

[ \t\r\n]+                 { /* skip whitespace */ }

"load"                    { return LOAD; }
"save"                    { return SAVE; }
"replace"                 { return REPLACE; }
"print"                   { return PRINT; }
"delete"                  { return DELETE; }
"count"                   { return COUNT; }
"append"                  { return APPEND; }
"if"                      { return IF; }
"then"                    { return THEN; }
"endif"                   { return ENDIF; }
"into"                    { return INTO; }
"to"                      { return TO; }
"from"                    { return FROM; }
"where"                   { return WHERE; }
"equals"                  { return EQUALS; }
"contains"                { return CONTAINS; }
"startswith"              { return STARTSWITH; }
"endswith"                { return ENDSWITH; }

"=="                      { return EQ; }
"!="                      { return NEQ; }
"="                       { return ASSIGN; }
"+"                       { return PLUS; }
","                       { return COMMA; }
"("                       { return LPAREN; }
")"                       { return RPAREN; }

\"[a-zA-Z0-9._= ]*\"      {
                              int len = strlen(yytext);
                              char *s = malloc(len - 1);
                              strncpy(s, yytext + 1, len - 2);
                              s[len - 2] = '\0';
                              yylval = (int)s;
                              return STRING;
                          }

[a-zA-Z][a-zA-Z0-9]*      {
                              if (!id_fa) id_fa = build_identifier_fa();
                              if (!dfa_traverse(id_fa, yytext))
                                  fprintf(stderr, "LEXICAL ERROR: Invalid identifier <%s>\n", yytext);

                              yylval = (int)strdup(yytext);
                              return ID;
                          }

[0-9]+                    {
                              if (!num_fa) num_fa = build_number_fa();
                              if (!dfa_traverse(num_fa, yytext))
                                  fprintf(stderr, "LEXICAL ERROR: Invalid number <%s>\n", yytext);

                              yylval = (int)strdup(yytext);
                              return NUMBER;
                          }

.                         {
                              fprintf(stderr, "LEXICAL ERROR: Unknown character: %c\n", yytext[0]);
                          }

%%

int yywrap(void) {
    return 1;
}
