#include "Config.h"
#include "Controlador.h"

void ResolverTableaux(Formula oracion, FILE* fichero) {
  //Dejar el puntero de fichero en la ultima linea
  rewind(fichero);
  int i = 0;
  int ret = 0;
  FILE *fich_svg;
  int nlineas = lineas(fichero);
  while(i < nlineas-1) {
    if(fgetc(fichero) == '\n')i++;
  }

  //Calcular informacion y resolver
  MAX_CHAR = LongitudCaracteres(fichero)*4;
  Tableaux t = CrearTableaux(oracion);
  Resolver(t);
  TableauxInfo tinf = CalcularTableauxInfo(t);
  int nodos = nNodos(tinf);
  MAX_NIVELES = nNiveles(tinf);

  //Comprobar que el arbol va a caber en la terminal
  //Si se ha especificado, aumentar MAX_HEIGHT en Tree.h(hacer como con MAX_CHAR)
  ret = mostrarTree();
  if(ret == BOOLEAN_FALSE) { //El usuario no ha especificado el campo
    if(nodos > DIRTY_OUTPUT_NODES) printMsg(MESSAGE_ARBOL_DEMASIADO_GRANDE);
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

	ret = mostrarSVG();
	if(ret == -1 || ret == BOOLEAN_TRUE) {
		char* nombre_archivo = nombreSVG();
		fich_svg = fopen(nombre_archivo,"w+");
		if (fich_svg==NULL) {
			printMsgRed(MESSAGE_ABRIR_ARCHIVO_FALLIDO,nombre_archivo);
			printMsgRed(MESSAGE_NO_SVG);
			perror(NULL);
		} else {
			showTableauxSVG(t,fich_svg,nodos);
		  fclose(fich_svg);
		}
		free(nombre_archivo);
	}

  LiberarTableaux(t);
  LiberarTableauxInfo(tinf);
}
