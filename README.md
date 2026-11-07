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
