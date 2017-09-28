#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Global.h"

extern char *yytext;
extern int  yyleng;
extern int yyparse(FILE *fich);
extern FILE *yyin;
extern int yylex();

int main( int argc, char* argv[] ) {
	int yyret = 0;
	if (argc != 2) {
		printf(RED "ERROR: Tableaux necesita al menos un argumento(el archivo donde se encuentra la formula)\n" RESET);
		return 1;
	}
	char* nombre = argv[1];
	FILE *fich = fopen(nombre,"r");
	if (fich==NULL) {
		printf(RED "ERROR: El archivo %s no ha podido abrirse\n" RESET, nombre);
		return 1;
	}
	yyin = fich;
	yyret = yyparse(fich);
	fclose(fich);
	return yyret;
}
