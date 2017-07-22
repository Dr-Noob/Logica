#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RED "\x1b[31;1m"
#define RESET "\x1b[0m"

extern char *yytext;
extern int  yyleng;
extern int yyparse();
extern FILE *yyin;
extern int yylex();

int main( int argc, char* argv[] ) {
	int yyret = 0;
	if (argc != 2) {
		printf(RED "ERROR: Tableaux necesita un argumento(el archivo)\n" RESET);
		return 1;
	}
	char* nombre = argv[1];
	FILE *fich = fopen(nombre,"r");
	if (fich==NULL) {
		printf(RED "ERROR: El archivo %s no ha podido abrirse\n" RESET, nombre);
		return 1;
	}
	yyin = fich;
	yyret = yyparse();
	fclose(fich);
	return yyret;
}
