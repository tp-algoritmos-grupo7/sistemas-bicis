# Sistema de gestión de bicicletas

Sistema académico desarrollado en C++ para administrar una red de bicicletas urbana, con estaciones, viajes, bicicletas en mantenimiento, registro de stock y redistribución de bicicletas mediante backtracking.

## 🚲 Descripción general

El proyecto modela una red de estaciones con:

- gestión de estaciones y ciclovías,
- cálculo de recorridos mínimos por BFS,
- registro de bicicletas en un árbol binario de búsqueda,
- viajes con colas de espera y taller de reparaciones,
- redistribución de bicicletas mediante búsqueda exhaustiva.

El programa está pensado para ejecutarse desde la raíz del repositorio, donde se encuentran los archivos de datos en la carpeta `datos/`.

---

## ⚙️ Requisitos

- Compilador `g++`
- Estándar: `C++17`
- Sistema operativo compatible con `make` (o `mingw32-make` en Windows)

```bash
make
./sistema_bicis
```

> El proyecto se compila con `g++ -std=c++17 -Wall -Wextra`, y el `Makefile` agrega `-Werror -pedantic`.

---

## 🧱 Estructura del proyecto

```text
sistemas-bicis/
├── Makefile
├── README.md
├── sistema_bicis.exe
├── test_estaciones.exe
├── datos/
│   ├── bicicletas.txt
│   ├── ciclovias.txt
│   ├── estaciones.txt
│   └── viajes.txt
├── include/
│   ├── Abb.h
│   ├── Bicicleta.h
│   ├── Cola.hpp
│   ├── ColaEspera.hpp
│   ├── Estacion.hpp
│   ├── GestionEstaciones.hpp
│   ├── GestionViajes.hpp
│   ├── GrafoCiclovias.hpp
│   ├── Lista.hpp
│   ├── MenuBicicletas.h
│   ├── MinHeapTaller.hpp
│   ├── NodoAbb.h
│   ├── Redistribucion.hpp
│   ├── RegistroBicicleta.h
│   ├── TablaHash.hpp
│   ├── Viaje.hpp
│   └── ...
└── src/
    ├── Bicicleta.cpp
    ├── Estacion.cpp
    ├── GestionEstaciones.cpp
    ├── GestionViajes.cpp
    ├── GrafoCiclovias.cpp
    ├── MenuBicicletas.cpp
    ├── Redistribucion.cpp
    ├── RegistroBicicleta.cpp
    ├── TablaHash.cpp
    ├── Viaje.cpp
    ├── main.cpp
    └── structures/
        ├── Abb.cpp
        └── NodoAbb.cpp
```

---

## Modelo de clases

Vista simplificada del modelo: composición, dependencias y estructuras que usa cada módulo.

![Modelo de clases del sistema de bicicletas](<docs/Sistema-Bicis-UML-UML-general-simplificado.drawio (2).png>)

---

## A. Gestión de estaciones

### 📁 Archivos principales

| Archivo | Contenido |
|---|---|
| `Estacion.hpp/.cpp` | Clase `Estacion` con código, nombre, barrio, anclajes y bicicletas disponibles. |
| `Lista.hpp` | Lista enlazada genérica con iterador. |
| `Cola.hpp` | Cola FIFO genérica (`encolar` / `desencolar`) apoyada en `Lista`. |
| `TablaHash.hpp/.cpp` | Tabla de hash que indexa estaciones por código. |
| `GrafoCiclovias.hpp/.cpp` | Grafo de ciclovías con listas de adyacencia y búsqueda BFS. |
| `GestionEstaciones.hpp/.cpp` | Carga de archivos, ordenamiento por selección y menú de la parte A. |

### 🔐 Tabla de hash

- Función hash: suma de los códigos ASCII de los caracteres del código, módulo `M = 31` (primo).
- Resolución de colisiones: encadenamiento. Cada casillero es una `Lista<Estacion*>`.

Con los datos de prueba de `datos/estaciones.txt`:

| Código | Suma ASCII | Índice (`suma % 31`) |
|---|---:|---:|
| `PLZ` | 246 | 29 |
| `PAL` | 221 | 4 |
| `CAB` | 198 | 12 |
| `REC` | 218 | 1 |
| `BEL` | 211 | 25 |
| `SNT` | 245 | 28 |

- Colisiones: 0. Las 6 estaciones caen en casilleros distintos.
- Factor de carga final: `N / M = 6 / 31 ≈ 0.19`.

### 📊 Listado ordenado (ordenamiento por selección)

