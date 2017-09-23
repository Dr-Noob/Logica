#include "Config.h"

struct TablaTokens {
  char* tokens[N_WORDS];
};

struct Match {
  char* id;
  char* string;
};

TablaTokens CrearTablaTokens() {
  TablaTokens t = malloc(sizeof(struct TablaTokens));

  for(int i=0;i<N_WORDS;i++) {
    t->tokens[i] = malloc(sizeof(char)*MAX_CHARS);
    memset(t->tokens[i],0,sizeof(char)*MAX_CHARS);
  }

  return t;
}

void print_TablaTokens(TablaTokens t) {
    printf("and='%s'\n",t->tokens[0]);
    printf("or='%s'\n",t->tokens[1]);
    printf("not='%s'\n",t->tokens[2]);
    printf("imp='%s'\n",t->tokens[3]);
    printf("dimp='%s'\n",t->tokens[4]);
}

void freeTablaTokens(TablaTokens t) {
  if(t == NULL){ printf("Me piden liberar t que es nulo\n"); return; }
  for(int i=0;i<N_WORDS;i++) free(t->tokens[i]);
  free(t);
}

void freeMatch(Match m) {
  if(m == NULL){ printf("Me piden liberar m que es nulo\n"); return; }
  free(m->id);
  free(m->string);
  free(m);
}

Match Groups(Match m,char* source,char* groupText, regmatch_t* pmatch) {
  //Primero ver que no supera los limites
  strcpy(groupText, source);
  groupText[pmatch[2].rm_eo] = 0;

  if(strlen(groupText+pmatch[2].rm_so) > MAX_CHARS) {
    printf("ERROR: Specified a token larger than %d(%s)\n",MAX_CHARS,source);
    freeMatch(m);
    return NULL;
  }

  //Copiar string e id
  strcpy(m->string,groupText+pmatch[2].rm_so);

  strcpy(groupText, source);
  groupText[pmatch[1].rm_eo] = 0;
  strcpy(m->id,groupText+pmatch[1].rm_so);

  return m;
}

int LineaVacia(char* linea) {
    if (linea == NULL || '\n' == linea[0])return BOOLEAN_TRUE;
    else return BOOLEAN_FALSE;
}

