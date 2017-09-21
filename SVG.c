#include "SVG.h"

int offset = XI;

// Funciones para imprimir

void print_linea(int x1,int y1,int x2,int y2,FILE *fich) {
	fprintf(fich,"<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:black;stroke-width:2\" />\n",x1,y1+ALTURA_CARACTER/2,x2,y2-ALTURA_CARACTER);
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
	fprintf(fich,"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n");
	fprintf(fich,"<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%d\" height=\"%d\">\n",2000,2000);
	fprintf(fich,"<rect width=\"%d\" height=\"%d\" fill=\"white\">\n</rect>\n",s->xmax,s->ymax);
	print_svg_recursivo(s->data,fich);
	fprintf(fich,"</svg>\n");
}

// FIN Funciones para imprimir

void AjustarHijos(SVG_data *s,int incX,int incY,int nivel,Nodo nodos[MAX_NIVELES]) {
	if(s->hi != NULL)AjustarHijos(s->hi,incX,incY,nivel,nodos);
	if(s->hd != NULL)AjustarHijos(s->hi,incX,incY,nivel,nodos);

	//Insertar(nodos,nivel,s);
}

int nCaracteres(char* cadena) {
  int count = 0;
  for(int m=0; cadena[m]; m++) {
    if(cadena[m] == '&')while(cadena[m] != ';')m++;//Contar como un caracter los simbolos
    count ++;
  }
  return count;
}

SVG_data *CrearSVGDesdeTableauxRecursivo(SVG_data *s,Tableaux t,int nivel,Nodo nodos[MAX_NIVELES]) {
	s = malloc(sizeof(SVG_data));
	if(t->ti != NULL)s->hi = CrearSVGDesdeTableauxRecursivo(s->hi,t->ti,nivel+1,nodos);
	if(t->td != NULL)s->hd = CrearSVGDesdeTableauxRecursivo(s->hd,t->td,nivel+1,nodos);
	
  if(t->ti != NULL && t->td != NULL) { //Ajustar hijos
    while(s->hi->xmax + 20 > s->hd->x)s->hd->x++;
    //AjustarHijos(s,incX,incY,nivel,nodos);
  }
	char *buffer = malloc(sizeof(char)*MAX_CHAR);
  s->formula = show_ascii(buffer,t->f);
  int caracteres = nCaracteres(buffer);
  s->centro = (caracteres/2)*PIXELES_POR_CARACTER;
  s->y = nivel*INCY+YI;

	if(t->etiqueta != VACIO) {	//COLOR
    if(t->etiqueta == CERRADO)s->color = COLOR_RED;
    else s->color = COLOR_GREEN;
  }

	if(t->ti == NULL) {
		s->x = offset;
		offset += INCX;
	}
	else if (t->td != NULL) s->x = s->hi->x + (s->hd->x - s->hi->x)/2;
	else s->x = s->hi->x+(s->hi->centro-s->centro);

  s->xmax = s->x+caracteres*PIXELES_POR_CARACTER;

	return s;
}

SVG *CrearSVGDesdeTableaux(SVG *s, Tableaux t)  {
	int nivel = 0;
	s = malloc(sizeof(struct SVG));
	Nodo nodos[MAX_NIVELES];
	s->data = CrearSVGDesdeTableauxRecursivo(s->data,t,nivel,nodos);
  return s;
}

void showTableauxSVG(Tableaux t, FILE *fich) {
	SVG *s = CrearSVGDesdeTableaux(s,t);
	print_svg(s,fich);
}
