/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LOAD = 258,
     SAVE = 259,
     REPLACE = 260,
     PRINT = 261,
     DELETE = 262,
     COUNT = 263,
     APPEND = 264,
     IF = 265,
     THEN = 266,
     ENDIF = 267,
     INTO = 268,
     TO = 269,
     FROM = 270,
     WHERE = 271,
     EQUALS = 272,
     CONTAINS = 273,
     STARTSWITH = 274,
     ENDSWITH = 275,
     ASSIGN = 276,
     EQ = 277,
     NEQ = 278,
     PLUS = 279,
     COMMA = 280,
     LPAREN = 281,
     RPAREN = 282,
     STRING = 283,
     ID = 284,
     NUMBER = 285
   };
#endif
/* Tokens.  */
#define LOAD 258
#define SAVE 259
#define REPLACE 260
#define PRINT 261
#define DELETE 262
#define COUNT 263
#define APPEND 264
#define IF 265
#define THEN 266
#define ENDIF 267
#define INTO 268
#define TO 269
#define FROM 270
#define WHERE 271
#define EQUALS 272
#define CONTAINS 273
#define STARTSWITH 274
#define ENDSWITH 275
#define ASSIGN 276
#define EQ 277
#define NEQ 278
#define PLUS 279
#define COMMA 280
#define LPAREN 281
#define RPAREN 282
#define STRING 283
#define ID 284
#define NUMBER 285




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

