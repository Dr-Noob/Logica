%{
#include "Formula.h"
#include "Config.h"
#include "grammar.tab.h"
#include <stdlib.h>
#include <string.h>

int error(int linea, int columna,char* yytext);
#define POSICION \
	yylloc.first_line = yylloc.last_line; \
	yylloc.first_column = yylloc.last_column + 1; \
	yylloc.last_column += yyleng;
%}

letra  [a-z]

%option yylineno
%option noyywrap

%%
	/*Descartar*/
[ \n\t]+													{ yylloc.last_column += yyleng; }

{letra}														{ POSICION; yylval.a = yytext[0]; return ATOM; }
"("																{ POSICION; return LPAREN; }
")"																{ POSICION; return RPAREN; }
","																{ POSICION; return COMA; }

	/*Operadores										Obtener de fichero de configuracion */
[^ \t\n(),]+											{ POSICION; return getCodigoToken(yytext); }

	/* EOF */
<<EOF>>														{ POSICION; return END_OF_FILE; }

	/*Error*/
.																	{ error(yylloc.last_line, yylloc.last_column, yytext); yylloc.last_column++; }

%%

int error(int linea, int columna,char* yytext) {
	fprintf(stderr, "\x1b[1;31m""ERROR: \"%s\" Caracter/es no reconocido/s(Columna %d)""\x1b[0m""\n", yytext,columna);
}
