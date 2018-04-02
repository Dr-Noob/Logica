DIR_COMMON=src/common
DIR_UNIX=src/unix
DIR_WINDOWS=src/windows

TREE=$(DIR_COMMON)/Tree.c $(DIR_COMMON)/Tree.h
SVG=$(DIR_COMMON)/SVG.c $(DIR_COMMON)/SVG.h
GLOBAL=$(DIR_COMMON)/Global.c $(DIR_COMMON)/Global.h
FORMULA=$(DIR_COMMON)/Formula.c $(DIR_COMMON)/Formula.h
BISON=$(DIR_COMMON)/grammar.y
H_CONTROLADOR=$(DIR_COMMON)/Controlador.h

ifeq ($(OS),Windows_NT)
	MAIN=$(DIR_WINDOWS)/main.c
	FLEX=$(DIR_WINDOWS)/lex.l
	CONTROLADOR=$(DIR_WINDOWS)/Controlador.c
	BASENAME=$(DIR_WINDOWS)/basename.c
	PLATFORM_FLAG=WINDOWS
else
	MAIN=$(DIR_UNIX)/main.c
	CONFIG=$(DIR_UNIX)/Config.c $(DIR_UNIX)/Config.h
	FLEX=$(DIR_UNIX)/lex.l
	CONTROLADOR=$(DIR_UNIX)/Controlador.c
	BASENAME=$(DIR_UNIX)/basename.c
	PLATFORM_FLAG=UNIX
endif

TARGET=tableaux
CFLAGS=-g -fstack-protector-all -Wall -pedantic -Wno-unused -Werror -o $(TARGET) -I $(DIR_COMMON) -D $(PLATFORM_FLAG)
CFLAGS_RELEASE=-fstack-protector-all -Wall -pedantic -Wno-unused -Werror -o $(TARGET) -I $(DIR_COMMON) -D $(PLATFORM_FLAG) -static -O2
CC=gcc

$(TARGET): $(MAIN) src/lex.yy.c src/grammar.tab.c $(FORMULA) $(TREE) $(SVG) $(GLOBAL) $(CONFIG) $(H_CONTROLADOR) $(CONTROLADOR) $(BASENAME)
	$(CC) $(MAIN) src/lex.yy.c src/grammar.tab.c $(FORMULA) $(TREE) $(SVG) $(GLOBAL) $(CONFIG) $(CONTROLADOR) $(BASENAME) $(CFLAGS)

release: $(MAIN) src/lex.yy.c src/grammar.tab.c $(FORMULA) $(TREE) $(SVG) $(GLOBAL) $(CONFIG) $(H_CONTROLADOR) $(CONTROLADOR) $(BASENAME)
	$(CC) $(MAIN) src/lex.yy.c src/grammar.tab.c $(FORMULA) $(TREE) $(SVG) $(GLOBAL) $(CONFIG) $(CONTROLADOR) $(BASENAME) $(CFLAGS_RELEASE)

src/lex.yy.c: $(FLEX)
	flex -o src/lex.yy.c $(FLEX)

src/grammar.tab.c: $(BISON)
	bison -o src/grammar.tab.c -d $(BISON)

clean:
	@rm -rf src/lex.yy.c src/grammar.tab.h src/grammar.tab.c $(TARGET)

run:
	./tableaux prueba

fast:
	make && make run

.PHONY: clean run fast release
