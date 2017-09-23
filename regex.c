#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Config.h"

/*
struct Match {
  char* id;
  char* string;
};

typedef struct Match *Match;

Match getMatch(char* source) {
  regex_t regex;
  int reti;
  char msgbuf[100];
  int MAX_GROUPS = 3;
  char groupText[strlen(source) + 1];
  regmatch_t pmatch[MAX_GROUPS];
  Match m = malloc(sizeof(struct Match));
  m->id = malloc(sizeof(char)*100);
  m->string = malloc(sizeof(char)*100);
  memset(m->id,0,100);
  memset(m->string,0,100);


  reti = regcomp(&regex, "(and)=([a-z]+)", REG_EXTENDED);
  if (reti) {
      fprintf(stderr, "Could not compile regex\n");
      return NULL;
  }


  reti = regexec(&regex, source, MAX_GROUPS, pmatch, 0);
  if (!reti) {

    strcpy(groupText, source);
    groupText[pmatch[1].rm_eo] = 0;
    strcpy(m->id,groupText+pmatch[1].rm_so);

    strcpy(groupText, source);
    groupText[pmatch[2].rm_eo] = 0;
    strcpy(m->string,groupText+pmatch[2].rm_so);

  }
  else if (reti == REG_NOMATCH) {
      printf("No match\n");
      return NULL;
  }
  else {
      regerror(reti, &regex, msgbuf, sizeof(msgbuf));
      fprintf(stderr, "Regex match failed: %s\n", msgbuf);
      return NULL;
  }


  regfree(&regex);
  return m;
}

//https://stackoverflow.com/questions/2577193/how-do-you-capture-a-group-with-regex
//https://regex101.com/

int main(int argc, char *argv[]) {
  if(argc != 2) {
    printf("Need 2 args\n");
    return 1;
  }

  Match m = getMatch(argv[1]);
  if(m != NULL) {
    printf("cod=\"%s\",string=\"%s\"\n",m->id,m->string);
    free(m->id);
    free(m->string);
    free(m);
  }
  return 0;
}
*/

int main(int argc, char const *argv[]) {
  if(argc != 2) {
    printf("Need 2 args\n");
    return 1;
  }

  FILE *fich = fopen(argv[1],"r");
	if (fich==NULL) {
		printf("ERROR: El archivo %s no ha podido abrirse\n", argv[1]);
		return 1;
	}

  TablaTokens t = GenerarTabla(fich);
  fclose(fich);
  if(t != NULL) {
    print_TablaTokens(t);
    freeTablaTokens(t);
    return 0;
  }
  return 1;
}
