#ifndef __Config__
#define __Config__

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Global.h"
#include "Formula.h"
#include "grammar.tab.h"

#define MAX_CHARS 15
#define N_WORDS 5
#define N_OPTIONS 3

#define STATUS_CORRECTO       0
#define STATUS_INCORRECTO     1
#define STATUS_VACIO          2

#define OPTION_SVG_INDEX      0
#define OPTION_STDOUT_INDEX   1
#define OPTION_SVG_NAME_INDEX 2

#define MAX_GROUPS 3

#define BISON_TOKEN_ERROR	-1

static const char* WORDS[] = { "and", "or", "not", "imp", "dimp" };
static const char* OPTIONS[] = { "svg", "stdout", "svg_name" };

static const char* NOMBRE_DEFECTO_SVG = "sol.svg";

struct TablaTokens;
typedef struct TablaTokens* TablaTokens;

struct Match;
typedef struct Match *Match;

extern TablaTokens t;
extern int nlineas;

void GenerarTabla(FILE *fich);
void freeTablaTokens();
void print_TablaTokens();
int getCodigoToken(char* token);
int TablaCorrecta();

int mostrarTree();
int mostrarSVG();
char* nombreSVG();

#endif