Match getMatch(char* source, regex_t regexs[N_WORDS]) {
  char msgbuf[100];
  int MAX_GROUPS = 3;
  int reti = 0;
  int len = strlen(source);
  source[len-1]=0;
  char groupText[len + 1];
  regmatch_t pmatch[MAX_GROUPS];
  Match m = malloc(sizeof(struct Match));
  m->id = malloc(sizeof(char)*MAX_CHARS);
  m->string = malloc(sizeof(char)*MAX_CHARS);
  memset(m->id,0,MAX_CHARS);
  memset(m->string,0,MAX_CHARS);

  /* AND */
  reti = regexec(&regexs[0], source, MAX_GROUPS, pmatch, 0);
  if (!reti) return Groups(m,source,groupText,pmatch);
  else if (reti != REG_NOMATCH) { //regex match failed
    regerror(reti, &regexs[0], msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    freeMatch(m);
    return NULL;
  }

  /* OR */
  reti = regexec(&regexs[1], source, MAX_GROUPS, pmatch, 0);
  if (!reti) return Groups(m,source,groupText,pmatch);
  else if (reti != REG_NOMATCH) {
    regerror(reti, &regexs[1], msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    freeMatch(m);
    return NULL;
  }

  /* NOT */
  reti = regexec(&regexs[2], source, MAX_GROUPS, pmatch, 0);
  if (!reti) return Groups(m,source,groupText,pmatch);
  else if (reti != REG_NOMATCH) {
    regerror(reti, &regexs[2], msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    freeMatch(m);
    return NULL;
  }

  /* IMP */
  reti = regexec(&regexs[3], source, MAX_GROUPS, pmatch, 0);
  if (!reti) return Groups(m,source,groupText,pmatch);
  else if (reti != REG_NOMATCH) {
    regerror(reti, &regexs[3], msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    freeMatch(m);
    return NULL;
  }

  /* DIMP */
  reti = regexec(&regexs[4], source, MAX_GROUPS, pmatch, 0);
  if (!reti) return Groups(m,source,groupText,pmatch);
  else if (reti != REG_NOMATCH) {
    regerror(reti, &regexs[4], msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    freeMatch(m);
    return NULL;
  }

  printf("ERROR: La linea '%s' es incorrecta\n",source);
  freeMatch(m);
  return NULL;
}

void LiberarTodo(regex_t regexs[N_WORDS],char* line,TablaTokens t,Match m) {
  for(int i=0;i<N_WORDS;i++)regfree(&regexs[i]);
  if(line)free(line);
  freeTablaTokens(t);
  freeMatch(m);
}

TablaTokens GenerarTabla(FILE *fich) {
  int ret = 0;

  regex_t regexs[N_WORDS];

  ret = regcomp(&regexs[0], "^(and)[ \t]*=[ \t]*([ -~]+)[ \t]*", REG_EXTENDED);
  if (ret) {
      fprintf(stderr, "Could not compile and regex\n");
      return NULL;
  }

  ret = regcomp(&regexs[1], "^(or)[ \t]*=[ \t]*([ -~]+)[ \t]*", REG_EXTENDED);
  if (ret) {
      fprintf(stderr, "Could not compile or regex\n");
      return NULL;
  }

  ret = regcomp(&regexs[2], "^(not)[ \t]*=[ \t]*([ -~]+)[ \t]*", REG_EXTENDED);
  if (ret) {
      fprintf(stderr, "Could not compile not regex\n");
      return NULL;
  }

  ret = regcomp(&regexs[3], "^(imp)[ \t]*=[ \t]*([ -~]+)[ \t]*", REG_EXTENDED);
  if (ret) {
      fprintf(stderr, "Could not compile imp regex\n");
      return NULL;
  }

  ret = regcomp(&regexs[4], "^(dimp)[ \t]*=[ \t]*([ -~]+)[ \t]*", REG_EXTENDED);
  if (ret) {
      fprintf(stderr, "Could not compile dimp regex\n");
      return NULL;
  }

  TablaTokens t = CrearTablaTokens();
  Match m = NULL;
  char* line = NULL;
  int tokenSet[N_WORDS];
  int i = 0;
  memset(tokenSet,BOOLEAN_FALSE,N_WORDS*sizeof(int));
  size_t len = 0;
  ssize_t read = getline(&line,&len,fich);
  while(read != -1 && LineaVacia(line))read = getline(&line,&len,fich);

  while(read != -1) {
    m = getMatch(line,regexs);

    if(m == NULL) {
      LiberarTodo(regexs,line,t,m);
      return NULL;
    }

    i = 0;
    while(i < N_WORDS) {
      if(strcmp(m->id,WORDS[i]) == 0) {
        if(tokenSet[i]) {
          printf("ERROR: El token '%s' ya habia sido especificado como '%s'\n",WORDS[i],t->tokens[i]);
          LiberarTodo(regexs,line,t,m);
          return NULL;
        }
        tokenSet[i] = BOOLEAN_TRUE;
        strcpy(t->tokens[i],m->string);
        i = N_WORDS+1;
      }
      i++;
    }

    if(i == N_WORDS) {
      printf("ERROR\n");
      LiberarTodo(regexs,line,t,m);
      return NULL;
    }
    /*
    if(strcmp(m->id,WORDS[0]) == 0) {
      if(tokenSet[0]) {
        printf("ERROR: El token '%s' ya habia sido especificado como '%s'\n",WORDS[0],t->and);
        LiberarTodo(regexs,line,t,m);
        return NULL;
      }
      tokenSet[0] = BOOLEAN_TRUE;
      strcpy(t->and,m->string);
    }
    else if(strcmp(m->id,WORDS[1]) == 0) {
      if(tokenSet[1]) {
        printf("ERROR: El token '%s' ya habia sido especificado como '%s'\n",WORDS[1],t->or);
        LiberarTodo(regexs,line,t,m);
        return NULL;
      }
      tokenSet[1] = BOOLEAN_TRUE;
      strcpy(t->or,m->string);
    }
    else if(strcmp(m->id,WORDS[2]) == 0) {
      if(tokenSet[2]) {
        printf("ERROR: El token '%s' ya habia sido especificado como '%s'\n",WORDS[2],t->not);
        LiberarTodo(regexs,line,t,m);
        return NULL;
      }
      tokenSet[2] = BOOLEAN_TRUE;
      strcpy(t->not,m->string);
    }
    else if(strcmp(m->id,WORDS[3]) == 0) {
      if(tokenSet[3]) {
        printf("ERROR: El token '%s' ya habia sido especificado como '%s'\n",WORDS[3],t->imp);
        LiberarTodo(regexs,line,t,m);
        return NULL;
      }
      tokenSet[3] = BOOLEAN_TRUE;
      strcpy(t->imp,m->string);
    }
    else if(strcmp(m->id,WORDS[4]) == 0) {
      if(tokenSet[4]) {
        printf("ERROR: El token '%s' ya habia sido especificado como '%s'\n",WORDS[4],t->dimp);
        LiberarTodo(regexs,line,t,m);
        return NULL;
      }
      tokenSet[4] = BOOLEAN_TRUE;
      strcpy(t->dimp,m->string);
    }
    else {
      printf("ERROR\n");
      LiberarTodo(regexs,line,t,m);
      return NULL;
    }
    */

    freeMatch(m);
    read = getline(&line,&len,fich);
    while(read != -1 && LineaVacia(line))read = getline(&line,&len,fich);
  }

  for(int i=0;i<N_WORDS;i++)regfree(&regexs[i]);
  if(line)free(line);

  for(int i=0;i<N_WORDS;i++) { //Solo devolver la tabla si esta rellenada entera
    if(!tokenSet[i]) {
      printf("ERROR: El token '%s' no ha sido especificado\n",WORDS[i]);
      freeTablaTokens(t);
      return NULL;
    }
  }
  return t;
}
