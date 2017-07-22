tableaux : main.c lex.yy.c grammar.tab.c Formula.c Formula.h
	gcc main.c lex.yy.c grammar.tab.c Formula.c -lfl -fstack-protector-all -o tableaux -O3 

lex.yy.c : lex.l
	flex -o lex.yy.c lex.l

grammar.tab.c : grammar.y
	bison -d grammar.y

clean :
	@rm -f lex.yy.c grammar.tab.h grammar.tab.c
run:
	./tableaux prueba
fast:
	make && make run