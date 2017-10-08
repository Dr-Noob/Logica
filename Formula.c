#include "Formula.h"
#include "SVG.h"
#include "Config.h"

struct AtomoRep {
  char id; //Identifica#include <locale.h>dor
  int not; //NEGADO-> el atomo va negado, SIN_NEGAR-> no va negado
};

struct FormulaRep {
  Atomo a1;	//Atomo1
  Atomo a2;	//Atomo2
  Formula f1; //Formula anidada
  Formula f2; //Formula anidada
  int COD_OP;	//Codigo operador
  int not; //NEGADO-> el atomo va negado, SIN_NEGAR-> no va negado
  struct FormulaRep *sig; //Siguiente formula
};

struct TableauxInfo {
  int nodos;
  int niveles;
};

Formula CrearFormula(Atomo a) {
	Formula f = malloc(sizeof(struct FormulaRep));
  memset(f,0,sizeof(struct FormulaRep));
  Atomo copia = malloc(sizeof(struct AtomoRep));
  copia->id = a->id;
  copia->not = a->not;
	f->a1 = copia;
  f->a2 = NULL;
  f->f1 = NULL;
  f->f2 = NULL;
	f->COD_OP = COD_INVALIDO;
  f->not = SIN_NEGAR;
  f->sig = NULL;
	return f;
}

void LiberarAtomo(Atomo a) {
  free(a);
}

void LiberarFormula(Formula f) {
  if(f->f1 != NULL)LiberarFormula(f->f1);
  if(f->f2 != NULL)LiberarFormula(f->f2);
  if(f->a1 != NULL)LiberarAtomo(f->a1);
  if(f->a2 != NULL)LiberarAtomo(f->a2);
  free(f);
}

void LiberarFormulaCompleta(Formula f) {
  if(f->sig != NULL)LiberarFormulaCompleta(f->sig);
  if(f->f1 != NULL)LiberarFormulaCompleta(f->f1);
  if(f->f2 != NULL)LiberarFormulaCompleta(f->f2);
  if(f->a1 != NULL)LiberarAtomo(f->a1);
  if(f->a2 != NULL)LiberarAtomo(f->a2);
  free(f);
}

void LiberarTableaux(Tableaux t) {
  if(t->ti != NULL)LiberarTableaux(t->ti);
  if(t->td != NULL)LiberarTableaux(t->td);
  LiberarFormulaCompleta(t->f);
  free(t);
}

Atomo CrearAtomo(char _id,int _not) {
  Atomo a = malloc(sizeof(struct AtomoRep));
  a->id = _id;
  a->not = _not;
  return a;
}

Atomo CopiarAtomo(Atomo original) {
    return CrearAtomo(original->id, original->not);
}

//Devuelve el unico atomo que  contiene(si tiene mas de uno, devuelve NULL)
Atomo ExtraerAtomo(Formula f) {
  if(f->f1 == NULL && f->f2 == NULL) {
    if(f->a1 == NULL)return f->a2;
    else if(f->a2 == NULL)return f->a1;
    else return NULL;
  }
  return NULL;
}

int SoloTieneUnAtomo(Formula f) {
  return(f->f1 == NULL && (f->a1 != NULL ^ f->a2 != NULL) && f->f2 == NULL);
}

int EsAlfaFormula(Formula f) {
  return ((f->COD_OP == COD_AND) || (f->COD_OP == COD_OR && f->not == NEGADO) || (f->COD_OP == COD_IMP && f->not == NEGADO) || (f->COD_OP == COD_DIMP));
}

Formula NegarFormula(Formula f) {
  if(SoloTieneUnAtomo(f)){
    if(f->a1 == NULL) { //Negar a2
      if (f->a2->not == NEGADO)f->a2->not = SIN_NEGAR;
      else f->a2->not = NEGADO;
    }
    else { //Negar a1
      if(f->a1->not == NEGADO)f->a1->not = SIN_NEGAR;
      else f->a1->not = NEGADO;
    }
  }
  else { //Negar la formula
    if(f->not == NEGADO)f->not = SIN_NEGAR;
    else f->not = NEGADO;
  }
  return f;
}

Formula CopiarFormula(Formula original) {
  Formula copia=NULL;
  Formula anterior=NULL;
  Formula temp = NULL;
  Atomo a1;
  Atomo a2;

  while(original != NULL) {
    //Copiar
    temp = malloc(sizeof(struct FormulaRep));
    memset(temp,0,sizeof(struct FormulaRep));
    if(original->a1 != NULL) {
      a1 = CrearAtomo(original->a1->id,original->a1->not);
      temp->a1=a1;
    }
    if(original->a2 != NULL) {
      a2 = CrearAtomo(original->a2->id,original->a2->not);
      temp->a2=a2;
    }
    if(original->f1 != NULL)temp->f1 = CopiarFormula(original->f1);
    if(original->f2 != NULL)temp->f2 = CopiarFormula(original->f2);
    temp->COD_OP=original->COD_OP;
    temp->not = original->not;

    //Enlazar
    if(copia==NULL){
        copia=temp;
        anterior=temp;
    }
    else {
        anterior->sig=temp;
        anterior=temp;
    }
    original=original->sig;
  }
  return copia;
}

