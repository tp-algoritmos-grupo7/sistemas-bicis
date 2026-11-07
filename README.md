# sistemas-bicis

## Compilacion

```
make            # o mingw32-make en Windows
./sistema_bicis
```

Se compila con `g++ -std=c++17 -Wall -Wextra` (el Makefile agrega `-Werror -pedantic`).
El programa debe ejecutarse desde la raiz del repositorio para encontrar la carpeta `datos/`.

## A. Gestion de estaciones

### Archivos

| Archivo | Contenido |
|---|---|
| `Estacion.hpp/.cpp` | Clase `Estacion` (codigo, nombre, barrio, anclajes, bicis disponibles). |
| `Lista.hpp` | Lista enlazada generica con iterador. |
| `Cola.hpp` | Cola FIFO generica (`encolar` / `desencolar`) apoyada en `Lista`. |
| `TablaHash.hpp/.cpp` | Tabla de hash que indexa las estaciones por codigo. |
| `GrafoCiclovias.hpp/.cpp` | Grafo de ciclovias con listas de adyacencia y busqueda BFS. |
| `GestionEstaciones.hpp/.cpp` | Carga de archivos, ordenamiento por seleccion y menu de la parte A. |

### Tabla de hash

- Funcion hash: suma de los codigos ASCII de los caracteres del codigo, modulo `M = 31` (primo).
- Resolucion de colisiones: encadenamiento (cada casillero es una `Lista<Estacion*>`).

Con los datos de prueba (`datos/estaciones.txt`):

| Codigo | Suma ASCII | Indice (suma % 31) |
|---|---|---|
| PLZ | 246 | 29 |
| PAL | 221 | 4 |
| CAB | 198 | 12 |
| REC | 218 | 1 |
| BEL | 211 | 25 |
| SNT | 245 | 28 |

- **Colisiones: 0** (las 6 estaciones caen en casilleros distintos).
- **Factor de carga final: N / M = 6 / 31 ≈ 0.19.**

### Listado ordenado (ordenamiento por seleccion)

Se copian los punteros de las estaciones de la tabla a un arreglo auxiliar y se ordenan con
**seleccion**: en cada vuelta se busca la mayor (o menor) entre las que faltan y se intercambia
con la posicion actual. El usuario elige el criterio (capacidad de anclajes o bicis disponibles)
y el sentido (mayor a menor o menor a mayor). La tabla hash no se modifica.

- Complejidad: **O(n²)** comparaciones en todos los casos; a lo sumo n-1 intercambios.

### Recorrido por ciclovias (BFS)

Las estaciones son los vertices del grafo y las ciclovias de `datos/ciclovias.txt` sus aristas
(bidireccionales). Se guarda con **listas de adyacencia**: a cada estacion se le asigna un indice
y `adyacentes[i]` tiene los indices de sus vecinas.

La busqueda en anchura recorre el grafo por niveles usando una `Cola`: primero las estaciones a 1
tramo del origen, despues las que estan a 2, etc. Por eso, la primera vez que se llega al destino
es por el camino con **menor cantidad de tramos**. El arreglo `anterior[]` guarda desde donde se
llego a cada estacion y permite reconstruir el camino yendo del destino hacia el origen.
Si el destino nunca se visita, se informa que no existe recorrido.

Al eliminar una estacion tambien se borran sus ciclovias del grafo, asi no se usa en recorridos.

- Complejidad: **O(V + E)** (V = estaciones, E = ciclovias): cada estacion entra una vez a la
  cola y cada lista de adyacencia se recorre una vez.
- Se eligio lista de adyacencia porque la red tiene pocas ciclovias por estacion (grafo disperso):
  ocupa O(V + E) de memoria en lugar de O(V²) de una matriz.

## C. Registro de bicicletas con arbol binario de busqueda

### Archivos

| Archivo | Contenido |
|---|---|
| `Bicicleta.h/.cpp` | Clase `Bicicleta` (ID, codigo de estacion, tipo, estado y kilometros). |
| `NodoAbb.h/.cpp` | Clase `Nodo`: guarda una bicicleta y los punteros a sus hijos izquierdo y derecho. |
| `Abb.h/.cpp` | ABB propio con insercion, busqueda, recorridos, conteos, altura, eliminacion y liberacion de memoria. |
| `RegistroBicicleta.h/.cpp` | Clase `RegistroBicicletas`: contiene el ABB, carga las bicicletas desde archivo y ofrece las operaciones al menu. |
| `MenuBicicletas.h/.cpp` | Lectura validada de enteros y menu para agregar, buscar, listar, contar averiadas, consultar altura y eliminar bicicletas. |
| `main.cpp` | Carga inicial de `datos/bicicletas.txt` y acceso al menu de bicicletas. |

### Organizacion del ABB

Todas las bicicletas se guardan en un **arbol binario de busqueda ordenado por `biciId`**.
Cada nodo contiene una bicicleta completa, un puntero al hijo izquierdo y otro al derecho.
La clase `Abb` guarda el puntero `raiz`, que vale `nullptr` cuando el arbol esta vacio.

- En el subarbol izquierdo quedan los IDs menores que el del nodo.
- En el subarbol derecho quedan los IDs mayores.
- No se permiten IDs repetidos.

Los nodos se crean con `new` y el arbol se encarga de liberarlos. 

Al iniciar el programa, `RegistroBicicletas::cargarDesdeArchivo` lee `datos/bicicletas.txt`
e inserta cada bicicleta en el ABB. La carga ocurre una sola vez, antes del menu principal.
Los IDs duplicados se omiten y se informa el problema; las bicicletas ya cargadas se conservan.

