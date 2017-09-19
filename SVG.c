#include "SVG.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

SVG *CrearSVG(int _x,int _y,char* _formula) {
	SVG *s = malloc(sizeof(struct SVG));
	s->x = _x;
	s->y = _y;
	s->hi = NULL;
	s->hd = NULL;
	strcpy(s->formula,_formula);
	return s;
}

void print_svg_recursivo(SVG *s) {
	printf("<text x=\"%d\" y=\"%d\" style=\"fill:black;\">%s</text>\n",s->x,s->y,s->formula);
	if(s->hi != NULL)print_svg_recursivo(s->hi);
	if(s->hd != NULL)print_svg_recursivo(s->hd);
}

void print_svg(SVG *s) {
	printf("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n");
	printf("<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"2000\" height=\"2000\">\n");
	printf("<rect width=\"2000\" height=\"2000\" fill=\"white\">\n</rect>\n");
	print_svg_recursivo(s);
	printf("</svg>\n");
}
