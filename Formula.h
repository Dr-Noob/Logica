#ifndef __Formula__
#define __Formula__

#define BOOLEAN_FALSE         0
#define BOOLEAN_TRUE          1

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

Formula CrearFormula(Atomo a);
Atomo CrearAtomo(char _id,int _not);
Formula NegarFormula(Formula f);
Formula Unir(Formula f1,int operador,Formula f2);
Formula Concatenar(Formula f1,Formula f2);
void ResolverTableaux(Formula oracion);
void show(Formula f);

#endif
