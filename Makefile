tableaux : main.c lex.yy.c grammar.tab.c Formula.c Formula.h Tree.c Tree.h SVG.c SVG.h Makefile Global.c Global.h Config.c Config.h
	gcc main.c lex.yy.c grammar.tab.c Global.c Formula.c Tree.c SVG.c Config.c -fstack-protector-all -o tableaux -g

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
