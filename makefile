all:
	bison -d eng.y
	flex eng.l
	gcc -o eng main.c eng.tab.c lex.yy.c -lfl
