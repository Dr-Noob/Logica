#ifndef __Config__
#define __Config__

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Bool.h"

#define MAX_CHARS 10
#define N_WORDS 5

/*
static const char* AND_WORD = "and";
static const char* OR_WORD = "or";
static const char* NOT_WORD = "not";
static const char* IMP_WORD = "imp";
static const char* DIMP_WORD = "dimp";
*/
static const char* WORDS[] = { "and", "or", "not", "imp", "dimp" };

struct TablaTokens;
typedef struct TablaTokens* TablaTokens;

struct Match;
typedef struct Match *Match;

TablaTokens GenerarTabla(FILE *fich);
void freeTablaTokens(TablaTokens t);
void print_TablaTokens(TablaTokens t);

#endif
