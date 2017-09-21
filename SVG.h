#ifndef __SVG__
#define __SVG__

#include "Tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct SVG_data SVG_data;
typedef struct SVG SVG;

#define ALTURA_CARACTER 15
#define PIXELES_POR_CARACTER 7

struct SVG {
	SVG_data* data;
	int xmax;
	int ymax;
};

struct SVG_data {
	int x;
	int y;
	int xmax;
	int color;
	float centro;
	char* formula;
	SVG_data* hi;
	SVG_data* hd;
};

void print_svg(SVG *s, FILE *fich);

#endif