Formula Unir(Formula formula1,int operador,Formula formula2) {
  Formula f = malloc(sizeof(struct FormulaRep));
  memset(f,0,sizeof(struct FormulaRep));

  if(formula1 == NULL || formula2 == NULL) {
    printf("ERROR: Formula nula\n");
    return NULL;
  }

  if(formula1->f1 == NULL) { //formula1 no tiene la primera formula
    if(formula1->f2 == NULL) { //formula1 no tiene ni la primera ni la segunda formula
      if(formula1->a2 == NULL) { //formula1 solo tiene un atomo
        f->a1 = CopiarAtomo(formula1->a1);
        if(formula1->not == NEGADO)f->a1->not = NEGADO;
      }
      else { //formula1 tiene los dos atomos
        Formula aux = malloc(sizeof(struct FormulaRep));
        memset(aux,0,sizeof(struct FormulaRep));
        aux->a1 = CopiarAtomo(formula1->a1);
        aux->a2 = CopiarAtomo(formula1->a2);
        aux->not = NEGADO;
        aux->COD_OP = formula1->COD_OP;
        aux->not = formula1->not;
        f->f1 = aux;
      }
    }
    else { //formula1 si tiene la segunda formula
      if(formula1->a1 == NULL) { //formula1 no tiene atomos
        f->f1 = CopiarFormula(formula1);
      }
      else { //formula1 tiene un atomo(el a1)
        Formula aux = malloc(sizeof(struct FormulaRep));
        memset(aux,0,sizeof(struct FormulaRep));
        aux->f2 = CopiarFormula(formula1->f2);
        aux->a1 = CopiarAtomo(formula1->a1);
        if(formula1->not == NEGADO)aux->a1->not = NEGADO;
        aux->COD_OP = formula1->COD_OP;
        aux->not = formula1->not;
        f->f1 = aux;
      }
    }
  }
  else { //formula1 tiene formulas
    if(formula1->f2 == NULL) { //formula1 solo tiene una formula
      if(formula1->a2 == NULL) { //formula1 no tiene atomos
        f->f1 = CopiarFormula(formula1);
      }
      else { //formula1 tiene un atomo(el a2)
        Formula aux = malloc(sizeof(struct FormulaRep));
        memset(aux,0,sizeof(struct FormulaRep));
        aux->f1 = CopiarFormula(formula1->f1);
        aux->a2 = CopiarAtomo(formula1->a2);
        if(formula1->not == NEGADO)aux->a2->not = NEGADO;
        aux->COD_OP = formula1->COD_OP;
        aux->not = formula1->not;
        f->f1 = aux;
      }
    }
    else { //formula1 tiene las dos formulas
      Formula aux = malloc(sizeof(struct FormulaRep));
      memset(aux,0,sizeof(struct FormulaRep));
      aux->f1 = CopiarFormula(formula1->f1);
      aux->f2 = CopiarFormula(formula1->f2);
      aux->COD_OP = formula1->COD_OP;
      aux->not = formula1->not;
      f->f1 = aux;
    }
  }

  //FORMULA 2//
  if(formula2->f1 == NULL) { //formula2 no la primera formula
    if(formula2->f2 == NULL) { //formula2 no tiene ni la primera ni la segunda formula
      if(formula2->a2 == NULL) { //formula2 solo tiene un atomo
        f->a2 = CopiarAtomo(formula2->a1);
        if(formula2->not == NEGADO)f->a2->not = NEGADO;
      }
      else { //formula2 tiene los dos atomos
        Formula aux = malloc(sizeof(struct FormulaRep));
        memset(aux,0,sizeof(struct FormulaRep));
        aux->a1 = CopiarAtomo(formula2->a1);
        aux->a2 = CopiarAtomo(formula2->a2);
        aux->COD_OP = formula2->COD_OP;
        aux->not = formula2->not;
        f->f2 = aux;
      }
    }
    else { //formula2 si tiene la segunda formula
      if(formula2->a1 == NULL) { //formula2 no tiene atomos
        f->f2 = CopiarFormula(formula2);
      }
      else { //formula2 tiene un atomo(el a1)
        Formula aux = malloc(sizeof(struct FormulaRep));
        memset(aux,0,sizeof(struct FormulaRep));
        aux->f2 = CopiarFormula(formula2->f2);
        aux->a1 = CopiarAtomo(formula2->a1);
        if(formula2->not == NEGADO)aux->a1->not = NEGADO;
        aux->COD_OP = formula2->COD_OP;
        aux->not = formula2->not;
        f->f2 = aux;
      }
    }
  }
  else { //formula2 tiene formulas
    if(formula2->f2 == NULL) { //formula2 solo tiene una formula
      if(formula2->a2 == NULL) { //formula2 no tiene atomos
        f->f2 = CopiarFormula(formula2);
      }
      else { //formula2 tiene un atomo
        Formula aux = malloc(sizeof(struct FormulaRep));
        memset(aux,0,sizeof(struct FormulaRep));
        aux->f1 = CopiarFormula(formula2->f1);
        aux->a2 = CopiarAtomo(formula2->a2);
        if(formula2->not == NEGADO)aux->a2->not = NEGADO;
        aux->COD_OP = formula2->COD_OP;
        aux->not = formula2->not;
        f->f2 = aux;
      }
    }
    else { //formula2 tiene las dos formulas
      f->f2 = CopiarFormula(formula2);
    }
  }
  f->COD_OP = operador;
  LiberarFormula(formula1);
  LiberarFormula(formula2);
	return f;
}

