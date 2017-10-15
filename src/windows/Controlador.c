#include "Controlador.h"

void ResolverTableaux(Formula oracion, FILE* fichero) {
  rewind(fichero);
  MAX_CHAR = LongitudCaracteres(fichero)*4;
  Tableaux t = CrearTableaux(oracion);
  Resolver(t);
  TableauxInfo tinf = CalcularTableauxInfo(t);
  int nodos = nNodos(tinf);
  MAX_NIVELES = nNiveles(tinf);

  //Comprobar que el arbol va a caber en la terminal
  //Si se ha especificado, aumentar MAX_HEIGHT en Tree.h(hacer como con MAX_CHAR)
  if(nodos > DIRTY_OUTPUT_NODES) printMsg(MESSAGE_ARBOL_DEMASIADO_GRANDE);
  else {
    printMsg(MESSAGE_SOLUCION);
    showTableauxTree(t);
    printf("\n\n");
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

  FILE *fich = fopen(NOMBRE_DEFECTO_SVG,"w+");
  if (fich==NULL) {
		printMsgRed(MESSAGE_ABRIR_ARCHIVO_FALLIDO,NOMBRE_DEFECTO_SVG);
		printMsgRed(MESSAGE_NO_SVG);
	} else {
		showTableauxSVG(t,fich,nodos);
    fclose(fich);
	}

  LiberarTableaux(t);
  LiberarTableauxInfo(tinf);
}