Se copian los punteros de las estaciones de la tabla a un arreglo auxiliar y se ordenan con selección: en cada vuelta se busca el mayor o menor entre los elementos restantes y se intercambia con la posición actual. El usuario eligió el criterio (capacidad de anclajes o bicicletas disponibles) y el sentido (mayor a menor o menor a mayor). La tabla hash no se modifica.

- Complejidad: `O(n²)` comparaciones en todos los casos.
- A lo sumo hay `n - 1` intercambios.

### 🗺️ Recorrido por ciclovías (BFS)

Las estaciones son los vértices del grafo y las ciclovías de `datos/ciclovias.txt` sus aristas bidireccionales. La estructura se guarda mediante listas de adyacencia: a cada estación se le asigna un índice y `adyacentes[i]` contiene los índices de sus vecinas.

La búsqueda en anchura recorre el grafo por niveles usando una `Cola`:

1. primero las estaciones a 1 tramo del origen,
2. luego las de 2 tramos,
3. así sucesivamente.

Por eso, la primera vez que se llega al destino, se obtiene el camino con menor cantidad de tramos. El arreglo `anterior[]` guarda desde dónde se llegó a cada estación y permite reconstruir el camino desde el destino hacia el origen. Si el destino nunca se visita, se informa que no existe recorrido.

Cuando se elimina una estación, también se borran sus ciclovías del grafo para que no se usen en recorridos.

- Complejidad: `O(V + E)` donde `V` es la cantidad de estaciones y `E` la cantidad de ciclovías.
- Se eligió lista de adyacencia porque el grafo es disperso: ocupa `O(V + E)` en memoria, en lugar de `O(V²)` que requeriría una matriz.

---

## B. Viajes, colas de espera y taller de reparaciones

### 📁 Archivos principales

| Archivo | Contenido |
|---|---|
| `Viaje.hpp/.cpp` | Clase `Viaje` con ID de viaje, ID de bici, DNI del usuario, origen, destino, fecha y duración. |
| `GestionViajes.hpp/.cpp` | Lógica de préstamos, devoluciones, colas de espera por estación, filtros por fecha y consultas por DNI. |
| `MinHeapTaller.hpp` | Cola de prioridad implementada a mano (Min-Heap) para la gestión del taller de reparaciones. |

### 🔍 Detalles de implementación y complejidad

- Consulta por DNI y minutos: recorre la lista de viajes con un iterador, filtra por DNI y acumula el total de minutos pedaleados. Complejidad: `O(n)`.
- Conteo por rango de fechas: permite calcular la cantidad de viajes originados en una estación específica durante un intervalo de tiempo en formato `AAAAMMDD`. Complejidad: `O(n)`.
- Bicicletas para revisión (más de X km): desde el menú de bicicletas (opción 7) se ingresa X y se recorre el ABB en inorden mostrando solo las que superan esos kilómetros, ordenadas por ID. Complejidad: `O(n)`, porque el árbol está ordenado por ID y no por kilómetros, así que hay que visitar todos los nodos.
- Cola de espera por estación (FIFO): cada estación administra una `Cola` independiente. Si un usuario solicita un préstamo y la estación no tiene stock, se encola automáticamente. Cuando llega una devolución, el sistema saca al primer usuario y le asigna la bicicleta devuelta al instante. Complejidad de encolar y desencolar: `O(1)`.
- Taller de reparaciones (Min-Heap): estructura basada en un arreglo dinámico con algoritmos internos de `flotar` y `hundir`. Las bicicletas averiadas se insertan con una prioridad numérica del 1 al 3; los vehículos eléctricos ingresan siempre con prioridad máxima (`1`). Inserción y extracción de la unidad más urgente: `O(log n)`.

### Préstamos y devoluciones conectados al registro

El préstamo pide DNI, estación de origen e ID de bicicleta. Valida que el ID exista en el ABB, que la bicicleta esté `OK`, que pertenezca al origen y que no esté prestada. Si se acepta, descuenta una disponible, marca la bicicleta en viaje y crea un viaje activo. Si el origen no tiene stock, encola al usuario automáticamente. La fecha se obtiene del reloj local del equipo y el ID es el siguiente al mayor de la lista, incluidos los viajes históricos.

La devolución pide el ID de la bicicleta, el destino real y la duración en minutos (no negativa). Debe existir un viaje activo y un anclaje libre, contando también las averiadas de la estación. Si falla una validación, el viaje sigue abierto y no se modifica el stock. Al aceptar, se cierra el viaje, se actualiza la estación de la bicicleta en el ABB y se suma una disponible si está `OK`. Los kilómetros se conservan: no se deducen de los minutos ni de los tramos del grafo.

