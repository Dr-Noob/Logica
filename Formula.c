#include "Formula.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct AtomoRep {
  char id;
  int not;
};

struct FormulaRep {
  Atomo a1;	//Atomo1
  Atomo a2;	//Atomo2
  Formula formula; //Formula anidada
  int COD_OP;	//Codigo operador
  struct FormulaRep *sig;
};

struct TableauxRep {
  Formula f;
  Tableaux ti;
  Tableaux td;
};

Formula CrearFormula(Atomo a) {
	Formula f = malloc(sizeof(struct FormulaRep));
	f->a1 = a;
  f->a2 = NULL;
	f->COD_OP = COD_INVALIDO;
  f->sig = NULL;
	return f;
}

Atomo CrearAtomo(char _id,int _not) {
  Atomo a = malloc(sizeof(struct AtomoRep));
  a->id = _id;
  a->not = _not;
  return a;
}

Formula CopiarFormula(Formula original) {
  Formula copia=NULL;
  Formula anterior=NULL;
  Atomo a1;
  Atomo a2;

  while(original != NULL) {
    //Copiar
    Formula temp = malloc(sizeof(struct FormulaRep));
    if(original->a1 != NULL) {
      a1 = CrearAtomo(original->a1->id,original->a1->not);
      temp->a1=a1;
    }
    if(original->a2 != NULL) {
      a2 = CrearAtomo(original->a2->id,original->a2->not);
      temp->a2=a2;
    }
    if(original->formula != NULL)temp->formula = CopiarFormula(original->formula);
    temp->COD_OP=original->COD_OP;

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

Formula Unir(Formula f1,int operador,Formula f2) {
  //TODO: Facil de mejorar
	f1->COD_OP = operador;
	if(f1->a2 == NULL) { //atomo operador atomo[p v q]
    f1->a2 = CrearAtomo(f2->a1->id,f2->a1->not);
  }
  else {  //formula operador atomo[(p v q) v r]
    Formula copia = CopiarFormula(f1);
    f1->a1 = NULL;
    f1->a2 = CrearAtomo(f2->a1->id,f2->a1->not);
    f1->formula = copia;
  }
	return f1;
}

Formula Concatenar(Formula f1,Formula f2) {
	f1->sig = f2;
	return f1;
}

Tableaux CrearTableaux(Formula inicial) {
  Tableaux t = malloc(sizeof(struct TableauxRep));
  t->f = inicial;
  t->ti = NULL;
  t->td = NULL;
  return t;
}

void show(Formula f) {
	Formula aux = f;
  printf("[");
  if(aux->formula != NULL)show(aux->formula);
  if(aux->a1 != NULL) {
    if(aux->a1->not == NEGADO)printf("~");
    printf("%c",aux->a1->id);
  }
  if(aux->a2 != NULL) {
    switch(aux->COD_OP) {
      case COD_DIMP:
        printf(" <-> ");
        if(aux->a2->not == NEGADO)printf("~");
        printf("%c",aux->a2->id);
        break;
      case COD_IMP:
        printf(" -> ");
        if(aux->a2->not == NEGADO)printf("~");
        printf("%c",aux->a2->id);
        break;
      case COD_AND:
        printf(" ^ ");
        if(aux->a2->not == NEGADO)printf("~");
        printf("%c",aux->a2->id);
        break;
      case COD_OR:
        printf(" v ");
        if(aux->a2->not == NEGADO)printf("~");
        printf("%c",aux->a2->id);
        break;
    }
  }
  printf("]");


	while(aux->sig != NULL){
		aux = aux->sig;

    printf("[");
    if(aux->formula != NULL)show(aux->formula);
    if(aux->a1 != NULL) {
      if(aux->a1->not == NEGADO)printf("~");
      printf("%c",aux->a1->id);
    }
    if(aux->a2 != NULL) {
      switch(aux->COD_OP) {
        case COD_DIMP:
          printf(" <-> ");
          if(aux->a2->not == NEGADO)printf("~");
          printf("%c",aux->a2->id);
          break;
        case COD_IMP:
          printf(" -> ");
          if(aux->a2->not == NEGADO)printf("~");
          printf("%c",aux->a2->id);
          break;
        case COD_AND:
          printf(" ^ ");
          if(aux->a2->not == NEGADO)printf("~");
          printf("%c",aux->a2->id);
          break;
        case COD_OR:
          printf(" v ");
          if(aux->a2->not == NEGADO)printf("~");
          printf("%c",aux->a2->id);
          break;
      }
    }
    printf("]");

  }

}

void showTableaux(Tableaux t) {
  show(t->f);
  if(t->ti != NULL)showTableaux(t->ti);
  if(t->td != NULL)showTableaux(t->td);
}

void Resolver(Tableaux t) {
  show(t->f);
  printf("\n");
  t->ti = CrearTableaux(CopiarFormula(t->f));
  t->td = CrearTableaux(CopiarFormula(t->f));
  Formula oracion = t->f;
  while(oracion->sig != NULL && ((oracion->a1 == NULL) || (oracion->a2 == NULL)))oracion = oracion->sig;
  if((oracion->a1 == NULL && oracion->formula == NULL) || oracion->a2 == NULL) { //Añadir a list
    return;
  }
  else {
    switch(oracion->COD_OP) {
      Formula aux;
      Formula* puntero;
      Atomo a1;
      Atomo a2;

      case COD_DIMP:
        //TODO: t->td no se usa (free?)
        aux = t->ti->f;
        while(aux->sig != NULL && (aux->a1->id != oracion->a1->id || (aux->a2 != NULL && aux->a2->id != oracion->a2->id) || aux->COD_OP != oracion->COD_OP))aux = aux->sig;
        aux->COD_OP = COD_IMP;
        Formula dimp = malloc(sizeof(struct FormulaRep));
        a1 = CrearAtomo(aux->a1->id,aux->a1->not);
        a2 = CrearAtomo(aux->a2->id,aux->a2->not);
        dimp->a1 = a2;
        dimp->a2 = a1;
        dimp->COD_OP = COD_IMP;
        dimp->sig = aux->sig;
        aux->sig = dimp;
        Resolver(t->ti);
        break;
      case COD_IMP:
        //TODO: Cuando se iguala a NULL,habra que borrar lo otro
        aux = t->ti->f;
        while(aux->sig != NULL && (aux->a1->id != oracion->a1->id || (aux->a2 != NULL && aux->a2->id != oracion->a2->id) || aux->COD_OP != oracion->COD_OP))aux = aux->sig;
        aux->a2 = NULL;
        aux->a1->not = NEGADO;
        aux->COD_OP = COD_INVALIDO;
        Resolver(t->ti);
        aux = t->td->f;
        while(aux->sig != NULL && (aux->a1->id != oracion->a1->id || (aux->a2 != NULL && aux->a2->id != oracion->a2->id) || aux->COD_OP != oracion->COD_OP))aux = aux->sig;
        aux->a1 = aux->a2;
        aux->a2 = NULL;
        aux->COD_OP = COD_INVALIDO;
        Resolver(t->td);
        break;
      case COD_AND:
        aux = t->ti->f;
        while(aux->sig != NULL && (aux->a1->id != oracion->a1->id || (aux->a2 != NULL && aux->a2->id != oracion->a2->id) || aux->COD_OP != oracion->COD_OP))aux = aux->sig;
        aux->COD_OP = COD_INVALIDO;
        Formula and = malloc(sizeof(struct FormulaRep));
        a2 = CrearAtomo(aux->a2->id,aux->a2->not);
        and->a1 = a2;
        aux->a2 = NULL;
        and->a2 = NULL;
        and->COD_OP = COD_INVALIDO;
        and->sig = aux->sig;
        aux->sig = and;
        Resolver(t->ti);
        break;
      case COD_OR:
        aux = t->ti->f;
        while(aux->sig != NULL && aux->formula == NULL && (aux->a1->id != oracion->a1->id || (aux->a2 != NULL && aux->a2->id != oracion->a2->id) || aux->COD_OP != oracion->COD_OP))aux = aux->sig;
        if(aux->formula != NULL) {
          if(aux->sig == NULL)printf("¿Que deberia hacer?\n");
          if(aux->formula->formula == NULL) {
            aux->a1 = CrearAtomo(aux->formula->a1->id,aux->formula->a1->not);
            aux->a2 = CrearAtomo(aux->formula->a2->id,aux->formula->a2->not);
            aux->COD_OP = aux->formula->COD_OP;
            aux->formula = NULL;
          }
          else {
            printf("Formulas andidadas\n");
            aux->a2 = aux->formula->a2;
            aux->formula = aux->formula->formula;
          }
          Resolver(t->ti);
          aux = t->td->f;
          aux->a1 = aux->a2;
          aux->a2 = NULL;
          aux->formula = NULL;
          aux->COD_OP = COD_INVALIDO;
          Resolver(t->td);
        }
        else {
          aux->a2 = NULL;
          aux->COD_OP = COD_INVALIDO;
          Resolver(t->ti);
          aux = t->td->f;
          while(aux->sig != NULL && (aux->a1->id != oracion->a1->id || (aux->a2 != NULL && aux->a2->id != oracion->a2->id) || aux->COD_OP != oracion->COD_OP))aux = aux->sig;
          aux->a1 = aux->a2;
          aux->a2 = NULL;
          aux->COD_OP = COD_INVALIDO;
          Resolver(t->td);
        }
        break;
    }
  }
}

void ResolverTableaux(Formula oracion) {
  Tableaux t = CrearTableaux(oracion);
  Resolver(t);
}