//Concatena las dos formulas
Formula Concatenar(Formula f1,Formula f2) {
  Formula aux = f1;
  while(aux->sig != NULL)aux = aux->sig;
	aux->sig = f2;
	return f1;
}

Tableaux CrearTableaux(Formula inicial) {
  Tableaux t = malloc(sizeof(struct TableauxRep));
  memset(t,0,sizeof(struct TableauxRep));
  t->f = inicial;
  t->ti = NULL;
  t->td = NULL;
  t->etiqueta = VACIO;
  return t;
}

//Funciones IMPRIMIR
//Funciones ascii para devolver un char* en vez de imprimir directamente
//Usadas por el ascii_tree

char* showAtomo_ascii(char* buf, Atomo a) {
  if(a->not == NEGADO)sprintf(buf+strlen(buf),"%s",ASCII_SIMBOLO_NOT);
  sprintf(buf+strlen(buf),"%c",a->id);
  return buf;
}

char* showAtomo_svg(char* buf, Atomo a) {
  if(a->not == NEGADO)sprintf(buf+strlen(buf),"%s",SVG_SIMBOLO_NOT);
  sprintf(buf+strlen(buf),"%c",a->id);
  return buf;
}

//https://www.w3schools.com/charsets/ref_utf_dingbats.asp
//https://www.w3schools.com/charsets/ref_html_entities_r.asp
char* showCOD_ascii(char* buf, int COD_OP) {
  switch(COD_OP) {
    case COD_DIMP:
      sprintf(buf+strlen(buf),"%s",ASCII_SIMBOLO_DIMP);
      break;
    case COD_IMP:
      sprintf(buf+strlen(buf),"%s",ASCII_SIMBOLO_IMP);
      break;
    case COD_AND:
      sprintf(buf+strlen(buf),"%s",ASCII_SIMBOLO_AND);
      break;
    case COD_OR:
      sprintf(buf+strlen(buf),"%s",ASCII_SIMBOLO_OR);
      break;
  }
  return buf;
}

char* showCOD_svg(char* buf, int COD_OP) {
  switch(COD_OP) {
    case COD_DIMP:
      sprintf(buf+strlen(buf),"%s",SVG_SIMBOLO_DIMP);
      break;
    case COD_IMP:
      sprintf(buf+strlen(buf),"%s",SVG_SIMBOLO_IMP);
      break;
    case COD_AND:
      sprintf(buf+strlen(buf),"%s",SVG_SIMBOLO_AND);
      break;
    case COD_OR:
      sprintf(buf+strlen(buf),"%s",SVG_SIMBOLO_OR);
      break;
  }
  return buf;
}

