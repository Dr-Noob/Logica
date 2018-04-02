#include <stdlib.h>
#include <string.h>
#include "../common/basename.h"

//https://msdn.microsoft.com/en-us/library/e737s6tf%28v=VS.100%29.aspx
char *getBasename(char *path) {
  char fname[_MAX_FNAME];
  _splitpath( path, NULL, NULL, fname, NULL);
  int len = strlen(fname);
  strncpy(path,fname,len);
  path[len] = '\0';
  return path;
}
