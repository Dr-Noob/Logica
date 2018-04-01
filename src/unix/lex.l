%{
#include "unix/Config.h"
#include "Formula.h"
#include "grammar.tab.h"
#include <stdlib.h>
#include <string.h>

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
[ \n\t\r]+													{ yylloc.last_column += yyleng; }

{letra}														{ POSICION; yylval.a = yytext[0]; return ATOM; }
"("																{ POSICION; return LPAREN; }
")"																{ POSICION; return RPAREN; }
"{"																{ POSICION; return LPAREN; }
"}"																{ POSICION; return RPAREN; }
","																{ POSICION; return COMA; }

	/*Operadores										Obtener de fichero de configuracion */
[^ \t\n\r(){},]+											{ POSICION; return getCodigoToken(yytext); }

	/* EOF */
<<EOF>>														{ POSICION; return END_OF_FILE; }

%%
