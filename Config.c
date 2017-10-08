#include "Config.h"

TablaTokens t;
int nlineas;

struct TablaTokens {
  char* tokens[N_WORDS];
  char* options[N_OPTIONS];
  int status;
};

struct Match {
  char* id;
  char* string;
};

TablaTokens CrearTablaTokens() {
  t = malloc(sizeof(struct TablaTokens));

  for(int i=0;i<N_WORDS;i++) {
    t->tokens[i] = malloc(sizeof(char)*MAX_CHARS);
    memset(t->tokens[i],0,sizeof(char)*MAX_CHARS);
  }
  for(int i=0;i<N_OPTIONS;i++) {
    t->options[i] = malloc(sizeof(char)*MAX_CHARS);
    memset(t->options[i],0,sizeof(char)*MAX_CHARS);
  }
  t->status = STATUS_CORRECTO;
  return t;
}

void print_TablaTokens() {
    printf("and='%s'\n",t->tokens[0]);
    printf("or='%s'\n",t->tokens[1]);
    printf("not='%s'\n",t->tokens[2]);
    printf("imp='%s'\n",t->tokens[3]);
    printf("dimp='%s'\n",t->tokens[4]);
    printf("svg='%s'\n",t->options[0]);
    printf("stdout='%s'\n",t->options[1]);
    printf("svg_name='%s'\n",t->options[2]);
}

void freeTablaTokens() {
  //if(t == NULL){ printf("Me piden liberar t que es nulo\n"); return; }
  for(int i=0;i<N_WORDS;i++) free(t->tokens[i]);
  for(int i=0;i<N_OPTIONS;i++) free(t->options[i]);
  free(t);
}

void freeMatch(Match m) {
  //if(m == NULL){ printf("Me piden liberar m que es nulo\n"); return; }
  free(m->id);
  free(m->string);
  free(m);
}

