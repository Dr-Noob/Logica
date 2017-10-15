TARGET=tableaux
CFLAGS=-g -fstack-protector-all -o $(TARGET) -I $(DIR)
CC=gcc

DIR=src/common
MAIN=$(DIR)/main.c
FORMULA=$(DIR)/Formula.c $(DIR)/Formula.h
TREE=$(DIR)/Tree.c $(DIR)/Tree.h
SVG=$(DIR)/SVG.c $(DIR)/SVG.h
GLOBAL=$(DIR)/Global.c $(DIR)/Global.h
CONFIG=$(DIR)/Config.c $(DIR)/Config.h

$(TARGET): $(MAIN) flex bison $(FORMULA) $(TREE) $(SVG) $(GLOBAL) $(CONFIG)
	$(CC) $(MAIN) lex.yy.c grammar.tab.c $(FORMULA) $(TREE) $(SVG) $(GLOBAL) $(CONFIG) $(CFLAGS)

flex: $(DIR)/lex.l
	flex -o $(DIR)/lex.yy.c $(DIR)/lex.l

bison: $(DIR)/grammar.y
	bison -d $(DIR)/grammar.y -o $(DIR)/grammar.tab.c

clean:
	@rm -rf $(DIR)/lex.yy.c $(DIR)/grammar.tab.h $(DIR)/grammar.tab.c $(TARGET)

run:
	./tableaux prueba

fast:
	make && make run

.PHONY: clean