Si hay usuarios esperando y la bicicleta está `OK`, se presta esa misma bicicleta al primero de la cola y se abre otro viaje, todavía sin destino. La devolución y el préstamo inmediato compensan el stock. El usuario se desencola después de aceptar el préstamo. Una bicicleta en viaje se muestra como `En viaje` en la consulta y no se puede volver a prestar ni eliminar.

Los registros cargados de `viajes.txt` son históricos finalizados, incluso si su duración es cero; no se reproducen sobre el stock inicial. Los viajes nuevos se agregan a ese archivo al finalizar, con destino y duración reales. Mientras están en curso viven en memoria, igual que los cambios de stock y ubicación: no se restauran sesiones pendientes al reiniciar. Si falla la escritura del histórico se avisa y el viaje finalizado se conserva en memoria.

La coordinación recorre la lista para buscar viajes activos o asignar IDs (`O(v)`). Buscar y actualizar una bicicleta cuesta `O(h + 1)`; comprobar anclajes requiere contar averiadas por estación (`O(b)`). `v` es la cantidad de viajes, `b` la de bicicletas y `h` la altura del ABB. El costo de las operaciones FIFO sigue siendo `O(1)`; el flujo completo de devolución incluye estas otras búsquedas.

### Taller conectado al registro y a las estaciones

El min-heap pertenece a `RegistroBicicletas`, cuya instancia vive en `main`. Salir del taller, volver al menú principal o entrar otra vez conserva las bicicletas pendientes durante la ejecución. Como los demás cambios de inventario, el taller vive en memoria y no se restaura al cerrar y volver a abrir el programa.

El ingreso pide ID y prioridad entre 1 y 3. El tipo se consulta en el ABB y las eléctricas reciben siempre prioridad 1. Se rechazan IDs inexistentes, bicicletas en viaje, ingresos repetidos y prioridades inválidas. Si se reporta una avería de una bicicleta `OK`, se descuenta una disponible de su estación; si ya estaba averiada, no se descuenta otra vez. En el ABB queda `Averiada` y `En taller`, sin ocupar un anclaje en una estación. Si su estación ya fue eliminada, se permite ingresarla sin modificar otra estación. El heap conserva su capacidad máxima de 100: si está lleno, se rechaza el ingreso sin modificar el registro ni el stock.

La opción de atención representa una reparación terminada y pide la estación de destino para la bicicleta más urgente. Primero consulta la raíz sin extraerla y valida que la estación exista y tenga lugar (incluyendo anclajes ocupados por averiadas). Si no se puede completar, conserva la bicicleta y su prioridad en el taller. Si se acepta, extrae del heap, actualiza el ABB a `OK` con la nueva estación y suma una disponible. Si hay usuarios esperando, esa misma bicicleta se presta al primero y el stock queda compensado, igual que en una devolución.

Mientras una bicicleta está en el taller no puede prestarse, darse de baja ni devolverse como si viniera de un viaje. Las consultas y listados del ABB muestran `En taller`; el conteo general de averiadas la incluye hasta su reparación, pero el conteo de anclajes de las estaciones la excluye.

Consultar la raíz cuesta `O(1)` e insertar/extraer del heap cuesta `O(log t)`, donde `t` es la cantidad en taller. El ingreso coordinado agrega búsquedas en el ABB (`O(h + 1)`) y en la tabla hash. La salida agrega el conteo de averiadas por estación (`O(b)`) y, si hay cola de espera, la asignación del viaje (`O(v)`). El heap evita ordenar todas las bicicletas cada vez que se atiende una reparación; la marca en el ABB permite detectar ingresos repetidos sin recorrer el heap.

---

## C. Registro de bicicletas con árbol binario de búsqueda

### 📁 Archivos principales

| Archivo | Contenido |
|---|---|
| `Bicicleta.h/.cpp` | Clase `Bicicleta` con ID, código de estación, tipo, estado y kilómetros. |
| `NodoAbb.h/.cpp` | Clase `Nodo`: guarda una bicicleta y los punteros a los hijos izquierdo y derecho. |
| `Abb.h/.cpp` | ABB propio con inserción, búsqueda, recorridos, conteos, altura, eliminación y liberación de memoria. |
| `RegistroBicicleta.h/.cpp` | Clase `RegistroBicicletas`: contiene el ABB, carga bicicletas desde archivo y ofrece operaciones al menú. |
| `MenuBicicletas.h/.cpp` | Lectura validada de enteros y menú para agregar, buscar, listar, contar averiadas, consultar altura y eliminar bicicletas. |
| `main.cpp` | Carga inicial de `datos/bicicletas.txt` y acceso al menú de bicicletas. |

