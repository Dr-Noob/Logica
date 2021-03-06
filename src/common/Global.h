#ifndef __Global__
#define __Global__

#include <locale.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define BOOLEAN_FALSE         0
#define BOOLEAN_TRUE          1

#define ESP_LANG              0
#define OTHER_LANG            1

static const char* ESP_LANG_STR1 = "es"; //Unix LANG
static const char* ESP_LANG_STR2 = "Spanish"; //Windows
extern int LANG;

#ifdef WINDOWS

#define RED
#define GREEN
#define RESET

#else

#define RED "\x1b[31;1m"
#define GREEN "\x1b[32;1m"
#define RESET "\x1b[0m"

#endif


//MENSAJES

//CONFIG//
static const char* MESSAGE_TOKEN_DEMASIADO_LARGO[2] =  { "ERROR: Se ha especificado un token de mas de %d caracteres(%s)\n", "ERROR: Specified a token larger than %d characters(%s)\n" };
static const char* MESSAGE_REGEX_MATCH_FALLIDO[2] = { "ERROR: Regex match fallido\n", "ERROR: Regex match failed\n" };
static const char* MESSAGE_LINEA_INCORRECTA[2] = { "ERROR: La linea '%s' es incorrecta\n", "ERROR: Line '%s' is invalid\n" };
static const char* MESSAGE_FALLO_COMPILAR_REGEX[2] = { "ERROR: Fallo al compilar la expresion regular\n", "ERROR: Failed to compile regex\n" };
static const char* MESSAGE_TOKEN_ESPECIAL[2] = { "ERROR: Se ha utilizado el token especial '%s'(los token especiales son '(',')',',' y cualquier letra(una sola letra minuscula))\n", "ERROR: Special token '%s' was used(special tokens are '(',')',',' and any word(single lower case word))\n" };
static const char* MESSAGE_TOKEN_YA_ESPECIFICADO[2] = { "ERROR: El token '%s' ya habia sido especificado como '%s'\n", "ERROR: Token '%s' was already specified as '%s'\n" };
static const char* MESSAGE_TOKEN_NO_ESPECIFICADO[2] = { "ERROR: El token '%s' no ha sido especificado\n", "ERROR: Token '%s' was not specified\n" };
static const char* MESSAGE_TOKEN_NO_ESPECIFICADO_INCORRECTO[2] = { "ERROR: La formula contiene el token '%s', que no ha sido especificado en el fichero de configuracion o no es correcto\n", "ERROR: Formula contains token '%s', which was not specified in the configuration file or it is not correct\n" };
static const char* MESSAGE_CONFIG_YA_ESPECIFICADO[2] = { "ERROR: La opcion '%s' ya habia sido especificada como '%s'\n", "ERROR: Option '%s' was already specified as '%s'\n" };
static const char* MESSAGE_TOKEN_YA_UTILIZADO_COMO_OPERADOR[2] = { "ERROR: El token '%s' ya habia sido utilizado en otro operador anteriormente\n", "ERROR: Token '%s' was already used in other operator before\n" };

//FORMULA//
static const char* MESSAGE_ARBOL_DEMASIADO_GRANDE[2] =  { "El arbol es demasiado grande para dibujarlo por la salida estandar\nSi aun asi quieres dibujarlo, especificalo en el fichero de configuracion\n", "The tree is too big to draw it by standart output\nIf you still want to draw it, specify it on the config file\n" };
static const char* MESSAGE_SOLUCION[2] = { "Solucion: \n\n\n", "Solution: \n\n\n" };
static const char* MESSAGE_TABLEAUX_CERRADO[2] = { "El tableaux esta cerrado\n" , "The tableaux is closed\n" };
static const char* MESSAGE_TABLEAUX_INSTATISFACIBLE[2] = { "La expresion inicial es insatisfacible\n" , "The initial expression is unsatisfactory\n" };
static const char* MESSAGE_TABLEAUX_ABIERTO[2] = { "El tableaux esta abierto\n" , "The tableaux is open\n" };
static const char* MESSAGE_TABLEAUX_STATISFACIBLE[2] = { "La expresion inicial es satisfacible\n" , "The initial expression is satisfactory\n" };
static const char* MESSAGE_ABRIR_ARCHIVO_FALLIDO[2] = { "ERROR: El archivo '%s' no ha podido abrirse\n", "ERROR: File '%s' could not be opened\n" };
static const char* MESSAGE_NO_SVG[2] = { "No se genera el SVG\n" , "SVG will not be generated\n" };

//TREE//
static const char* MESSAGE_ARBOL_MUY_GRANDE[2] = { "WARNING: El arbol es mas alto de %d, y puede que se dibuje de forma incorrecta\n", "WARNING: This tree is taller than %d, and may be drawn incorrectly\n" };

//MAIN//
static const char* MESSAGE_ARGS_MAIN[2] = { "ERROR: Tableaux necesita al menos un argumento(el archivo donde se encuentra la formula)\n", "ERROR: Tableaux needs at least one argument(the file where it is the formula)\n" };

//GRAMATICA//
static const char* MESSAGE_EXPRESION_CORRECTA[2] = { "La expresion se ha reconocido correctamente\n", "Expression was recognised successfully\n" };
static const char* MESSAGE_ERROR_SEMANTICO[2] = { "ERROR: Error semantico(%d-%d) Por favor, revisa la formula\n", "ERROR: Semantic error(%d-%d) Please, check the formula\n" };

//FIN_MENSAJES

void printMsg(const char *fmt[2], ...);
void printMsgRed(const char *fmt[2], ...);
void printMsgGreen(const char *fmt[2], ...);
void getLang();

int LongitudCaracteres(FILE *fichero);
int lineas(FILE *fich);

#endif
