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

Formula CrearFormula(char atomo) {
	Formula f = malloc(sizeof(struct FormulaRep));
	f->a1 = atomo;
	f->COD_OP = COD_INVALIDO;
	return f;
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

void show(Formula f) {
	printf("##SHOW##\n");
	Formula aux = f;
	printf("[%c",aux->a1);
	switch(aux->COD_OP) {
		case COD_DIMP:
			printf("<->");
			break;
		case COD_IMP:
			printf("->");
			break;	
		case COD_NOT:
			printf("~");
			break;
		case COD_AND:
			printf("^");
			break;
		case COD_OR:
			printf("v");
			break;
	}
	printf("%c]",aux->a2);
	while(aux->sig != NULL){
		aux = aux->sig;
		printf("[%c",aux->a1);
	switch(aux->COD_OP) {
		case COD_DIMP:
			printf("<->");
			break;
		case COD_IMP:
			printf("->");
			break;	
		case COD_NOT:
			printf("~");
			break;
		case COD_AND:
			printf("^");
			break;
		case COD_OR:
			printf("v");
			break;
	}
	printf("%c]",aux->a2);
	}
	printf("\n");
}

probar solo un atomo
