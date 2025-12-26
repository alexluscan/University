%{
#include "parser.tab.h"
%}

%option noyywrap

%%
LOAD            return LOAD;
SAVE            return SAVE;
PRINT           return PRINT;
DELETE          return DELETE;
APPEND          return APPEND;
IF              return IF;
THEN            return THEN;
ENDIF           return ENDIF;
INTO            return INTO;
TO              return TO;
EQUALS          return EQUALS;
CONTAINS        return CONTAINS;
STARTSWITH      return STARTSWITH;
ENDSWITH        return ENDSWITH;
ID              return ID;
STRING          return STRING;

[ \t\r\n]+      ;   /* ignore whitespace */
.               return yytext[0];
%%
