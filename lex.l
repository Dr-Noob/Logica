%{
#include "Formula.h"
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
T																	return TRUE;
F																	return FALSE;
"("																	return LPAREN;
")"																	return RPAREN;
","																	return COMA;
	/*Operadores*/
and																return AND;
or																return OR;
not																return NOT;
imp																return IMP;
dimp															return DIMP;

	/*Error*/
[^a-zA-Z0-9_"/;,+\-*=(){}\n><&|!]+			fprintf(stderr, "\x1b[1;31m""ERROR:""\x1b[0m""\x1b[31m"" \"%s\" Token not recognised""\x1b[0m""\n", yytext);
%%
