# Simple Makefile for flex + bison
CC = gcc
FLEX = flex
BISON = bison

all: parser

parser: parser.tab.c lex.yy.c
	$(CC) -o parser parser.tab.c lex.yy.c

parser.tab.c parser.tab.h: parser.y
	$(BISON) -d -v parser.y

lex.yy.c: lexer.l parser.tab.h
	$(FLEX) lexer.l

clean:
	rm -f parser parser.tab.c parser.tab.h parser.output lex.yy.c