char* show_ascii(char* buf, Formula f) {
  Formula aux = f;

  if(aux->not == NEGADO)sprintf(buf+strlen(buf),"%s",ASCII_SIMBOLO_NOT);
  sprintf(buf+strlen(buf),"(");
  if(aux->f1 != NULL) { //Existe formula izquierda
    show_ascii(buf,aux->f1);
    showCOD_ascii(buf,aux->COD_OP);
    if(aux->f2 != NULL)show_ascii(buf,aux->f2); //Existe formula derecha
    else if(aux->a2 != NULL)showAtomo_ascii(buf,aux->a2); //No existe formula derecha
  }
  else { //No existe formula izquierda
    if(aux->a1 == NULL)printf("ERROR: a1 es nulo\n");
    showAtomo_ascii(buf,aux->a1);
    showCOD_ascii(buf,aux->COD_OP);
    if(aux->f2 != NULL)show_ascii(buf,aux->f2); //Existe formula derecha
    else if(aux->a2 != NULL)showAtomo_ascii(buf,aux->a2); //No existe formula derecha
  }
  sprintf(buf+strlen(buf),")");

  while(aux->sig != NULL){
		aux = aux->sig;

    if(aux->not == NEGADO)sprintf(buf+strlen(buf),"%s",ASCII_SIMBOLO_NOT);
    sprintf(buf+strlen(buf),"(");
    if(aux->f1 != NULL) { //Existe formula izquierda
      show_ascii(buf,aux->f1);
      showCOD_ascii(buf,aux->COD_OP);
      if(aux->f2 != NULL)show_ascii(buf,aux->f2); //Existe formula derecha
      else if(aux->a2 != NULL)showAtomo_ascii(buf,aux->a2); //No existe formula derecha
    }
    else { //No existe formula izquierda
      if(aux->a1 == NULL)printf("ERROR: a1 es nulo\n");
      showAtomo_ascii(buf,aux->a1);
      showCOD_ascii(buf,aux->COD_OP);
      if(aux->f2 != NULL)show_ascii(buf,aux->f2); //Existe formula derecha
      else if(aux->a2 != NULL)showAtomo_ascii(buf,aux->a2); //No existe formula derecha
    }
    sprintf(buf+strlen(buf),")");
  }

  return buf;
}

char* show_svg(char* buf, Formula f) {
  Formula aux = f;

  if(aux->not == NEGADO)sprintf(buf+strlen(buf),"%s",SVG_SIMBOLO_NOT);
  sprintf(buf+strlen(buf),"(");
  if(aux->f1 != NULL) { //Existe formula izquierda
    show_svg(buf,aux->f1);
    showCOD_svg(buf,aux->COD_OP);
    if(aux->f2 != NULL)show_svg(buf,aux->f2); //Existe formula derecha
    else if(aux->a2 != NULL)showAtomo_svg(buf,aux->a2); //No existe formula derecha
  }
  else { //No existe formula izquierda
    if(aux->a1 == NULL)printf("ERROR: a1 es nulo\n");
    showAtomo_svg(buf,aux->a1);
    showCOD_svg(buf,aux->COD_OP);
    if(aux->f2 != NULL)show_svg(buf,aux->f2); //Existe formula derecha
    else if(aux->a2 != NULL)showAtomo_svg(buf,aux->a2); //No existe formula derecha
  }
  sprintf(buf+strlen(buf),")");

  while(aux->sig != NULL){
		aux = aux->sig;

    if(aux->not == NEGADO)sprintf(buf+strlen(buf),"%s",SVG_SIMBOLO_NOT);
    sprintf(buf+strlen(buf),"(");
    if(aux->f1 != NULL) { //Existe formula izquierda
      show_svg(buf,aux->f1);
      showCOD_svg(buf,aux->COD_OP);
      if(aux->f2 != NULL)show_svg(buf,aux->f2); //Existe formula derecha
      else if(aux->a2 != NULL)showAtomo_svg(buf,aux->a2); //No existe formula derecha
    }
    else { //No existe formula izquierda
      if(aux->a1 == NULL)printf("ERROR: a1 es nulo\n");
      showAtomo_svg(buf,aux->a1);
      showCOD_svg(buf,aux->COD_OP);
      if(aux->f2 != NULL)show_svg(buf,aux->f2); //Existe formula derecha
      else if(aux->a2 != NULL)showAtomo_svg(buf,aux->a2); //No existe formula derecha
    }
    sprintf(buf+strlen(buf),")");
  }

  return buf;
}

//FIN Funciones IMPRIMIR

//Funcion recursiva que devuelve el 'tree' correspondiende al tableaux 't'
Tree *CrearArbolDesdeTableaux(Tree *tree, Tableaux t) {
  char *buffer = malloc(sizeof(char)*MAX_CHAR);
  memset(buffer,0,sizeof(char)*MAX_CHAR);
  tree = malloc (sizeof (struct Tree));
  memset(tree,0,sizeof (struct Tree));
  tree->element = show_ascii(buffer,t->f);
  if(t->etiqueta != VACIO) {
    if(t->etiqueta == CERRADO)tree->color = COLOR_RED;
    else tree->color = COLOR_GREEN;
  }
  if(t->ti != NULL)tree->left = CrearArbolDesdeTableaux(tree->left,t->ti);
  if(t->td != NULL)tree->right = CrearArbolDesdeTableaux(tree->right,t->td);
  return tree;
}

int TableauxCerrado(Tableaux t) {
  int tmp;
  if(t->etiqueta == ABIERTO)return BOOLEAN_FALSE;

  if(t->ti != NULL) {
    tmp = TableauxCerrado(t->ti);
    if(tmp == BOOLEAN_FALSE)return BOOLEAN_FALSE;
  }
  if(t->td != NULL) {
    tmp = TableauxCerrado(t->td);
    if(tmp == BOOLEAN_FALSE)return BOOLEAN_FALSE;
  }
  return BOOLEAN_TRUE;
}

