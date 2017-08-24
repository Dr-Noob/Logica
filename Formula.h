#ifndef __Formula__
#define __Formula__

#define COD_INVALIDO  0
#define COD_DIMP      1
#define COD_IMP       2
#define COD_AND       3
#define COD_OR        4

#define NEGADO        5
#define SIN_NEGAR     6

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
