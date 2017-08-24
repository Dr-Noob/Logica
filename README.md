# Resolución de Tableaux(Lógica proposicional)

El objetivo de este proyecto es resolver un tableaux semántico de lógica proposicional(L0)

### Estructura del proyecto

    ├── Formula.c
    ├── Formula.h
    ├── grammar.y
    ├── lex.l
    ├── main.c
    ├── Makefile
    ├── README.md
    ├── Tree.c
    └── Tree.h
    
### Prerequisitos

Para compilar el proyecto es necesario tener instalado tanto Bison como Flex(y un compilador de C). Esta pequeña guía la haré mediante Linux.

### Sintaxis para escribir expresiones

En la tabla se detalla cómo escribir los operadores

| Operador          | Sintaxis  |
|:-----------------:|:---------:|
| AND               | 'and'     |
| OR                | 'or'      |
| NOT               | 'not'    |
| Implicación       | 'imp'     |
| Doble Implicación | 'dimp'    |
| Separador         | ','       |

Los atomos pueden especificarse mediante cualquier letra, siempre y cuando sea solo una. El separador permite separar la expresión en distintas fórmulas.

### Haciendo unos tests

El proyecto incluye un makefile, así que es suficiente con ejecutar
```
make
```
Esto genera un fichero ejecutable llamado 'tableaux'. Para introducir la expresión a resolver, es necesario escribirla en un fichero de texto y pasársela al ejecutable. Suponiendo que el fichero se llama 'ejemplo':

```
./tableaux ejemplo
```

Resolvería la fórmula escrita en el fichero. 
Si la expresión no es correcta, se avisará con un mensaje de error.
Los nodos hoja en rojo son nodos cerrados, mientras que los verdes son nodos abiertos.

### Ejemplos

A continuación se muestran unos cuantos ejemplos;

La expresión

```
not(p dimp a),a or s,not a
```

genera

```
La expresion se ha reconocido correctamente
Solucion: 


                   ~(p <-> a)(a v s)(~a)
                            / \
                           /   \
                          /     \
                         /       \
                        /         \
                       /           \
                      /             \
                     /               \
                    /                 \
                   /                   \
                  /                     \
        ~(p -> a)(a v s)(~a)   ~(a -> p)(a v s)(~a)
                 |                        |
         (p)(~a)(a v s)(~a)      (a)(~p)(a v s)(~a)
                / \
               /   \
              /     \
             /       \
            /         \
           /           \
          /             \
         /               \
  (p)(~a)(a)(~a)   (p)(~a)(s)(~a)



El tableaux esta abierto
La expresion inicial es satisfacible
```

La expresión

```
p imp q,q imp p and r,p imp (p imp q) imp r
```
genera

```
La expresion se ha reconocido correctamente
Solucion: 


                                           (p -> q)(q -> (p ^ r))((p -> (p -> q)) -> r)
                                                                / \
                                                               /   \
                                                              /     \
                                                             /       \
                                                            /         \
                                                           /           \
                                                          /             \
                                                         /               \
                                                        /                 \
                                                       /                   \
                                                      /                     \
                                                     /                       \
                                                    /                         \
                                                   /                           \
                                                  /                             \
                                                 /                               \
                                                /                                 \
                                               /                                   \
                                              /                                     \
                                             /                                       \
                                            /                                         \
                                           /                                           \
                                          /                                             \
                                         /                                               \
                                        /                                                 \
                    (~p)(q -> (p ^ r))((p -> (p -> q)) -> r)            (q)(q -> (p ^ r))((p -> (p -> q)) -> r)
                                      / \                                                 / \
                                     /   \                                               /   \
                                    /     \                                             /     \
                                   /       \                                           /       \
                                  /         \                                         /         \
                                 /           \                                       /           \
                                /             \                                     /             \
                               /               \                                   /               \
                              /                 \                                 /                 \
                             /                   \                               /                   \
                            /                     \                             /                     \
                           /                       \                           /                       \
                          /                         \                         /                         \
                         /                           \                       /                           \
                        /                             \                     /                             \
                       /                               \                   /                               \
                      /                                 \   (q)(~q)((p -> (p -> q)) -> r)   (q)(p ^ r)((p -> (p -> q)) -> r)
       (~p)(~q)((p -> (p -> q)) -> r)    (~p)(p ^ r)((p -> (p -> q)) -> r)                                  |
                    / \                                  |                                    (q)(p)(r)((p -> (p -> q)) -> r)
                   /   \                  (~p)(p)(r)((p -> (p -> q)) -> r)                                 / \
                  /     \                                                                                 /   \
                 /       \                                                                               /     \
                /         \                                                                             /       \
               /           \                                                                           /         \
              /             \                                                                         /           \
             /               \                                                                       /             \
            /                 \                                                                     /               \
(~p)(~q)~(p -> (p -> q))  (~p)(~q)(r)                                                              /                 \
           |                                                                                       /                   \
  (~p)(~q)(p)~(p -> q)                                                               (q)(p)(r)~(p -> (p -> q))   (q)(p)(r)(r)
                                                                                                 |
                                                                                       (q)(p)(r)(p)~(p -> q)
                                                                                                 |
                                                                                        (q)(p)(r)(p)(p)(~q)



El tableaux esta abierto
La expresion inicial es satisfacible
```