//Imprime el Tableaux en forma de arbol
void showTableauxTree(Tableaux t) {
  Tree *arbol = CrearArbolDesdeTableaux(arbol,t);
  print_ascii_tree(arbol);
  freeTree(arbol);
}

//Devuelve la formula o atomo que haya en la izquierda de 'f'
Formula ExtraerIzquierda(Formula f) {
  if(f->f1 == NULL)return CrearFormula(f->a1);
  return CopiarFormula(f->f1);
}

//Devuelve la formula o atomo que haya en la derecha de 'f'
Formula ExtraerDerecha(Formula f) {
  if(f->f2 == NULL)return CrearFormula(f->a2);
  return CopiarFormula(f->f2);
}

//Busca la oracion 'aux' en el tableaux 't' que esta a 'busqueda' iteraciones de distancia
Formula BuscarOracion(int busqueda, Formula aux, Tableaux t) {
  aux = t->f;
  for(int i=0;i<busqueda-1;i++)aux = aux->sig;
  return aux;
}

//True si en 'f' existe el negado de 'a', false si no existe
int ContieneSuNegado(Atomo a, Formula f) {
  Formula aux = f;
  Atomo tmp;
  while(aux->sig != NULL) {
    tmp = ExtraerAtomo(aux);
    if(tmp != NULL && tmp->id == a->id && tmp->not != a->not)return BOOLEAN_TRUE;
    aux = aux->sig;
  }
  tmp = ExtraerAtomo(aux);
  if(tmp != NULL && tmp->id == a->id && tmp->not != a->not)return BOOLEAN_TRUE;
  else return BOOLEAN_FALSE;
}

//True si existe un atomo y su negado, False si no
int ContieneContradiccion(Formula f) {
  Formula aux = f;
  Atomo tmp;
  while(aux->sig != NULL) {
    tmp = ExtraerAtomo(aux);
    //Si no es nulo, extraer devolvio el unico atomo
    if(tmp != NULL && ContieneSuNegado(tmp,f))return BOOLEAN_TRUE;
    aux = aux->sig;
  }
  tmp = ExtraerAtomo(aux);
  if(tmp != NULL && ContieneSuNegado(tmp,f))return BOOLEAN_TRUE;
  else return BOOLEAN_FALSE;
}

//Funciones para resolver alfa formulas
//(P ^ Q)
void and(Tableaux t,int busqueda);
//~(P V Q)
void orNegado(Tableaux t,int busqueda);
//~(P -> Q)
void impNegado(Tableaux t,int busqueda);
//(P <-> Q)
void dobleImp(Tableaux t,int busqueda);
//FIN_Funciones para resolver alfa formulas

//Funciones para resolver beta formulas
//~(P ^ Q)
void andNegado(Tableaux t,int busqueda);
//(P V Q)
void or(Tableaux t,int busqueda);
//(P -> Q)
void imp(Tableaux t,int busqueda);
//~(P <-> Q)
void dobleImpNegado(Tableaux t,int busqueda);
//FIN_Funciones para resolver beta formulas

//Resuelve el tableaux 't'
void Resolver(Tableaux t) {
  int busqueda = 0;
  Formula oracion = t->f;

  //Busqueda de lo siguiente que sea necesario resolver, intentado que sea alfa formula
  while(oracion->sig != NULL && (SoloTieneUnAtomo(oracion) || !EsAlfaFormula(oracion))) {
    busqueda++;
    oracion = oracion->sig;
  }

  //No hemos encontrado una alfa formula para resolver; buscar una beta formula
  if(!EsAlfaFormula(oracion)) {
    oracion = t->f;
    busqueda = 0;
    while(oracion->sig != NULL && SoloTieneUnAtomo(oracion)) {
      busqueda++;
      oracion = oracion->sig;
    }
  }

  //Si ya esta todo resuelto en esta rama o hay una contradiccion, terminar
  //y marcar el tableaux como corresponda(ABIERTO o CERRADO)
  if(oracion->sig == NULL && SoloTieneUnAtomo(oracion)) {
    if (ContieneContradiccion(t->f))t->etiqueta = CERRADO;
    else t->etiqueta = ABIERTO;
    return;
  }
  if (ContieneContradiccion(t->f)) {
    t->etiqueta = CERRADO;
    return;
  }
  //Si no lo esta, ramificar
  else {
    switch(oracion->COD_OP) {
      case COD_DIMP:
        if(oracion->not == NEGADO) dobleImpNegado(t,busqueda);
        else dobleImp(t,busqueda);
        break;

      case COD_AND:
        if(oracion->not == NEGADO) andNegado(t,busqueda);
        else and(t,busqueda);
        break;

      case COD_IMP:
        if(oracion->not == NEGADO) impNegado(t,busqueda);
        else imp(t,busqueda);
        break;

      case COD_OR:
        if(oracion->not == NEGADO) orNegado(t,busqueda);
        else or(t,busqueda);
        break;
    }
  }
}

