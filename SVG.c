#include "SVG.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void print_linea(int x1,int y1,int x2,int y2) {
	printf("<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:black;stroke-width:2\" />\n",x1,y1+ALTURA_CARACTER/2,x2,y2-ALTURA_CARACTER);
}

void print_svg_recursivo(SVG_data *s) {
	printf("<text x=\"%d\" y=\"%d\" style=\"fill:black;font-family:monospace\">%s</text>\n",s->x,s->y,s->formula);
	//printf("<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:none;stroke-width:1;stroke:rgb(0,0,0)\" />\n",s->x,s->y,s->xmax,s->ymax);
	//printf("<text x=\"%d\" y=\"%d\" style=\"fill:black;\">a</text>\n",s->x,s->y);
	if(s->hi != NULL) {
		print_svg_recursivo(s->hi);
		print_linea(s->x+s->centro, s->y, s->hi->x+s->hi->centro, s->hi->y);
	}
	if(s->hd != NULL) {
		print_svg_recursivo(s->hd);
		print_linea(s->x+s->centro, s->y, s->hd->x+s->hd->centro ,s->hd->y);
	}
}

void print_svg(SVG *s) {
	printf("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n");
	printf("<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%d\" height=\"%d\">\n",s->xmax,s->ymax);
	printf("<rect width=\"%d\" height=\"%d\" fill=\"white\">\n</rect>\n",s->xmax,s->ymax);
	print_svg_recursivo(s->data);
	printf("</svg>\n");
}