Match Groups(Match m,char* source,char* groupText, regmatch_t* pmatch) {
  //Primero ver que no supera los limites
  strcpy(groupText, source);
  groupText[pmatch[2].rm_eo] = 0;

  if(strlen(groupText+pmatch[2].rm_so) > MAX_CHARS) {
    printMsgRed(MESSAGE_TOKEN_DEMASIADO_LARGO,MAX_CHARS,source);
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

Match getMatch(char* source, regex_t regexs[N_WORDS], regex_t options[N_WORDS]) {
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
    printMsgRed(MESSAGE_REGEX_MATCH_FALLIDO,msgbuf);
    freeMatch(m);
    return NULL;
  }

  /* OR */
  reti = regexec(&regexs[1], source, MAX_GROUPS, pmatch, 0);
  if (!reti) return Groups(m,source,groupText,pmatch);
  else if (reti != REG_NOMATCH) {
    regerror(reti, &regexs[1], msgbuf, sizeof(msgbuf));
    printMsgRed(MESSAGE_REGEX_MATCH_FALLIDO,msgbuf);
    freeMatch(m);
    return NULL;
  }

  /* NOT */
  reti = regexec(&regexs[2], source, MAX_GROUPS, pmatch, 0);
  if (!reti) return Groups(m,source,groupText,pmatch);
  else if (reti != REG_NOMATCH) {
    regerror(reti, &regexs[2], msgbuf, sizeof(msgbuf));
    printMsgRed(MESSAGE_REGEX_MATCH_FALLIDO,msgbuf);
    freeMatch(m);
    return NULL;
  }

  /* IMP */
  reti = regexec(&regexs[3], source, MAX_GROUPS, pmatch, 0);
  if (!reti) return Groups(m,source,groupText,pmatch);
  else if (reti != REG_NOMATCH) {
    regerror(reti, &regexs[3], msgbuf, sizeof(msgbuf));
    printMsgRed(MESSAGE_REGEX_MATCH_FALLIDO,msgbuf);
    freeMatch(m);
    return NULL;
  }

  /* DIMP */
  reti = regexec(&regexs[4], source, MAX_GROUPS, pmatch, 0);
  if (!reti) return Groups(m,source,groupText,pmatch);
  else if (reti != REG_NOMATCH) {
    regerror(reti, &regexs[4], msgbuf, sizeof(msgbuf));
    printMsgRed(MESSAGE_REGEX_MATCH_FALLIDO,msgbuf);
    freeMatch(m);
    return NULL;
  }

  /* SVG */
  reti = regexec(&options[0], source, MAX_GROUPS, pmatch, 0);
  if (!reti) return Groups(m,source,groupText,pmatch);
  else if (reti != REG_NOMATCH) {
    regerror(reti, &options[0], msgbuf, sizeof(msgbuf));
    printMsgRed(MESSAGE_REGEX_MATCH_FALLIDO,msgbuf);
    freeMatch(m);
    return NULL;
  }

  /* STDOUT */
  reti = regexec(&options[1], source, MAX_GROUPS, pmatch, 0);
  if (!reti) return Groups(m,source,groupText,pmatch);
  else if (reti != REG_NOMATCH) {
    regerror(reti, &options[1], msgbuf, sizeof(msgbuf));
    printMsgRed(MESSAGE_REGEX_MATCH_FALLIDO,msgbuf);
    freeMatch(m);
    return NULL;
  }

  /* SVG_NAME */
  reti = regexec(&options[2], source, MAX_GROUPS, pmatch, 0);
  if (!reti) return Groups(m,source,groupText,pmatch);
  else if (reti != REG_NOMATCH) {
    regerror(reti, &options[2], msgbuf, sizeof(msgbuf));
    printMsgRed(MESSAGE_REGEX_MATCH_FALLIDO,msgbuf);
    freeMatch(m);
    return NULL;
  }

  printMsgRed(MESSAGE_LINEA_INCORRECTA,source);
  freeMatch(m);
  return NULL;
}

void LiberarTodo(regex_t regexs[N_WORDS], regex_t options[N_OPTIONS], char* line,Match m) {
  for(int i=0;i<N_WORDS;i++)regfree(&regexs[i]);
  for(int i=0;i<N_OPTIONS;i++)regfree(&options[i]);
  if(line)free(line);
  freeMatch(m);
}

void GenerarTabla(FILE *fich) {
  nlineas = lineas(fich);
  t = CrearTablaTokens();
  if(nlineas < 2) { //Analizar si hay al menos dos lineas
    t->status = STATUS_VACIO;
    return;
  }
  int ret = 0;

  regex_t regexs[N_WORDS];
	regex_t options[N_OPTIONS];

  ret = regcomp(&regexs[0], "^(and)[ \t]*=[ \t]*([!-~]+)[ \t]*$", REG_EXTENDED);
  if (ret) {
      printMsgRed(MESSAGE_FALLO_COMPILAR_REGEX);
      return;
  }

  ret = regcomp(&regexs[1], "^(or)[ \t]*=[ \t]*([!-~]+)[ \t]*$", REG_EXTENDED);
  if (ret) {
      printMsgRed(MESSAGE_FALLO_COMPILAR_REGEX);
      return;
  }

  ret = regcomp(&regexs[2], "^(not)[ \t]*=[ \t]*([!-~]+)[ \t]*$", REG_EXTENDED);
  if (ret) {
      printMsgRed(MESSAGE_FALLO_COMPILAR_REGEX);
      return;
  }

  ret = regcomp(&regexs[3], "^(imp)[ \t]*=[ \t]*([!-~]+)[ \t]*$", REG_EXTENDED);
  if (ret) {
      printMsgRed(MESSAGE_FALLO_COMPILAR_REGEX);
      return;
  }

  ret = regcomp(&regexs[4], "^(dimp)[ \t]*=[ \t]*([!-~]+)[ \t]*$", REG_EXTENDED);
  if (ret) {
      printMsgRed(MESSAGE_FALLO_COMPILAR_REGEX);
      return;
  }

  ret = regcomp(&options[0], "^(svg)[ \t]*=[ \t]*(yes|no)[ \t]*$", REG_EXTENDED);
  if (ret) {
      printMsgRed(MESSAGE_FALLO_COMPILAR_REGEX);
      return;
  }

  ret = regcomp(&options[1], "^(stdout)[ \t]*=[ \t]*(yes|no)[ \t]*$", REG_EXTENDED);
  if (ret) {
      printMsgRed(MESSAGE_FALLO_COMPILAR_REGEX);
      return;
  }

  ret = regcomp(&options[2], "^(svg_name)[ \t]*=[ \t]*([!-~]+)[ \t]*$", REG_EXTENDED);
  if (ret) {
      printMsgRed(MESSAGE_FALLO_COMPILAR_REGEX);
      return;
  }

  Match m = NULL;
  char* line = NULL;
  int tokenSet[N_WORDS];
  int optionSet[N_OPTIONS];
  int i = 0;
  int lineasleidas = 1;
  memset(tokenSet,BOOLEAN_FALSE,N_WORDS*sizeof(int));
  memset(optionSet,BOOLEAN_FALSE,N_OPTIONS*sizeof(int));
  size_t len = 0;
  ssize_t read = getline(&line,&len,fich);
  while(read != -1 && LineaVacia(line) && lineasleidas+1<nlineas) {
    read = getline(&line,&len,fich);
    lineasleidas++;
  }

  while(read != -1 && lineasleidas+1<nlineas) {
    m = getMatch(line,regexs,options);
    lineasleidas++;

    if(m == NULL) {
      //LiberarTodo(regexs,line,m);
      for(int i=0;i<N_WORDS;i++)regfree(&regexs[i]);
      for(int i=0;i<N_OPTIONS;i++)regfree(&options[i]);
			if(line)free(line);
      t->status = STATUS_INCORRECTO;
      return;
    }

    i = 0;
    while(i < N_WORDS+N_OPTIONS) {

      if(i < N_WORDS && strcmp(m->id,WORDS[i]) == 0) {
        if(tokenSet[i]) {
          printMsgRed(MESSAGE_TOKEN_YA_ESPECIFICADO,WORDS[i],t->tokens[i]);
          LiberarTodo(regexs,options,line,m);
          t->status = STATUS_INCORRECTO;
          return;
        }
        tokenSet[i] = BOOLEAN_TRUE;
        strcpy(t->tokens[i],m->string);
        i = N_WORDS+N_OPTIONS; //Salir del bucle
      }

      if(i < N_OPTIONS && strcmp(m->id,OPTIONS[i]) == 0) {
        if(optionSet[i]) {
          printMsgRed(MESSAGE_CONFIG_YA_ESPECIFICADO,OPTIONS[i],t->options[i]);
          LiberarTodo(regexs,options,line,m);
          t->status = STATUS_INCORRECTO;
          return;
        }
        optionSet[i] = BOOLEAN_TRUE;
        strcpy(t->options[i],m->string);
        i = N_WORDS+N_OPTIONS;
      }

      i++;
    }

    if(i == N_WORDS+N_OPTIONS) {
      printf("ERROR\n");
      LiberarTodo(regexs,options,line,m);
      t->status = STATUS_INCORRECTO;
      return;
    }

    freeMatch(m);
    read = getline(&line,&len,fich);
    while(read != -1 && LineaVacia(line) && lineasleidas+1<nlineas) {
      read = getline(&line,&len,fich);
      lineasleidas++;
    }
  }

  if(!LineaVacia(line)) { //Solo analizar la penultima linea si no es vacia
    m = getMatch(line,regexs,options);
    if(m == NULL) {
      //LiberarTodo(regexs,line,m);
      for(int i=0;i<N_WORDS;i++)regfree(&regexs[i]);
      for(int i=0;i<N_OPTIONS;i++)regfree(&options[i]);
			if(line)free(line);
      t->status = STATUS_INCORRECTO;
      return;
    }

    i = 0;
    while(i < N_WORDS+N_OPTIONS) {

      if(i < N_WORDS && strcmp(m->id,WORDS[i]) == 0) {
        if(tokenSet[i]) {
          printMsgRed(MESSAGE_TOKEN_YA_ESPECIFICADO,WORDS[i],t->tokens[i]);
          LiberarTodo(regexs,options,line,m);
          t->status = STATUS_INCORRECTO;
          return;
        }
        tokenSet[i] = BOOLEAN_TRUE;
        strcpy(t->tokens[i],m->string);
        i = N_WORDS+N_OPTIONS; //Salir del bucle
      }

      if(i < N_OPTIONS && strcmp(m->id,OPTIONS[i]) == 0) {
        if(optionSet[i]) {
          printMsgRed(MESSAGE_CONFIG_YA_ESPECIFICADO,OPTIONS[i],t->options[i]);
          LiberarTodo(regexs,options,line,m);
          t->status = STATUS_INCORRECTO;
          return;
        }
        optionSet[i] = BOOLEAN_TRUE;
        strcpy(t->options[i],m->string);
        i = N_WORDS+N_OPTIONS;
      }

      i++;
    }

    if(i == N_WORDS+N_OPTIONS) {
      printf("ERROR\n");
      LiberarTodo(regexs,options,line,m);
      t->status = STATUS_INCORRECTO;
      return;
    }

    freeMatch(m);
  }

  for(int i=0;i<N_WORDS;i++)regfree(&regexs[i]);
  for(int i=0;i<N_OPTIONS;i++)regfree(&options[i]);
  if(line)free(line);

  for(int i=0;i<N_WORDS;i++) { //Si la tabla no esta rellenada entera, liberarla, porque es invalida
    if(!tokenSet[i]) {
      printMsgRed(MESSAGE_TOKEN_NO_ESPECIFICADO,WORDS[i]);
      t->status = STATUS_INCORRECTO;
      return;
    }
  }
  print_TablaTokens();
}

int getCodigoDesdeIndice(int indice) {
    switch (indice) {
      case 0:
        return AND;
      case 1:
        return OR;
      case 2:
        return NOT;
      case 3:
        return IMP;
      case 4:
        return DIMP;
    }
}

int TablaCorrecta() {
  if(t->status == STATUS_CORRECTO || t->status == STATUS_VACIO)return BOOLEAN_TRUE;
  else return BOOLEAN_FALSE;
}

int mostrarTree() {
    if(t->options[OPTION_STDOUT_INDEX] != NULL) {
      if(strcmp(t->options[OPTION_STDOUT_INDEX],"yes") == 0)return BOOLEAN_TRUE;
      return BOOLEAN_FALSE;
    }
    return -1;
}

int getCodigoToken(char* token) {
  if(t->status == STATUS_VACIO) { //No tener en cuenta la tabla
    if(strcmp(token,WORDS[0]) == 0)return AND;
    else if(strcmp(token,WORDS[1]) == 0)return OR;
    else if(strcmp(token,WORDS[2]) == 0)return NOT;
    else if(strcmp(token,WORDS[3]) == 0)return IMP;
    else if(strcmp(token,WORDS[4]) == 0)return DIMP;
  }
  else {
    for(int i=0;i<N_WORDS;i++) {
      if(strcmp(t->tokens[i],token) == 0)return getCodigoDesdeIndice(i);
    }
  }
  printMsgRed(MESSAGE_TOKEN_NO_ESPECIFICADO_INCORRECTO,token);
  return BISON_TOKEN_ERROR;
}
