#include "Formula.h"

SVG CrearSVG(int _x,int _y,char* _formula) {
	SVG s = malloc(sizeof(struct SVG));
	s->x = _x;
	s->y = _y;
	strcpy(s->formula,_formula);
	return s;
}

void print_svg(SVG s) {
	printf("<svg height=\"1000\" witdh=\"1000\" xlms=\"https://www.w3.org/2000/svg\" >\n");
	print_svg_recursivo(s);
	printf("</svg>");
}

void print_svg_recursivo(SVG s) {
	printf("<\"text x=\"%d\" y=\"%d\" style=\"fill:black;\">%s</text>\n",s->x,s->y,s->formula);
	if(s->hi != NULL)print_svg_recursivo(s->hi);
	if(s->hd != NULL)print_svg_recursivo(s->hd);
}
