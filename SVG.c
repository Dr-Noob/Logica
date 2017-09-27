#include "SVG.h"

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

struct NodoRep {
	SVG_data* svg;
	Nodo sig;
};

struct PuntoRep {
	int x;
	int y;
};

int offset = XI;

void LiberarSVG_data(SVG_data *s) {
	if(s->hi != NULL)LiberarSVG_data(s->hi);
	if(s->hd != NULL)LiberarSVG_data(s->hd);
	free(s->formula);
	free(s);
}

void LiberarSVG(SVG *s) {
	LiberarSVG_data(s->data);
	free(s);
}

void LiberarNodo(Nodo n) {
	//No liberar el SVG porque no se hace una
	//copia, sino que es una referencia
	if(n->sig != NULL)LiberarNodo(n->sig);
	free(n);
}

Punto MaxMinRecursivo(SVG_data *s, Punto p) {
		if(s->hi != NULL)MaxMinRecursivo(s->hi,p);
		if(s->hd != NULL)MaxMinRecursivo(s->hd,p);

		if((int)s->xmax > p->x)p->x = (int)s->xmax;
		if(s->y > p->y)p->y = s->y;
		return p;
}

Punto MaxMin(SVG_data *s) {
		Punto p = malloc(sizeof(Punto));
		memset(p,0,sizeof(Punto));
		p->x = 0;
		p->y = 0;
		p = MaxMinRecursivo(s,p);
		return p;
}

void LiberarPunto(Punto p) {
	free(p);
}


// Funciones para imprimir

void print_linea(int x1,int y1,int x2,int y2,FILE *fich) {
	fprintf(fich,"<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:black;stroke-width:2\" />\n",x1,y1+ALTURA_CARACTER/2,x2,y2-ALTURA_CARACTER);
}

void print_svg_recursivo(SVG_data *s,FILE *fich) {
	//printf("width=%f porque xmax=%f y x=%d\n",s->xmax-(float)s->x,s->xmax,s->x);
	//fprintf(fich,"<rect x=\"%d\" y=\"%d\" width=\"%f\" height=\"%d\" style=\"fill:none;stroke-width:1;stroke:rgb(0,0,0)\" />\n",s->x,s->y,s->xmax-(float)s->x,10);
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
	Punto p = MaxMin(s->data);
	fprintf(fich,"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n");
	fprintf(fich,"<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%d\" height=\"%d\">\n",p->x + XI,p->y + (int)PIXELES_POR_CARACTER + YI);
	fprintf(fich,"<rect width=\"%d\" height=\"%d\" fill=\"white\">\n</rect>\n",p->x + XI,p->y + (int)PIXELES_POR_CARACTER + YI);
	print_svg_recursivo(s->data,fich);
	fprintf(fich,"</svg>\n");
	LiberarPunto(p);
}

// FIN Funciones para imprimir

// Funciones para ajustar los hijos

void Insertar(SVG_data *s,int nivel,Nodo nodos[MAX_NIVELES]) {
	Nodo n = nodos[nivel];
	int nodoIgual = BOOLEAN_FALSE;

	while(n->sig != NULL)n = n->sig;

	n->sig = malloc(sizeof(struct NodoRep));
	n = n->sig;
	memset(n,0,sizeof(struct NodoRep));
	n->svg = s;
	n->sig = NULL;
}

void Rellenar(SVG_data *s,int nivel,Nodo nodos[MAX_NIVELES]) {
	if(s->hi != NULL)Rellenar(s->hi,nivel+1,nodos);
	if(s->hd != NULL)Rellenar(s->hd,nivel+1,nodos);

	Insertar(s,nivel,nodos);
}

void print_nodos(Nodo nodos[MAX_NIVELES]) {
		printf("\tMe llaman\n");
		Nodo n;
		for(int i=0;i<MAX_NIVELES;i++){
			if(nodos[i]->sig != NULL) {
				printf("Nivel %d\n",i);
				n = nodos[i]->sig;
				do {
					printf("%s\n",n->svg->formula);
					n = n->sig;
				} while(n->sig != NULL);
				printf("%s\n",n->svg->formula);
			}
		}
}

