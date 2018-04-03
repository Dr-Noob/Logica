# Resolución de Tableaux(Lógica proposicional)

El objetivo de este proyecto es resolver un tableaux semántico de lógica proposicional(L0)

*__ÍNDICE:__*

1. [Estructura del proyecto](#estructura-del-proyecto)
2. [Descarga y uso básico](#descarga-y-uso-básico)
3. [Nota importante](#nota-importante)
4. [Cómo escribir expresiones](#cómo-escribir-expresiones)
5. [Configuración adicional](#configuración-adicional)
6. [La salida](#la-salida)
7. [Compilando](#compilando)
8. [Ejemplos](#ejemplos)
9. [Implementación](#implementación)
10. [Compilado con](#compilado-con)
11. [Notas](#notas)

### Estructura del proyecto

    ├── doc.pdf
    ├── flex.supp
    ├── Makefile
    ├── pruebas
    │   ├── 1-16
    │   └── complejas
    │       └── 1-11
    ├── README.md
    └── src
        ├── common
        │   ├── basename.h
        │   ├── Controlador.h
        │   ├── Formula.c
        │   ├── Formula.h
        │   ├── Global.c
        │   ├── Global.h
        │   ├── grammar.y
        │   ├── SVG.c
        │   ├── SVG.h
        │   ├── Tree.c
        │   └── Tree.h
        ├── unix
        │   ├── basename.c
        │   ├── Config.c
        │   ├── Config.h
        │   ├── Controlador.c
        │   ├── Fuente.h
        │   ├── lex.l
        │   └── main.c
        └── windows
            ├── basename.c
            ├── Controlador.c
            ├── Fuente.h
            ├── lex.l
            └── main.c



### Descarga y uso básico

Para descargar el software, ve a `releases`(pulsando en la parte de arriba en github, o yendo [aqui](https://github.com/Dr-Noob/Logica/releases)). Si quieres compilarlo tú mismo, puedes echarle un vistazo al apartado de [Haciendo pruebas](#haciendo-pruebas)

Para ejecutar el programa, es necesario siempre pasarle un fichero cualquiera(fichero de configuración) como parámetro donde se incluya, al menos, una expresión, aunque también puede usarse para escribir operadores personalizados y otras opciones extra de configuración.

Por ejemplo,

```
./tableaux prueba
```

donde `prueba` es el fichero en el que está la expresión. Para ver cómo escribir dicha expresión, ve al apartado [Cómo escribir expresiones](#cómo-escribir-expresiones)

### Nota importante

En esta guía se habla de fichero de configuración. Dicho fichero, incluye tanto la expresión o fórmula, como __la configuración adicional__. Dicha configuración, por motivos de implementación, __no puede usarse desde Windows__. Cualquier intento provocará un error.

### Cómo escribir expresiones

En el fichero de configuración hay que escribir la expresión lógica(en la última línea) que se quiera resolver. Para esto, existen dos opciones; Es posible utilizar expresiones personalizadas, mediante el uso del fichero de configuración, o usar las expresiones por defecto. A continuación se detallan las dos opciones, siendo el primer punto para aquellas cuestiones que son comunes para ambas.

* Expresiones comunes a ambas formas

Los atomos deben especificarse mediante cualquier letra, siempre y cuando sea solo una y en minúscula. El separador permite separar la expresión en distintas fórmulas, mientras que los paréntesis permiten modificar el orden y la asociatividad y las llaves delimitan el conjunto(con el mismo significado que los paréntesis). Dichos operadores, cuya sintaxis se detalla, no pueden modificarse:

| Operador          | Sintaxis  |
|:-----------------:|:---------:|
| Parentesis Izq    | `)`       |
| Parentesis Der    | `(`       |
| Parentesis Der    | `{`       |
| Parentesis Der    | `}`       |
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

De esta forma, cuando en la expresión aparezca '^' se entiende que es un `and` lógico. Los espacios entre `OPERADOR` y `operador_personalizado` son opcionales. Para especificar los operadores, hay que seguir la sintaxis que se detalla en la siguiente tabla

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

Es decir, la última línea es la fórmula en sí, mientras que lo de arriba es la especificación de los operadores. Estos siempre deberán escribirse así pues no pueden modificarse(Los espacios que hay entre líneas no afectan en nada al funcionamiento)

* Expresiones predeterminadas

En la tabla anterior puede verse también como escribir cada operador. En este caso, la misma expresión del apartado anterior, en este caso sería:

```
a and not(b or c dimp a)
```

### Configuración adicional

Es posible especificar otras opciones de configuración, como son:

| Opción              | Sintaxis  |
|:-------------------:|:---------:|
| stdout=(yes\|no)    | Mediante `yes` se le dice al programa que siempre muestre el árbol resultado por la salida estándar(por terminal) mientras que `no` fuerza a no mostrar el árbol(por defecto, `yes`  |
| svg=(yes\|no)       | Igual que la opción `stdout`, pero para el fichero `SVG`(por defecto `yes`) |
| svg_name=nombre    | Especifica el nombre del fichero `SVG` generado (por defecto el nombre del fichero de entrada-`sol.svg`(p e.j `prueba1-sol.svg`) |

Entonces, un fichero de configuración completo, con todas las opciones, sería uno como el siguiente:
```
and = ^
or = |
not = ~
imp = ->
dimp = <->

stdout=no
svg=yes
svg_name=salida.svg

a ^ ~(b | c <-> a)
```

en el que estamos expresando que no queremos que se muestre el árbol por la salida estándar, que genere siempre un svg cuyo nombre sea `salida.svg`

### La salida

A partir de la fórmula inicial, el programa mostrará una salida(un árbol en cualquier caso), que saldrá por:

* La salida estándar(terminal)

Si el programa considera que el árbol generado cabe en la terminal en un tamaño razonable, lo imprimirá, en caso contrario, no lo hará(excepto si se ha usado una opción de configuración que fuerze a hacerlo)

* Fichero SVG

Se genera siempre(excepto si se ha usado una opción de configuración que fuerze a no hacerlo)un fichero SVG donde se encuentra el árbol dibujado. Dicho fichero puede visualizarse con cualquier navegador, o mediante un visor de imágenes

### Compilando

Para compilar el proyecto es necesario tener ciertas cosas instaladas, como se detalla unas cuantas secciones más abajo, en [Compilado con](#compilado-con). Para descargar el proyecto, puedes hacerlo desde la propia página web, o mediante git, con:

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
                      /                       \
                     /                         \
                    /                           \
                   /                             \
                  /                               \
        ~(p -> a)(a v s)(~a)             ~(a -> p)(a v s)(~a)
                 |                                  |
         (p)(~a)(a v s)(~a)                (a)(~p)(a v s)(~a)
                / \                               / \
               /   \                             /   \
              /     \                           /     \
             /       \                         /       \
            /         \                       /         \
           /           \                     /           \
          /             \                   /             \
         /               \                 /               \
  (p)(~a)(a)(~a)   (p)(~a)(s)(~a)   (a)(~p)(a)(~a)   (a)(~p)(s)(~a)

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
                                                    /                                                   \
                                                   /                                                     \
                                                  /                                                       \
                                                 /                                                         \
                                                /                                                           \
                                               /                                                             \
                                              /                                                               \
                                             /                                                                 \
                                            /                                                                   \
                                           /                                                                     \
                                          /                                                                       \
                                         /                                                                         \
                     (~p)(q -> (p ^ r))((p -> (p -> q)) -> r)                                    (q)(q -> (p ^ r))((p -> (p -> q)) -> r)
                                       / \                                                                         / \
                                      /   \                                                                       /   \
                                     /     \                                                                     /     \
                                    /       \                                                                   /       \
                                   /         \                                                                 /         \
                                  /           \                                                               /           \
                                 /             \                                                             /             \
                                /               \                                                           /               \
                               /                 \                                                         /                 \
                              /                   \                                                       /                   \
                             /                     \                                                     /                     \
                            /                       \                                                   /                       \
                           /                         \                                                 /                         \
                          /                           \                                               /                           \
                         /                             \                                             /                             \
                        /                               \                                           /                               \
                       /                                 \                           (q)(~q)((p -> (p -> q)) -> r)   (q)(p ^ r)((p -> (p -> q)) -> r)
        (~p)(~q)((p -> (p -> q)) -> r)    (~p)(p ^ r)((p -> (p -> q)) -> r)                       / \                                |
                     / \                                  |                                       /   \                (q)(p)(r)((p -> (p -> q)) -> r)
                    /   \                  (~p)(p)(r)((p -> (p -> q)) -> r)                     /     \                             / \
                   /     \                               / \                                   /       \                           /   \
                  /       \                             /   \                                 /         \                         /     \
                 /         \                           /     \                               /           \                       /       \
                /           \                         /       \                             /             \                     /         \
               /             \                       /         \                           /               \                   /           \
              /               \                     /           \                         /                 \                 /             \
             /                 \                   /             \            (q)(~q)~(p -> (p -> q))   (q)(~q)(r)           /               \
 (~p)(~q)~(p -> (p -> q))  (~p)(~q)(r)            /               \                      |                                   /                 \
            |                                     /                 \            (q)(~q)(p)~(p -> q)                        /                   \
   (~p)(~q)(p)~(p -> q)                         /                   \                    |                     (q)(p)(r)~(p -> (p -> q))   (q)(p)(r)(r)
            |                       (~p)(p)(r)~(p -> (p -> q))  (~p)(p)(r)(r)     (q)(~q)(p)(p)(~q)                        |
    (~p)(~q)(p)(p)(~q)                         |                                                                 (q)(p)(r)(p)~(p -> q)
                                     (~p)(p)(r)(p)~(p -> q)                                                               |
                                               |                                                                  (q)(p)(r)(p)(p)(~q)
                                      (~p)(p)(r)(p)(p)(~q)
```

## Implementación
Debido a distintos factores, se ha dividido el desarrollo entre `Windows` y `Unix`. De forma resumida:

* Los códigos escape ANSI no están soportados por la terminal en `Windows` por defecto en todas las versiones, mientras que sí sucede(en la mayoría) en `Unix`(`Linux` y `MacOS`)
* Ciertas funciones son dependientes de cada plataforma(`basename` en `Unix`/`_splitpath` en `Windows`)
* Para analizar el fichero de configuración, se usan expresiones regulares, con ayuda de `regex.h` para C. Dicha cabecera no se encuentra de forma sencilla en una instalación de `Windows`

Una guía más en detalle de la implementación puede encontrarse en [este pdf](doc.pdf), aunque ciertos detalles pueden quedarse desactualizados en nuevas versiones, por lo que se recomenda acudir siempre al código fuente.

En grandes rasgos, el funcionamiento es:
1. `Flex` lee la última línea del fichero que se pasa como parámetro y va pasando tokens a `Bison`.
2. `Bison` reliza el análisis semántico y va construyendo la estructura de datos que representa el tableaux.
3. Dicha estructura se desarrolla y resuelve.
4. Esta estructura se pasa a una función que la imprimirá por pantalla, y otra que la imprimirá en un fichero svg.


## Compilado con

* [Flex](https://www.gnu.org/software/flex/) - Analizador léxico
* [Bison](https://www.gnu.org/software/bison/) - Analizador sintáctico
* [GCC](https://www.gnu.org/software/gcc/) - Compilador C

###### viva gnu D:

## Notas

* Para dibujar el árbol(fichero Tree.c)me he apoyado de código que encontré en [esta web](http://web.archive.org/web/20071224095835/http://www.openasthra.com/wp-content/uploads/2007/12/binary_trees1.c)
