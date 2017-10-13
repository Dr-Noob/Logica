# Resolución de Tableaux(Lógica proposicional)

El objetivo de este proyecto es resolver un tableaux semántico de lógica proposicional(L0)

### Estructura del proyecto

    |-- Config.c
    |-- Config.h
    |-- Formula.c
    |-- Formula.h
    |-- Global.c
    |-- Global.h
    |-- Makefile
    |-- README.md
    |-- SVG.c
    |-- SVG.h
    |-- Tree.c
    |-- Tree.h
    |-- doc.pdf
    |-- flex.supp
    |-- grammar.y
    |-- lex.l
    |-- main.c
    |-- prueba
    `-- pruebas
        |-- 1
        |-- 10
        |-- 11
        |-- 12
        |-- 13
        |-- 14
        |-- 15
        |-- 16
        |-- 2
        |-- 3
        |-- 4
        |-- 5
        |-- 6
        |-- 7
        |-- 8
        `-- 9

    
### Prerequisitos

Para compilar el proyecto es necesario tener instalado tanto Bison como Flex(y un compilador de C). Esta pequeña guía la haré usando Linux.

### Funcionamiento general

Para ejecutar el programa, es necesario siempre pasarle un fichero cualquiera(fichero de configuración) donde se incluya, al menos, una expresión, aunque también puede usarse para escribir operadores personalizados y otras opciones extra de configuración.

### La salida

A partir de la fórmula inicial, el programa mostrará una salida(un árbol en cualquier caso), que saldrá por:

* La salida estándar(terminal)

Si el programa considera que el árbol generado cabe en la terminal en un tamaño razonable, lo imprimirá, en caso contrario, no lo hará(excepto si se ha usado una opción de configuración que fuerze a hacerlo)

* Fichero SVG

Se genera siempre(excepto si se ha usado una opción de configuración que fuerze a no hacerlo)un fichero SVG donde se encuentra el árbol dibujado. Dicho fichero puede visualizarse con cualquier navegador, o mediante un visor de imágenes

### Como escribir expresiones

En el fichero de configuración hay que escribir la expresión lógica(en la última línea) que se quiera resolver. Para esto, existen dos opciones; Es posible utilizar expresiones personalizadas, mediante el uso del fichero de configuración, o usar las expresiones por defecto. A continuación se detallan las dos opciones, siendo el primer punto para aquellas cuestiones que son comunes para ambas.

* Expresiones comunes a ambas

Los atomos deben especificarse mediante cualquier letra, siempre y cuando sea solo una(en minúscula). El separador permite separar la expresión en distintas fórmulas, mientras que los paréntesis permiten modificar el orden y la asociatividad. Dichos operadores, cuya sintaxis se detalla, no pueden modificarse:

| Operador          | Sintaxis  |
|:-----------------:|:---------:|
| Parentesis Izq    | `)`       |
| Parentesis Der    | `(`       |
| Separador         | `,`       |

* Expresiones personalizadas

Para especificar los operadores de forma persoanlizada, es necesario seguir el siguiente esquema:

```
OPERADOR = operador_personalizado
```

Por ejemplo, si quisieramos cambiar el operador AND, sería:

```
and = ^
```

De esta forma, cuando en la expresión aparezca '^' se entiende que es un and lógico. Los espacios entre `OPERADOR` y `operador_personalizado` son opcionales. Para especificar los operadores, hay que seguir la sintaxis que se detalla en la siguiente tabla

| Operador          | Sintaxis  |
|:-----------------:|:---------:|
| AND               | `and`     |
| OR                | `or`      |
| NOT               | `not`     |
| Implicación       | `imp`     |
| Doble Implicación | `dimp`    |


Dicho esto, una especificación completa y correcta sería una como la siguiente:

```
and = ^
or = |
not = ~
imp = ->
dimp = <->

a ^ ~(b | c <-> a)
```

Es decir, la última línea es la fórmula en sí, mientras que lo de arriba es la especificación de los operadores. Estos siempre deberán escribirse así pues no pueden modificarse. 

* Expresiones predeterminadas

En la siguiente tabla se detalla cómo escribir los operadores

| Operador          | Sintaxis  |
|:-----------------:|:---------:|
| AND               | `and`     |
| OR                | `or`      |
| NOT               | `not`     |
| Implicación       | `imp`     |
| Doble Implicación | `dimp`    |

En este caso, la misma expresión del apartado anterior, en este caso sería:

```
a and not(b or c dimp a)
```

### Configuración adicional

Es posible especificar otras opciones de configuración, como son:

| Opción          | Sintaxis  |
|:-----------------:|:---------:|
| stdout=(yes\|no)   | Mediante `yes` se le dice al programa que siempre muestre el árbol resultado por la salida estándar(por terminal) mientras que `no` fuerza a no mostrar el árbol  |
| svg=(yes\|no)      | Igual que la opción `stdout`, pero para el fichero `SVG` |
| svg_name=nombre   | Especifica el nombre del fichero `SVG` generado |

### Haciendo pruebas

Para compilar el proyecto es necesario tener ciertas cosas instaladas, como se detalla unas cuantas secciones más abajo, en `Compilado con`. Para descargar el proyecto, puedes hacerlo desde la propia página web, o mediante git, con:

```
git clone https://github.com/Dr-Noob/Logica
```

Una vez dentro del directorio del proyecto, que incluye un makefile, es suficiente con ejecutar
```
make
```
Esto genera un fichero ejecutable llamado 'tableaux'(si todo ha ido bien) Suponiendo que el fichero de configuración se llama 'ejemplo':

```
./tableaux ejemplo
```

Resolvería la fórmula escrita en el fichero. Si la expresión no es correcta, se avisará con un mensaje de error.
Los nodos hoja en rojo son nodos cerrados, mientras que los verdes son nodos abiertos.

### Ejemplos

A continuación se muestran unos cuantos ejemplos de cómo es el árbol por la salida estándar;

La expresión

```
not(p dimp a),a or s,not a
```

genera

```


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

```

La expresión

```
p imp q,q imp p and r,p imp (p imp q) imp r
```
genera

```


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

```

## Compilado con

* [Flex](https://www.gnu.org/software/flex/) - Analizador léxico
* [Bison](https://www.gnu.org/software/bison/) - Analizador sintáctico
* [GCC](https://www.gnu.org/software/gcc/) - Compilador C

###### viva gnu D:

## Notas

* Para dibujar el árbol(fichero Tree.c)me he apoyado de código que encontré en [esta web](http://web.archive.org/web/20071224095835/http://www.openasthra.com/wp-content/uploads/2007/12/binary_trees1.c)
