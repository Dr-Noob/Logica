#ifndef __Config__
#define __Config__

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Global.h"
#include "Formula.h"
#include "grammar.tab.h"

#define MAX_CHARS 10
#define N_WORDS 5

#define STATUS_CORRECTO       0
#define STATUS_INCORRECTO     1
#define STATUS_VACIO          2

static const char* WORDS[] = { "and", "or", "not", "imp", "dimp" };

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

#endif
