#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Global.h"
#include "Config.h"

extern char *yytext;
extern int  yyleng;
extern int yyparse(FILE *fich);
extern FILE *yyin;
extern int yylex();
extern TablaTokens t;

int main( int argc, char* argv[] ) {
	int yyret = 0;

	if (argc != 2) {
		printMsgRed(MESSAGE_ARGS_MAIN);
		return 1;
	}
	char* nombre = argv[1];
	FILE *fich = fopen(nombre,"r");
	if (fich==NULL) {
		printMsgRed(MESSAGE_ABRIR_ARCHIVO_FALLIDO, nombre);
		return 1;
	}

	GenerarTabla(fich);
	if(!TablaCorrecta()) {
		fclose(fich);
		freeTablaTokens();
		return EXIT_FAILURE;
	}
	getLang();
	yyin = fich;
	yyret = yyparse(fich);
	fclose(fich);
	freeTablaTokens();
	return yyret;
}
