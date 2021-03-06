#include "Global.h"

int LANG;

void getLang() {
  const char* tmp = setlocale(LC_CTYPE,"");
  if(tmp == NULL) {
    printf("WARNING: Imposible obtener el idioma del sistema\n");
    LANG = ESP_LANG;
  }
  else {
    if (strstr(tmp, ESP_LANG_STR1) != NULL || strstr(tmp, ESP_LANG_STR2) != NULL) {
        LANG = ESP_LANG;
    }
    else LANG = OTHER_LANG;
  }
}

void printMsg(const char *fmt[2], ...) {
  if(LANG == OTHER_LANG)fmt[0]=fmt[1];
  char buffer[4096];
  va_list args;
  va_start(args, fmt[0]);
  vsnprintf(buffer, sizeof(buffer), fmt[0], args);
  va_end(args);
  fprintf(stdout,"%s",buffer);
}

void printMsgRed(const char *fmt[2], ...) {
  if(LANG == OTHER_LANG)fmt[0]=fmt[1];
  char buffer[4096];
  va_list args;
  va_start(args, fmt[0]);
  vsnprintf(buffer, sizeof(buffer), fmt[0], args);
  va_end(args);
  fprintf(stderr,RED "%s" RESET,buffer);
}

void printMsgGreen(const char *fmt[2], ...) {
  if(LANG == OTHER_LANG)fmt[0]=fmt[1];
  char buffer[4096];
  va_list args;
  va_start(args, fmt[0]);
  vsnprintf(buffer, sizeof(buffer), fmt[0], args);
  va_end(args);
  fprintf(stderr,GREEN "%s" RESET,buffer);
}

int LongitudCaracteres(FILE *fichero) {
  int c = 0;
  int count = 0;

  do {
    c = fgetc(fichero);
    count++;
  } while(c != EOF && c != '\n');

  return count;
}

int lineas(FILE *fich) {
  int ch = 0;
  int count = 0;
  int charsOnCurrentLine = 0;

  while ((ch = fgetc(fich)) != EOF) {
    if (ch == '\n') {
        count++;
        charsOnCurrentLine = 0;
    } else {
        charsOnCurrentLine++;
    }
  }
  if (charsOnCurrentLine > 0)count++;
  rewind(fich);
  return count;
}
