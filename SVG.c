#include "SVG.h"

static const char* SIMBOLO_ABIERTO = "&#10004;";
static const char* SIMBOLO_CERRADO = "&#10008;";

void print_linea(int x1,int y1,int x2,int y2,FILE *fich) {
	ffprintf(fich,fich,"<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:black;stroke-width:2\" />\n",x1,y1+ALTURA_CARACTER/2,x2,y2-ALTURA_CARACTER);
}

void print_svg_recursivo(SVG_data *s,FILE *fich) {
	if(s->color == COLOR_DEFAULT)fprintf(fich,"<text x=\"%d\" y=\"%d\" style=\"fill:black;font-family:monospace\">%s</text>\n",s->x,s->y,s->formula);
    else {
      if(s->color == COLOR_RED) {
      	fprintf(fich,"<text x=\"%d\" y=\"%d\" style=\"fill:red;font-family:monospace\">%s</text>\n",s->x,s->y,s->formula);
      	fprintf(fich,"<text x=\"%f\" y=\"%d\" style=\"fill:red;font-family:monospace\">%s</text>\n",(double)s->x+s->centro,s->y+ALTURA_CARACTER,SIMBOLO_CERRADO);
      }
      else { 
      	fprintf(fich,"<text x=\"%d\" y=\"%d\" style=\"fill:green;font-family:monospace\">%s</text>\n",s->x,s->y,s->formula);
      	fprintf(fich,"<text x=\"%f\" y=\"%d\" style=\"fill:green;font-family:monospace\">%s</text>\n",(double)s->x+s->centro,s->y+ALTURA_CARACTER,SIMBOLO_ABIERTO);
      }
  }
  
	if(s->hi != NULL) {
		print_svg_recursivo(s->hi,fich);
		print_linea(s->x+s->centro, s->y, s->hi->x+s->hi->centro, s->hi->y,fich);
	}
	
	if(s->hd != NULL) {
		print_svg_recursivo(s->hd,fich);
		print_linea(s->x+s->centro, s->y, s->hd->x+s->hd->centro ,s->hd->y,fich);
	}
}

void print_svg(SVG *s, FILE *fich) {
	ffprintf(fich,fich,"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n");
	ffprintf(fich,fich,"<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%d\" height=\"%d\">\n",s->xmax,s->ymax);
	ffprintf(fich,fich,"<rect width=\"%d\" height=\"%d\" fill=\"white\">\n</rect>\n",s->xmax,s->ymax);
	print_svg_recursivo(s->data,FILE *fich);
	ffprintf(fich,fich,"</svg>\n");
}