### 🌳 Organización del ABB

Todas las bicicletas se guardan en un árbol binario de búsqueda ordenado por `biciId`. Cada nodo contiene una bicicleta completa, un puntero al hijo izquierdo y otro al derecho. La clase `Abb` guarda el puntero `raiz`, que vale `nullptr` si el árbol está vacío.

- En el subárbol izquierdo quedan los IDs menores que el del nodo.
- En el subárbol derecho quedan los IDs mayores.
- No se permiten IDs repetidos.
- Los nodos se crean con `new` y el árbol se encarga de liberarlos.

Al iniciar el programa, `RegistroBicicletas::cargarDesdeArchivo` lee `datos/bicicletas.txt` e inserta cada bicicleta en el ABB. La carga ocurre una sola vez, antes del menú principal. Los IDs duplicados se omiten y se informa el problema; las bicicletas ya cargadas se conservan.

La carga inicial no modifica `bicisDisponibles`: ese stock ya se leyó de `estaciones.txt`. Los archivos de ejemplo no constituyen un inventario completo por ID, por lo que no se recalcula el stock contando los nodos del ABB.

Las altas y bajas del menú se coordinan en `RegistroBicicletas`:

- El alta valida ID positivo y único, kilómetros no negativos, estación existente y espacio antes de insertar. Una bicicleta `OK` suma una disponible; una `Averiada` se registra sin aumentar las disponibles.
- Para comprobar el espacio en el alta se suman las disponibles de la estación y las averiadas que el ABB tiene asignadas a ella. Se considera que estas últimas siguen ocupando anclajes. El conteo de averiadas por estación recorre todo el árbol, por lo que el alta cuesta `O(n)` más la búsqueda en la tabla hash.
- La baja se rechaza si la bicicleta está en viaje o en el taller. Si está en una estación y `OK`, descuenta una disponible. Una averiada se elimina sin descontar disponibles. Si una bicicleta `OK` tiene estación existente pero stock cero, se rechaza la baja sin tocar el árbol.
- Si la estación fue eliminada, se permite borrar su bicicleta del ABB sin modificar ninguna otra estación, conforme a la excepción del TP sobre reubicación.
- Los rechazos informan el motivo y conservan ambas estructuras. En una baja con dos hijos se usa la estación y el estado de la bicicleta solicitada, no los del sucesor que copia el ABB.

Esta coordinación cubre carga inicial, altas, bajas, préstamos, devoluciones y taller.

### ➕ Inserción y búsqueda

Ambos métodos son iterativos: usan un puntero `actual` que comienza en la raíz y un ciclo `while` para avanzar por el árbol.

- `insertar(bicicleta)`: si el árbol está vacío, crea la raíz. En otro caso, compara el ID con el nodo actual. Si es menor, avanza a la izquierda; si es mayor, a la derecha. Cuando encuentra el lugar libre, crea el nodo y devuelve `true`. Si el ID ya existe, devuelve `false` sin modificar la bicicleta guardada.
- `buscar(biciId)`: compara el ID y avanza por el lado que puede contenerlo. Si lo encuentra, devuelve un puntero de lectura a la bicicleta; si llega a `nullptr`, devuelve `nullptr`.

El costo depende de la cantidad de nodos del camino recorrido: `O(h + 1)`, donde `h` es la altura del árbol. En un árbol balanceado es `O(log n)`; en el peor caso, si es una cadena, es `O(n)`. Estos métodos usan `O(1)` de memoria auxiliar.

### 🔁 Recorridos, conteos y altura

Estos métodos tienen una versión pública que inicia desde la raíz y una auxiliar privada que recibe un nodo. La auxiliar permite repetir la operación recursivamente en cada subárbol.

- `recorridoInorden()`: visita primero el subárbol izquierdo, luego muestra la bicicleta del nodo y finalmente visita el derecho. Así lista todas las bicicletas por ID ascendente.
- `contarNodos()`: devuelve `0` para un nodo nulo; para cada nodo existente, suma `1 + cantidadIzquierda + cantidadDerecha`.
- `contarBicicletasAveriadas()`: recorre todo el árbol y suma `1` si la bicicleta del nodo está averiada, más los resultados de ambos subárboles.
- `altura()`: calcula `1 + máximo(altura izquierda, altura derecha)`. La altura se mide en aristas: un árbol vacío tiene altura `-1`, y una única raíz tiene altura `0`.

Cada operación visita todos los nodos, por lo que cuesta `O(n)`. Las llamadas recursivas ocupan `O(h + 1)` de memoria auxiliar.

