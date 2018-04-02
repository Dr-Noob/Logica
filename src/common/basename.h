#ifndef __basename__
#define __basename__

//Necesario para tener llamar a una unica funcion
//y tener compatibilidad con unix y windows
//En unix se llama a basename de libgen.h
//y en windows se llama a _splitpath

char *getBasename(char *path);

#endif
