#include <libgen.h>
#include <string.h>
#include "../common/basename.h"

char *getBasename(char *path) {
  char* ret = basename(path);

  //Eliminar extension si tiene
  char* final = strstr(ret,".");
  if(final != NULL)
    *final = '\0';

  return ret;
}
