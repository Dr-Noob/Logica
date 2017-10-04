%{
#include "Formula.h"
#include "Config.h"
#include "grammar.tab.h"
#include <stdlib.h>
#include <string.h>
%}

letra  [a-z]

%option yylineno
%option noyywrap

%%
	/*Descartar*/
[ \n\t]+													;

{letra}														{ yylval.a = yytext[0]; return ATOM; }
"("																return LPAREN;
")"																return RPAREN;
","																return COMA;

	/*Operadores										Obtener de fichero de configuracion */
[^ \t\n(),]+											return getCodigoToken(yytext);

	/* EOF */
<<EOF>>														return END_OF_FILE;


%%