//Funciones para resolver alfa formulas
void and(Tableaux t,int busqueda) {
  Formula aux;
  Formula izquierda;
  Formula derecha;

  t->ti = CrearTableaux(CopiarFormula(t->f));
  if(busqueda == 0) {
    izquierda = ExtraerIzquierda(t->ti->f);
    derecha = ExtraerDerecha(t->ti->f);
    derecha->sig = t->ti->f->sig;
    izquierda->sig = derecha;
    LiberarFormula(t->ti->f);
    t->ti->f = izquierda;
  }
  else {
    aux = BuscarOracion(busqueda,aux,t->ti);
    izquierda = ExtraerIzquierda(aux->sig);
    derecha = ExtraerDerecha(aux->sig);
    derecha->sig = aux->sig->sig;
    izquierda->sig = derecha;
    LiberarFormula(aux->sig);
    aux->sig = izquierda;
  }
  Resolver(t->ti);
}

void orNegado(Tableaux t,int busqueda) {
  Formula aux;
  Formula izquierda;
  Formula derecha;

  t->ti = CrearTableaux(CopiarFormula(t->f));
  if(busqueda == 0) {
    izquierda = NegarFormula(ExtraerIzquierda(t->ti->f)); //La unica diferencia respecto al AND
    derecha = NegarFormula(ExtraerDerecha(t->ti->f)); //La unica diferencia respecto al AND
    derecha->sig = t->ti->f->sig;
    izquierda->sig = derecha;
    LiberarFormula(t->ti->f);
    t->ti->f = izquierda;
  }
  else {
    aux = BuscarOracion(busqueda,aux,t->ti);
    izquierda = NegarFormula(ExtraerIzquierda(aux->sig)); //La unica diferencia respecto al AND
    derecha = NegarFormula(ExtraerDerecha(aux->sig)); //La unica diferencia respecto al AND
    derecha->sig = aux->sig->sig;
    izquierda->sig = derecha;
    LiberarFormula(aux->sig);
    aux->sig = izquierda;
  }
  Resolver(t->ti);
}

void impNegado(Tableaux t,int busqueda) {
  Formula aux;
  Formula izquierda;
  Formula derecha;

  t->ti = CrearTableaux(CopiarFormula(t->f));
  if(busqueda == 0) {
    izquierda = ExtraerIzquierda(t->ti->f);
    derecha = NegarFormula(ExtraerDerecha(t->ti->f)); //La unica diferencia respecto al AND
    derecha->sig = t->ti->f->sig;
    izquierda->sig = derecha;
    LiberarFormula(t->ti->f);
    t->ti->f = izquierda;
  }
  else {
    aux = BuscarOracion(busqueda,aux,t->ti);
    izquierda = ExtraerIzquierda(aux->sig);
    derecha = NegarFormula(ExtraerDerecha(aux->sig)); //La unica diferencia respecto al AND
    derecha->sig = aux->sig->sig;
    izquierda->sig = derecha;
    LiberarFormula(aux->sig);
    aux->sig = izquierda;
  }
  Resolver(t->ti);
}

void dobleImp(Tableaux t,int busqueda) {
  Formula aux;
  Formula izquierda;
  Formula derecha;

  t->ti = CrearTableaux(CopiarFormula(t->f));
  if(busqueda == 0) {
    izquierda = Unir(ExtraerIzquierda(t->ti->f), COD_IMP, ExtraerDerecha(t->ti->f));
    derecha = Unir(ExtraerDerecha(t->ti->f), COD_IMP, ExtraerIzquierda(t->ti->f));
    derecha->sig = t->ti->f->sig;
    izquierda->sig = derecha;
    LiberarFormula(t->ti->f);
    t->ti->f = izquierda;
  }
  else {
    aux = BuscarOracion(busqueda,aux,t->ti);
    izquierda = Unir(ExtraerIzquierda(aux->sig), COD_IMP, ExtraerDerecha(aux->sig));
    derecha = Unir(ExtraerDerecha(aux->sig), COD_IMP, ExtraerIzquierda(aux->sig));
    derecha->sig = aux->sig->sig;
    izquierda->sig = derecha;
    LiberarFormula(aux->sig);
    aux->sig = izquierda;
  }
  Resolver(t->ti);
}
//FIN_Funciones para resolver alfa formulas