### 🗑️ Eliminación de bicicletas

`eliminar(biciId)` busca el nodo y su padre mediante `buscarPadre`. Devuelve `false` si el ID no existe y `true` si logra eliminarlo. Se consideran tres casos:

1. `eliminarCasoHoja`: desconecta el nodo de su padre y lo libera. Si era la raíz, el árbol queda vacío.
2. `eliminarCasoUnHijo`: conecta el padre directamente con el único hijo y libera el nodo. Si se elimina la raíz, ese hijo pasa a ser la nueva raíz.
3. `eliminarCasoDosHijos`: busca el menor ID del subárbol derecho, llamado sucesor. Copia toda su bicicleta al nodo que se quiere eliminar y luego elimina el nodo original del sucesor, que puede ser una hoja o tener un hijo derecho.

El auxiliar `pPadreMenorMayores` entra una vez a la derecha y luego avanza todo lo posible a la izquierda, guardando el padre de cada nodo. Devuelve el padre del sucesor para poder actualizar el enlace al quitarlo. Se llama cuando el nodo tiene dos hijos.

La eliminación cuesta `O(h + 1)`: en el peor caso, recorre caminos proporcionales a la altura.

### 🧹 Liberación de memoria

El constructor `Abb()` inicia la raíz en `nullptr`. Al finalizar la vida del árbol, el destructor `~Abb()` llama a `liberar(raiz)`, que recorre en postorder: primero libera el subárbol izquierdo, luego el derecho y por último el nodo actual. Este orden permite acceder a los hijos antes de borrar el nodo que guarda sus punteros.

La liberación cuesta `O(n)` y utiliza `O(h + 1)` de memoria auxiliar por la recursión. La copia y la asignación de un ABB están deshabilitadas para evitar que dos árboles compartan los mismos nodos y los intenten liberar dos veces.

### 📈 Análisis: altura al cargar bicicletas ordenadas por ID

Si se cargan `n` bicicletas con IDs distintos en orden ascendente, la altura es `n - 1`. Cada nueva bicicleta tiene un ID mayor que las anteriores y se agrega como hijo derecho del último nodo. El árbol queda como una cadena. En orden descendente ocurre lo mismo, pero hacia la izquierda.

Por ejemplo, al insertar cinco bicicletas en este orden: `501, 502, 503, 504, 505`, el árbol queda así:

```text
501          <- raíz, profundidad 0
   \
    502      <- profundidad 1
       \
        503  <- profundidad 2
           \
            504  <- profundidad 3
               \
                505  <- profundidad 4
```

Hay 5 nodos y 4 aristas en el camino más largo desde la raíz: la altura es 4. Para buscar la bicicleta 505 se visitan los cinco nodos: 501, 502, 503, 504 y 505. Por eso, la búsqueda en el peor caso pasa a ser `O(n)`, en lugar de `O(log n)` como en un árbol balanceado.

Con las mismas bicicletas, pero insertadas en el orden `503, 501, 505, 502, 504`, se obtiene una forma balanceada:

```text
        503
       /   \
     501   505
       \   /
       502 504
```

En este caso la altura es 2 y cualquier búsqueda visita como máximo tres nodos.

---

## D. Redistribución con backtracking

### 🧠 Implementación

- Algoritmo: se utiliza una solución basada en backtracking para recorrer el espacio de estados y hallar todos los subconjuntos de estaciones cuyos faltantes de bicicletas sumen exactamente la capacidad `C` especificada para el camión de redistribución.
- Complejidad temporal (peor caso): `O(2^n)`, donde `n` representa la cantidad de estaciones candidatas con faltantes, ya que el algoritmo evalúa recursivamente incluir o excluir cada estación del subconjunto.

Este enfoque permite explorar todas las combinaciones posibles y decidir, en cada rama del árbol de decisión, si una estación participa o no en la redistribución. Cuando se alcanza el valor objetivo, se registra la solución válida.

---

## ✅ Resumen

El proyecto combina varias estructuras de datos fundamentales de la programación:

- tablas hash,
- colas FIFO,
- grafos con BFS,
- heaps mínimos,
- árboles binarios de búsqueda,
- backtracking.

Esto permite gestionar de forma ordenada y eficiente una red de bicicletas con múltiples requerimientos de negocio, tanto operativos como analíticos.

---

## ▶️ Ejecución

Desde la raíz del proyecto:

```bash
make
./sistema_bicis
```

Si se trabaja en Windows con `mingw32`, la compilación puede reemplazarse por:

```bash
mingw32-make
sistema_bicis.exe
```