### Insercion y busqueda

Ambos metodos son **iterativos**: usan un puntero `actual` que comienza en la raiz y un ciclo
`while` para avanzar por el arbol.

- **`insertar(bicicleta)`**: si el arbol esta vacio, crea la raiz. En otro caso, compara el ID
  con el nodo actual. Si es menor, avanza a la izquierda; si es mayor, a la derecha.
  Cuando encuentra el lugar libre, crea el nodo y devuelve `true`. Si el ID ya existe,
  devuelve `false` sin modificar la bicicleta guardada.
- **`buscar(biciId)`**: compara el ID y avanza por el lado que puede contenerlo. Si lo encuentra,
  devuelve un puntero de lectura a la bicicleta; si llega a `nullptr`, devuelve `nullptr`.
  La bicicleta sigue perteneciendo al arbol: quien recibe el puntero no debe liberarla.

El costo depende de la cantidad de nodos del camino recorrido: **O(h + 1)**, donde `h` es
la altura del arbol. En un arbol balanceado es **O(log n)** y, en el peor caso de una cadena,
**O(n)**. Estos dos metodos usan **O(1)** de memoria auxiliar.

### Recorridos, conteos y altura

Estos metodos tienen una version publica que inicia desde la raiz y una auxiliar privada
que recibe un nodo. La auxiliar permite repetir la operacion recursivamente en cada subarbol.

- **`recorridoInorden()`**: visita primero el subarbol izquierdo, luego muestra la bicicleta
  del nodo y finalmente visita el derecho. Asi lista todas las bicicletas por ID ascendente.
- **`contarNodos()`**: devuelve `0` para un nodo nulo. Para cada nodo existente suma
  `1 + cantidad izquierda + cantidad derecha`.
- **`contarBicicletasAveriadas()`**: recorre todo el arbol y suma `1` si la bicicleta del nodo
  esta averiada, mas los resultados de ambos subarboles.
- **`altura()`**: calcula `1 + maximo(altura izquierda, altura derecha)`. La altura se mide
  en **aristas**: un arbol vacio tiene altura `-1` y una unica raiz tiene altura `0`.

Cada operacion visita todos los nodos, por lo que cuesta **O(n)**. Las llamadas recursivas
ocupan **O(h + 1)** de memoria auxiliar.

### Eliminacion de bicicletas

**`eliminar(biciId)`** busca el nodo y su padre mediante `buscarPadre`. Devuelve `false` si
el ID no existe y `true` si logra eliminarlo. Se separan los tres casos:

1. **`eliminarCasoHoja`**: desconecta el nodo de su padre y lo libera. Si era la raiz,
   el arbol queda vacio.
2. **`eliminarCasoUnHijo`**: conecta el padre directamente con el unico hijo y libera el nodo.
   Si se elimina la raiz, ese hijo pasa a ser la nueva raiz.
3. **`eliminarCasoDosHijos`**: busca el menor ID del subarbol derecho, llamado sucesor.
   Copia toda su bicicleta al nodo que se quiere eliminar y luego elimina el nodo original
   del sucesor, que puede ser una hoja o tener un hijo derecho.

El auxiliar **`pPadreMenorMayores`** entra una vez a la derecha y luego avanza todo lo posible
a la izquierda, guardando el padre de cada nodo. Devuelve el padre del sucesor para poder
actualizar el enlace al quitarlo. Se llama cuando el nodo tiene dos hijos.

La eliminacion cuesta **O(h + 1)**: en el peor caso recorre caminos proporcionales a la altura.

### Liberacion de memoria

El constructor **`Abb()`** inicia la raiz en `nullptr`. Al finalizar la vida del arbol,
el destructor **`~Abb()`** llama a **`liberar(raiz)`**, que recorre en **postorder**:
primero libera el subarbol izquierdo, despues el derecho y por ultimo el nodo actual.
Este orden permite acceder a los hijos antes de borrar el nodo que guarda sus punteros.

La liberacion cuesta **O(n)** y utiliza **O(h + 1)** de memoria auxiliar por la recursion.
La copia y la asignacion de un ABB estan deshabilitadas para evitar que dos arboles compartan
los mismos nodos y los intenten liberar dos veces.

### Analisis: altura al cargar bicicletas ordenadas por ID

**Si se cargan `n` bicicletas con IDs distintos en orden ascendente, la altura es `n - 1`.**
Cada nueva bicicleta tiene un ID mayor que las anteriores y se agrega como hijo derecho
del ultimo nodo. El arbol queda como una cadena. En orden descendente ocurre lo mismo,
pero hacia la izquierda.

Por ejemplo, al insertar cinco bicicletas en este orden: **501, 502, 503, 504, 505**,
el arbol queda asi (se muestran solo los IDs):

```text
501          <- raiz, profundidad 0
   \
    502      <- profundidad 1
       \
        503  <- profundidad 2
           \
            504  <- profundidad 3
               \
                505  <- profundidad 4
```

Hay **5 nodos y 4 aristas** en el camino mas largo desde la raiz: la altura es **4**.
Para buscar la bicicleta 505 se visitan los cinco nodos: 501, 502, 503, 504 y 505.
Por eso la busqueda en el peor caso pasa a ser **O(n)**, en lugar de **O(log n)** como
en un arbol balanceado. 

Con las mismas bicicletas, pero insertadas en el orden **503, 501, 505, 502, 504**,
se obtiene una forma balanceada:

```text
        503
       /   \
     501   505
       \   /
       502 504
```

En este caso la altura es **2** y cualquier busqueda visita como maximo tres nodos.