int HayColisionesEnFila(Nodo n) {
	Nodo tmp;
	while(n->sig != NULL) {
		tmp = n;
    do {
			tmp = tmp->sig;
			if(n->svg != NULL && n->svg->xmax + 20 > tmp->svg->x) { //Comprobar primero que no es la cabecera
				while(n->svg->xmax +20 > tmp->svg->x) {
					tmp->svg->x++;
					tmp->svg->xmax++;
				}
				return BOOLEAN_TRUE;
			}
		} while(tmp->sig != NULL);
		n = n->sig;
  }
	return BOOLEAN_FALSE;
}

int ComprobarTodasColisiones(SVG_data *s,int nivel, Nodo nodos[MAX_NIVELES]) {

	do {
		if(HayColisionesEnFila(nodos[nivel]) == BOOLEAN_TRUE)return BOOLEAN_TRUE;
		nivel++;
	}while(nodos[nivel]->sig != NULL);

	return BOOLEAN_FALSE;
}

void AjustarHijos(SVG_data *s,int nivel,Nodo nodos[MAX_NIVELES]);

void AjustarHijos(SVG_data *s,int nivel,Nodo nodos[MAX_NIVELES]) {
	for(int i=0;i<MAX_NIVELES;i++){
		nodos[i] = malloc(sizeof(struct NodoRep));
		memset(nodos[i],0,sizeof(struct NodoRep));
		nodos[i]->sig = NULL;
	}
	Rellenar(s->hi,nivel,nodos);
	Rellenar(s->hd,nivel,nodos);
	int ret = ComprobarTodasColisiones(s,nivel,nodos);
	for(int i=0;i<MAX_NIVELES;i++)LiberarNodo(nodos[i]);
	if(ret == BOOLEAN_TRUE)AjustarHijos(s,nivel,nodos); //ajustar hasta que todos lo esten
}

//FIN Funciones para ajustar los hijos

int nCaracteres(char* cadena) {
  int count = 0;
  for(int m=0; cadena[m]; m++) {
    if(cadena[m] == '&')while(cadena[m] != ';')m++;//Contar como un caracter los simbolos
    count ++;
  }
  return count;
}

SVG_data *CrearSVGDesdeTableauxRecursivo(SVG_data *s,Tableaux t,int nivel,Nodo nodos[MAX_NIVELES]) {
	s = malloc(sizeof(struct SVG_data));
	memset(s,0,sizeof(struct SVG_data));
	if(t->ti != NULL)s->hi = CrearSVGDesdeTableauxRecursivo(s->hi,t->ti,nivel+1,nodos);
	if(t->td != NULL)s->hd = CrearSVGDesdeTableauxRecursivo(s->hd,t->td,nivel+1,nodos);

  if(t->ti != NULL && t->td != NULL)AjustarHijos(s,nivel,nodos);

	char *buffer = malloc(sizeof(char)*MAX_CHAR);
	memset(buffer,0,sizeof(char)*MAX_CHAR);
	s->formula = show_svg(buffer,t->f);
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
	else if (t->td != NULL)s->x = (s->hi->x + s->hd->xmax-caracteres*PIXELES_POR_CARACTER)/2;
	else s->x = s->hi->x+(s->hi->centro-s->centro);

  s->xmax = s->x+caracteres*(int)PIXELES_POR_CARACTER;
	return s;
}

SVG *CrearSVGDesdeTableaux(SVG *s, Tableaux t)  {
	int nivel = 0;
	s = malloc(sizeof(struct SVG));
	memset(s,0,sizeof(struct SVG));
	Nodo nodos[MAX_NIVELES];
	s->data = CrearSVGDesdeTableauxRecursivo(s->data,t,nivel,nodos);
  return s;
}

void showTableauxSVG(Tableaux t, FILE *fich) {
	SVG *s = CrearSVGDesdeTableaux(s,t);
	print_svg(s,fich);
	LiberarSVG(s);
}
