#ifndef __Formula__
#define __Formula__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Global.h"
#include "Tree.h"

#define VACIO         2
#define CERRADO       3
#define ABIERTO       4

#define COD_INVALIDO  5
#define COD_DIMP      6
#define COD_IMP       7
#define COD_AND       8
#define COD_OR        9

#define NEGADO        10
#define SIN_NEGAR     11

struct AtomoRep;
typedef struct AtomoRep *Atomo;

struct FormulaRep;
typedef struct FormulaRep *Formula;

struct TableauxRep;
typedef struct TableauxRep *Tableaux;

struct TableauxInfo;
typedef struct TableauxInfo *TableauxInfo;

struct TableauxRep {
  Formula f;
  Tableaux ti; //Tableaux izquierdo
  Tableaux td; //Tableaux derecho
  int etiqueta; //VACIO, CERRADO, ABIERTO
};

Formula CrearFormula(Atomo a);
Atomo CrearAtomo(char _id,int _not);
TableauxInfo CrearTableauxInfo();
Tableaux CrearTableaux(Formula inicial);
TableauxInfo CrearTableauxInfo(Tableaux t);

Formula NegarFormula(Formula f);
Formula Unir(Formula f1,int operador,Formula f2);
Formula Concatenar(Formula f1,Formula f2);
char* show_svg(char* buf, Formula f);
int nNodos(TableauxInfo tinf);
int nNiveles(TableauxInfo tinf);
int TableauxCerrado(Tableaux t);
void Resolver(Tableaux t);
void showTableauxTree(Tableaux t);

void LiberarTableauxInfo(TableauxInfo tinf);
void LiberarTableaux(Tableaux t);
void LiberarAtomo(Atomo a);

#endif
