%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%%

BEGIN      { printf("TOKEN: BEGIN\n"); }
END        { printf("TOKEN: END\n"); }
WHILE      { printf("TOKEN: WHILE\n"); }
DO         { printf("TOKEN: DO\n"); }
ENDWHILE   { printf("TOKEN: ENDWHILE\n"); }
PRINT      { printf("TOKEN: PRINT\n"); }
":="       { printf("TOKEN: ASSIGN\n"); }
"+"        { printf("TOKEN: PLUS\n"); }
"-"        { printf("TOKEN: MINUS\n"); }
"*"        { printf("TOKEN: MULT\n"); }
"/"        { printf("TOKEN: DIV\n"); }
"("        { printf("TOKEN: LPAREN\n"); }
")"        { printf("TOKEN: RPAREN\n"); }
";"        { printf("TOKEN: SEMICOLON\n"); }
"<="       { printf("TOKEN: LEQ\n"); }
">="       { printf("TOKEN: GEQ\n"); }
"<"        { printf("TOKEN: LT\n"); }
">"        { printf("TOKEN: GT\n"); }
"="        { printf("TOKEN: EQ\n"); }
[a-zA-Z][a-zA-Z0-9]*  { printf("TOKEN: ID <%s>\n", yytext); }
[0-9]+                { printf("TOKEN: NUMBER <%s>\n", yytext); }
[ \t\r\n]+            { /* ignore */ }
.                     { printf("LEXICAL ERROR: <%s>\n", yytext); }

%%

int yywrap(void) { return 1; }

int main(int argc, char** argv) {
    yylex();
    return 0;
}