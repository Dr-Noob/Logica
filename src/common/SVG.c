#include "SVG.h"
#include "basename.h"

#define MSG_SAT_X 12
#define MSG_SAT_Y 22

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

struct CoordenadasRep {
	int xmax;
	int ymax;
	int xmin;
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

Coordenadas MaxMinRecursivo(SVG_data *s, Coordenadas c,SVG_data ** nodos,int* i) {
		nodos[*i] = s;
		*i += 1;
		if(s->hi != NULL)MaxMinRecursivo(s->hi,c,nodos,i);
		if(s->hd != NULL)MaxMinRecursivo(s->hd,c,nodos,i);

		if(s->xmax > c->xmax)c->xmax = s->xmax;
		if(s->y > c->ymax)c->ymax = s->y;
		if(s->x < XI || s->x < c->xmin)c->xmin = s->x;
		return c;
}

Coordenadas MaxMin(SVG_data *s,SVG_data ** nodos) {
		int* i = malloc(sizeof(int));
		memset(i,0,sizeof(int));
		Coordenadas c = malloc(sizeof(struct CoordenadasRep));
		memset(c,0,sizeof(struct CoordenadasRep));

		c->xmin = XI;
		c = MaxMinRecursivo(s,c,nodos,i);
		free(i);
		return c;
}

void LiberarCoordenadas(Coordenadas c) {
	free(c);
}


// Funciones para imprimir

void print_linea(int x1,int y1,int x2,int y2,FILE *fich) {
	fprintf(fich,"<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:black;stroke-width:2\" />\n",x1,y1+ALTURA_CARACTER/2,x2,y2-ALTURA_CARACTER);
}

void print_svg_recursivo(SVG_data *s,FILE *fich) {
	//printf("width=%f porque xmax=%f y x=%d\n",s->xmax-(float)s->x,s->xmax,s->x);
	//fprintf(fich,"<rect x=\"%d\" y=\"%d\" width=\"%f\" height=\"%d\" style=\"fill:none;stroke-width:1;stroke:rgb(0,0,0)\" />\n",s->x,s->y,s->xmax-(float)s->x,10);
	if(s->color == COLOR_DEFAULT)fprintf(fich,"<text x=\"%d\" y=\"%d\" style=\"fill:black;font-family:%s\">%s</text>\n",s->x,s->y,FUENTE,s->formula);
    else {
      if(s->color == COLOR_RED) {
      	fprintf(fich,"<text x=\"%d\" y=\"%d\" style=\"fill:red;font-family:%s\">%s</text>\n",s->x,s->y,FUENTE,s->formula);
      	fprintf(fich,"<text x=\"%f\" y=\"%d\" style=\"fill:red;font-family:%s\">%s</text>\n",(double)s->x+s->centro,s->y+ALTURA_CARACTER,FUENTE,SIMBOLO_CERRADO);
      }
      else {
      	fprintf(fich,"<text x=\"%d\" y=\"%d\" style=\"fill:green;font-family:%s\">%s</text>\n",s->x,s->y,FUENTE,s->formula);
      	fprintf(fich,"<text x=\"%f\" y=\"%d\" style=\"fill:green;font-family:%s\">%s</text>\n",(double)s->x+s->centro,s->y+ALTURA_CARACTER,FUENTE,SIMBOLO_ABIERTO);
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

//Correccion del arbol en el caso de que se mueva a la izquierda al haber
//continuamente ramas verticales con distinto tamano(a and a and a and ...)
void AjustarDerecha(Coordenadas c, SVG_data ** nodos,int nNodos) {
	int dif = XI - c->xmin;
	if(c->xmin < XI) {
		for(int i=0;i<nNodos;i++) {
			nodos[i]->x += dif;
			nodos[i]->xmax += dif;
		}
		c->xmax += dif;
	}
}

void print_svg(SVG *s, FILE *fich, SVG_data ** nodos, int nNodos, int tableauxCerrado) {
	Coordenadas c = MaxMin(s->data,nodos);
	AjustarDerecha(c,nodos,nNodos);
	fprintf(fich,"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n");
	fprintf(fich,"<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%d\" height=\"%d\">\n",c->xmax + XI,c->ymax + (int)PIXELES_POR_CARACTER + YI);
	fprintf(fich,"<rect width=\"%d\" height=\"%d\" fill=\"white\">\n</rect>\n",c->xmax + XI,c->ymax + (int)PIXELES_POR_CARACTER + YI);
	if(tableauxCerrado)
		fprintf(fich,"<text x=\"%d\" y=\"%d\" style=\"fill:red;font-family:monospace\">INSATISFACIBLE</text>\n",MSG_SAT_X,MSG_SAT_Y);
	else
		fprintf(fich,"<text x=\"%d\" y=\"%d\" style=\"fill:green;font-family:monospace\">SATISFACIBLE</text>\n",MSG_SAT_X,MSG_SAT_Y);

	print_svg_recursivo(s->data,fich);
	fprintf(fich,"</svg>\n");
	LiberarCoordenadas(c);
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

//Devuelve el nombre por defect para el SVG
// que se calcula a partir de la ruta donde
//se encuentra el fichero de entrada
char* getNombreDefecto(char* path_fich) {
  static const char* STRING_TERMINACION = "-sol.svg";
	char* nombre_fichero = getBasename(path_fich);
	int len = strlen(nombre_fichero);
  char* nombre = malloc(sizeof(char)*(len+strlen(STRING_TERMINACION)));
  strcpy(nombre,nombre_fichero);

  //A partir del ultimo caracter hay que escribir STRING_TERMINACION
  strcpy(nombre+len,STRING_TERMINACION);
  return nombre;
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

SVG *CrearSVGDesdeTableaux(Tableaux t)  {
	SVG *s = NULL;
	int nivel = 0;
	s = malloc(sizeof(struct SVG));
	memset(s,0,sizeof(struct SVG));
	Nodo nodos[MAX_NIVELES];
	s->data = CrearSVGDesdeTableauxRecursivo(s->data,t,nivel,nodos);
  return s;
}

void showTableauxSVG(Tableaux t, FILE *fich, int nNodos, int tableauxCerrado) {
	SVG *s = CrearSVGDesdeTableaux(t);
	SVG_data ** nodos = malloc(sizeof(struct SVG_data*)+sizeof(struct SVG_data*)*nNodos);
	memset(nodos,0,sizeof(struct SVG_data*)+sizeof(struct SVG_data*)*nNodos);

	print_svg(s,fich,nodos,nNodos,tableauxCerrado);

	LiberarSVG(s);
	free(nodos);
}
