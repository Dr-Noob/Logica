#include "Formula.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct FormulaRep {
  char a1;	//Atomo1
  char a2;	//Atomo2
  int COD_OP;	//Codigo operador
  struct FormulaRep *sig;
};

struct TableauxRep {
  Formula f;
  struct Tableaux *ti;
  struct Tableaux *td;
};


Formula CrearFormula(char atomo) {
	Formula f = malloc(sizeof(struct FormulaRep));
	f->a1 = atomo;
	f->COD_OP = COD_INVALIDO;
	return f;
}

Formula copy(Formula start1) {
  printf("Copiar \n");
  show(start1);

  Formula start2=NULL;
  Formula previous=NULL;

  while(start1!=NULL) {
    Formula temp = (Formula) malloc (sizeof(struct FormulaRep));
    temp->a1=start1->a1;
    temp->a2=start1->a2;
    temp->COD_OP=start1->COD_OP;
    temp->sig=NULL;

    if(start2==NULL){
        start2=temp;
        previous=temp;
    }
    else {
        previous->sig=temp;
        previous=temp;
    }
    start1=start1->sig;
  }

  printf("A \n");
  show(start2);
  return start2;
}

Tableaux CrearTableaux(Formula inicial) {
  Tableaux t = malloc(sizeof(struct TableauxRep));
  t->f = inicial;
  t->ti->f = NULL;
  t->td->f = NULL;
  return t;
}

Formula Unir(Formula f1,int operador,Formula f2) {
	f1->COD_OP = operador;
	if(f2 != NULL)f1->a2 = f2->a1;
	return f1;
}

Formula Concatenar(Formula f1,Formula f2) {
	f1->sig = f2;
	return f1;
}

void NodoHoja(Formula hoja) {
  printf("Hoja:\n");
  show(hoja);
  printf("\n");
}

void Resolver(Tableaux t) {
  t->ti->f = copy(t->f);
  t->td->f = copy(t->f);
  Formula oracion = t->f;
  while(((oracion->COD_OP == COD_INVALIDO) || (oracion->COD_OP == COD_NOT)) && oracion->sig != NULL)oracion = oracion->sig;
  if(oracion->sig == NULL && ((oracion->COD_OP == COD_INVALIDO) || (oracion->COD_OP == COD_NOT)))NodoHoja(t->f);
  else {
    switch(oracion->COD_OP) {
      Formula f1;
      Formula f2;
      case COD_DIMP:
        oracion->COD_OP = COD_IMP;
        f1 = CrearFormula(oracion->a2);
        f1->a2 = oracion->a1;
        f1->COD_OP = COD_IMP;
        ResolverTableaux(Concatenar(oracion,f1));
        break;
      case COD_IMP:
        f1 = malloc(sizeof(struct FormulaRep));
      	f1->a1 = oracion->a1;
      	f1->COD_OP = COD_NOT;
        f1->sig = t->f->sig;
        ResolverTableaux(f1);
        f2 = malloc(sizeof(struct FormulaRep));
      	f2->a1 = oracion->a2;
      	f2->COD_OP = COD_INVALIDO;
        f2->sig = oracion->sig;
        ResolverTableaux(f2);
        break;
      case COD_AND:
        oracion->COD_OP = COD_INVALIDO;
        f1 = CrearFormula(oracion->a2);
        oracion->a2 = 0;
        ResolverTableaux(Concatenar(oracion,f1));
        break;
      case COD_OR:
        f1 = malloc(sizeof(struct FormulaRep));
        f1->a1 = oracion->a1;
        f1->COD_OP = COD_INVALIDO;
        f1->sig = oracion->sig;
        ResolverTableaux(f1);
        f2 = malloc(sizeof(struct FormulaRep));
        f2->a1 = oracion->a2;
        f2->COD_OP = COD_INVALIDO;
        f2->sig = oracion->sig;
        ResolverTableaux(f2);
        break;
    }
  }
}

void ResolverTableaux(Formula oracion) {
  Tableaux t = CrearTableaux(oracion);
  Resolver(t);
}

void show(Formula f) {
	printf("##SHOW##\n");

	Formula aux = f;
  printf("[");
  if(aux->COD_OP == COD_NOT)printf("~");
  printf("%c",aux->a1);
  switch(aux->COD_OP) {
    case COD_NOT:
    case COD_INVALIDO:
      break;
    case COD_DIMP:
      printf(" <-> %c",aux->a2);
      break;
    case COD_IMP:
      printf(" -> %c",aux->a2);
      break;
    case COD_AND:
      printf(" ^ %c",aux->a2);
      break;
    case COD_OR:
      printf(" v %c",aux->a2);
      break;
  }
  printf("]");

	while(aux->sig != NULL){
		aux = aux->sig;
    printf("[");
    if(aux->COD_OP == COD_NOT)printf("~");
    printf("%c",aux->a1);
    switch(aux->COD_OP) {
      case COD_NOT:
      case COD_INVALIDO:
        break;
      case COD_DIMP:
        printf(" <-> %c",aux->a2);
        break;
      case COD_IMP:
        printf(" -> %c",aux->a2);
        break;
      case COD_AND:
        printf(" ^ %c",aux->a2);
        break;
      case COD_OR:
        printf(" v %c",aux->a2);
        break;
    }
    printf("]");
  }
	printf("\n");
}
