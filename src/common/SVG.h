#ifndef __SVG__
#define __SVG__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Global.h"
#include "Formula.h"

#ifdef WINDOWS
#include "../windows/Fuente.h"
#else
#include "../unix/Fuente.h"
#endif

typedef struct SVG_data SVG_data;
typedef struct SVG SVG;
typedef struct NodoRep *Nodo;
typedef struct CoordenadasRep *Coordenadas;

#define ALTURA_CARACTER 15
#define INCY 140 //Incremento y
#define INCX 300 //Incremento x
#define YI 50 //y inicial
#define XI 50 //x inicial

extern int MAX_NIVELES;

static const char* SIMBOLO_ABIERTO = "&#10004;";
static const char* SIMBOLO_CERRADO = "&#10008;";

static const char* SVG_SIMBOLO_AND = " &#8743; ";
static const char* SVG_SIMBOLO_OR = " &#8744; ";
static const char* SVG_SIMBOLO_IMP = " &#8594; ";
static const char* SVG_SIMBOLO_DIMP = " &#8596; ";
static const char* SVG_SIMBOLO_NOT = "&#172;";

static const char* ASCII_SIMBOLO_AND = " ^ ";
static const char* ASCII_SIMBOLO_OR = " v ";
static const char* ASCII_SIMBOLO_IMP = " -> ";
static const char* ASCII_SIMBOLO_DIMP = " <-> ";
static const char* ASCII_SIMBOLO_NOT = "~";

static const float PIXELES_POR_CARACTER = 7.2025;

static const char* NOMBRE_DEFECTO_SVG = "sol.svg";

void showTableauxSVG(Tableaux t, FILE *fich, int nNodos);

#endif