//Funciones para resolver beta formulas
void andNegado(Tableaux t,int busqueda) {
  Formula aux;
  Formula izquierda;
  Formula derecha;

  t->ti = CrearTableaux(CopiarFormula(t->f));
  if(busqueda == 0) {
    izquierda = NegarFormula(ExtraerIzquierda(t->ti->f)); //Unica diferencia con el OR
    izquierda->sig = t->ti->f->sig;
    LiberarFormula(t->ti->f);
    t->ti->f = izquierda;
  }
  else {
    aux = BuscarOracion(busqueda,aux,t->ti);
    izquierda = NegarFormula(ExtraerIzquierda(aux->sig)); //Unica diferencia con el OR
    izquierda->sig = aux->sig->sig;
    LiberarFormula(aux->sig);
    aux->sig = izquierda;
  }
  Resolver(t->ti);

  t->td = CrearTableaux(CopiarFormula(t->f));
  if(busqueda == 0) {
    derecha = NegarFormula(ExtraerDerecha(t->td->f)); //Unica diferencia con el OR
    derecha->sig = t->td->f->sig;
    LiberarFormula(t->td->f);
    t->td->f = derecha;
  }
  else {
    aux = BuscarOracion(busqueda,aux,t->td);
    derecha = NegarFormula(ExtraerDerecha(aux->sig)); //Unica diferencia con el OR
    derecha->sig = aux->sig->sig;
    LiberarFormula(aux->sig);
    aux->sig = derecha;
  }
  Resolver(t->td);
}

void or(Tableaux t,int busqueda) {
  Formula aux;
  Formula izquierda;
  Formula derecha;

  t->ti = CrearTableaux(CopiarFormula(t->f));
  if(busqueda == 0) {
    izquierda = ExtraerIzquierda(t->ti->f);
    izquierda->sig = t->ti->f->sig;
    LiberarFormula(t->ti->f);
    t->ti->f = izquierda;
  }
  else {
    aux = BuscarOracion(busqueda,aux,t->ti);
    izquierda = ExtraerIzquierda(aux->sig);
    izquierda->sig = aux->sig->sig;
    LiberarFormula(aux->sig);
    aux->sig = izquierda;
  }
  Resolver(t->ti);

  t->td = CrearTableaux(CopiarFormula(t->f));
  if(busqueda == 0) {
    derecha = ExtraerDerecha(t->td->f);
    derecha->sig = t->td->f->sig;
    LiberarFormula(t->td->f);
    t->td->f = derecha;
  }
  else {
    aux = BuscarOracion(busqueda,aux,t->td);
    derecha = ExtraerDerecha(aux->sig);
    derecha->sig = aux->sig->sig;
    LiberarFormula(aux->sig);
    aux->sig = derecha;
  }
  Resolver(t->td);
}

void imp(Tableaux t,int busqueda) {
  Formula aux;
  Formula izquierda;
  Formula derecha;

  t->ti = CrearTableaux(CopiarFormula(t->f));
  if(busqueda == 0) {
    izquierda = ExtraerIzquierda(t->ti->f);
    izquierda = NegarFormula(izquierda);
    izquierda->sig = t->ti->f->sig;
    LiberarFormula(t->ti->f);
    t->ti->f = izquierda;
  }
  else {
    aux = BuscarOracion(busqueda,aux,t->ti);
    izquierda = ExtraerIzquierda(aux->sig);
    izquierda = NegarFormula(izquierda);
    izquierda->sig = aux->sig->sig;
    LiberarFormula(aux->sig);
    aux->sig = izquierda;
  }
  Resolver(t->ti);

  t->td = CrearTableaux(CopiarFormula(t->f));
  if(busqueda == 0) {
    derecha = ExtraerDerecha(t->td->f);
    derecha->sig = t->td->f->sig;
    LiberarFormula(t->td->f);
    t->td->f = derecha;
  }
  else {
    aux = BuscarOracion(busqueda,aux,t->td);
    derecha = ExtraerDerecha(aux->sig);
    derecha->sig = aux->sig->sig;
    LiberarFormula(aux->sig);
    aux->sig = derecha;
  }
  Resolver(t->td);
}

void dobleImpNegado(Tableaux t,int busqueda) {
  Formula aux;
  Formula izquierda;
  Formula derecha;

  t->ti = CrearTableaux(CopiarFormula(t->f));
  if(busqueda == 0) {
    izquierda = Unir(ExtraerIzquierda(t->ti->f), COD_IMP, ExtraerDerecha(t->ti->f));
    izquierda->sig = t->ti->f->sig;
    LiberarFormula(t->ti->f);
    t->ti->f = izquierda;
    t->ti->f->not = NEGADO;
  }
  else {
    aux = BuscarOracion(busqueda,aux,t->ti);
    izquierda = Unir(ExtraerIzquierda(aux->sig), COD_IMP, ExtraerDerecha(aux->sig));
    izquierda->sig = aux->sig->sig;
    izquierda->not = NEGADO;
    LiberarFormula(aux->sig);
    aux->sig = izquierda;
  }
  Resolver(t->ti);

  t->td = CrearTableaux(CopiarFormula(t->f));
  if(busqueda == 0) {
    izquierda = Unir(ExtraerDerecha(t->td->f), COD_IMP, ExtraerIzquierda(t->td->f));
    izquierda->sig = t->td->f->sig;
    LiberarFormula(t->td->f);
    t->td->f = izquierda;
    t->td->f->not = NEGADO;
  }
  else {
    aux = BuscarOracion(busqueda,aux,t->td);
    izquierda = Unir(ExtraerDerecha(aux->sig), COD_IMP, ExtraerIzquierda(aux->sig));
    izquierda->sig = aux->sig->sig;
    izquierda->not = NEGADO;
    LiberarFormula(aux->sig);
    aux->sig = izquierda;
  }
  Resolver(t->td);
}
//FIN_Funciones para resolver beta formulas

