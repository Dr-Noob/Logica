%{
#include "Formula.h"
#include "Config.h"
#include "grammar.tab.h"
#include <stdlib.h>
#include <string.h>
%}

letra  [a-z]

%option yylineno

%%
	/*Descartar*/
[ \n\t]+													;

{letra}														yylval.a = yytext[0]; return ATOM;
"("																return LPAREN;
")"																return RPAREN;
","																return COMA;

	/*Operadores										Obtener de fichero de configuracion */
[^ \t\n(),]+											return getCodigoToken(yytext);

	/* EOF */
<<EOF>>														return END_OF_FILE;

	/*Error*/
.												{ fprintf(stderr, "\x1b[1;31m""ERROR: \"%s\" Caracter/es no reconocido/s""\x1b[0m""\n", yytext); yyterminate(); }

%%
