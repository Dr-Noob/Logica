#ifndef __Formula__
#define __Formula__

#define COD_INVALIDO	0
#define COD_DIMP		1
#define COD_IMP		2
#define COD_NOT		3
#define COD_AND		4
#define COD_OR		5

struct FormulaRep;
typedef struct FormulaRep *Formula;

struct TableauxRep;
typedef struct TableauxRep *Tableaux;

Formula CrearFormula(char atomo);
Formula Unir(Formula f1,int operador,Formula f2);
Formula Concatenar(Formula f1,Formula f2);
void ResolverTableaux(Formula oracion);
void show(Formula f);

#endif