//Declaracion de variables globales para tamano de los arrays
int MAX_CHAR;
int MAX_NIVELES;

int LongitudCaracteres(FILE *fichero) {
  int c = 0;
  int count = 0;

  do {
    c = fgetc(fichero);
    count++;
  } while(c != EOF && c != '\n');

  return count;
}

TableauxInfo CrearTableauxInfo() {
  TableauxInfo tinf = malloc(sizeof(struct TableauxInfo));
  memset(tinf,0,sizeof(struct TableauxInfo));
  return tinf;
}

void LiberarTableauxInfo(TableauxInfo tinf) {
  free(tinf);
}

void CalcularTableauxInfoRecursivo(Tableaux t, TableauxInfo tinf, int nivel) {
  tinf->nodos++;
  if(nivel > tinf->niveles)tinf->niveles = nivel;
  if(t->ti != NULL)CalcularTableauxInfoRecursivo(t->ti,tinf,nivel+1);
  if(t->td != NULL)CalcularTableauxInfoRecursivo(t->td,tinf,nivel+1);
}

TableauxInfo CalcularTableauxInfo(Tableaux t) {
  TableauxInfo tinf = CrearTableauxInfo();
  tinf->nodos = 1;
  if(t->ti != NULL)CalcularTableauxInfoRecursivo(t->ti,tinf,2);
  if(t->td != NULL)CalcularTableauxInfoRecursivo(t->td,tinf,2);
  return tinf;
}

int lineas(FILE *fich) {
  int ch = 0;
  int count = 0;
  int charsOnCurrentLine = 0;

  while ((ch = fgetc(fich)) != EOF) {
    if (ch == '\n') {
        count++;
        charsOnCurrentLine = 0;
    } else {
        charsOnCurrentLine++;
    }
  }
  if (charsOnCurrentLine > 0)count++;
  rewind(fich);
  return count;
}

void ResolverTableaux(Formula oracion, FILE* fichero) {
  //Dejar el puntero de fichero en la ultima linea
  rewind(fichero);
  int i = 0;
  int nlineas = lineas(fichero);
  while(i < nlineas-1) {
    if(fgetc(fichero) == '\n')i++;
  }

  //Calcular informacion y resolver
  MAX_CHAR = LongitudCaracteres(fichero)*4;
  Tableaux t = CrearTableaux(oracion);
  Resolver(t);
  TableauxInfo tinf = CalcularTableauxInfo(t);
  MAX_NIVELES = tinf->niveles;

  //Comprobar que el arbol va a caber en la terminal
  //Si se ha especificado, aumentar MAX_HEIGHT en Tree.h(hacer como con MAX_CHAR)
  int ret = mostrarTree();
  if(ret == -1) { //El usuario no ha especificado el campo
    if(tinf->nodos > DIRTY_OUTPUT_NODES) printMsg(MESSAGE_ARBOL_DEMASIADO_GRANDE);
    else {
      printMsg(MESSAGE_SOLUCION);
      showTableauxTree(t);
      printf("\n\n");
    }
  }
  else { //El usuario ha especificado el campo
    if(ret == BOOLEAN_TRUE) {
      printMsg(MESSAGE_SOLUCION);
      showTableauxTree(t);
      printf("\n\n");
    }
  }
  printf("\n");

  if(TableauxCerrado(t)) {
    printMsgRed(MESSAGE_TABLEAUX_CERRADO);
    printMsg(MESSAGE_TABLEAUX_INSTATISFACIBLE);
  }
  else {
    printMsgGreen(MESSAGE_TABLEAUX_ABIERTO);
    printMsg(MESSAGE_TABLEAUX_STATISFACIBLE);
  }

  FILE *fich = fopen(NOMBRE_ARCHIVO,"w+");
  if (fich==NULL) {
		printMsgRed(MESSAGE_ABRIR_ARCHIVO_FALLIDO,NOMBRE_ARCHIVO);
		printMsgRed(MESSAGE_NO_SVG);
	} else {
		showTableauxSVG(t,fich,tinf->nodos);
    fclose(fich);
	}

  LiberarTableaux(t);
  LiberarTableauxInfo(tinf);
}
