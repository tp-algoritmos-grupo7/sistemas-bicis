#include "GestionEstaciones.hpp"
#include "MenuBicicletas.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Lee una linea completa y exige una sola palabra (codigos, nombres y barrios no llevan espacios).
// Devuelve false si termina la entrada.
static bool leerPalabra(const std::string& mensaje, std::string& palabra) {
    while (true) {
        std::cout << mensaje;

        std::string linea;
        if (!std::getline(std::cin, linea)) {
            return false;
        }

        std::istringstream entrada(linea);
        std::string leida, sobrante;
        if ((entrada >> leida) && !(entrada >> sobrante)) {
            palabra = leida;
            return true;
        }

        std::cout << "Entrada invalida. Ingrese una sola palabra, sin espacios.\n";
    }
}

bool cargarEstaciones(const std::string& rutaArchivo, TablaHash& tabla) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo abrir " << rutaArchivo << std::endl;
        return false;
    }

    std::string codigo, nombre, barrio;
    int capacidad, disponibles;

    while (archivo >> codigo >> nombre >> barrio >> capacidad >> disponibles) {
        Estacion* e = new Estacion(codigo, nombre, barrio, capacidad, disponibles);
        if (!tabla.insertar(e)) {
            delete e;
        }
    }

    archivo.close();
    return true;
}

void mostrarEstacionPorCodigo(const TablaHash& tabla, const std::string& codigo) {
    Estacion* estacion = tabla.buscar(codigo);
    if (estacion != nullptr) {
        std::cout << "\n[Estacion Encontrada]\n";
        estacion->mostrar();
    } else {
        std::cout << "\nError: No se encontro ninguna estacion con el codigo '" << codigo << "'.\n";
    }
}

void agregarEstacionInteractivo(TablaHash& tabla, GrafoCiclovias& grafo) {
    std::string codigo, nombre, barrio;
    int capacidad = 0;
    int disponibles = -1;

    std::cout << "\n--- Agregar Nueva Estacion ---\n";
    if (!leerPalabra("Codigo (ej. RET): ", codigo)) {
        return;
    }

    if (tabla.buscar(codigo) != nullptr) {
        std::cout << "Error: Ya existe una estacion con el codigo '" << codigo << "'.\n";
        return;
    }

    if (!leerPalabra("Nombre: ", nombre) || !leerPalabra("Barrio: ", barrio)) {
        return;
    }

    if (!leerEnteroValidado("Capacidad de anclajes: ", capacidad)) {
        return;
    }
    while (capacidad <= 0) {
        if (!leerEnteroValidado("La capacidad debe ser mayor a 0. Ingrese nuevamente: ", capacidad)) {
            return;
        }
    }

    if (!leerEnteroValidado("Bicicletas disponibles: ", disponibles)) {
        return;
    }
    while (disponibles < 0 || disponibles > capacidad) {
        std::string mensaje = "Debe estar entre 0 y " + std::to_string(capacidad) + ". Ingrese nuevamente: ";
        if (!leerEnteroValidado(mensaje, disponibles)) {
            return;
        }
    }

    Estacion* nueva = new Estacion(codigo, nombre, barrio, capacidad, disponibles);
    if (tabla.insertar(nueva)) {
        // La estacion entra al grafo sin ciclovias, asi el BFS la reconoce
        grafo.agregarEstacion(codigo);
        std::cout << "\nEstacion '" << codigo << "' agregada con exito.\n";
    } else {
        std::cout << "\nError al insertar en la tabla.\n";
        delete nueva;
    }
}

void eliminarEstacionInteractivo(TablaHash& tabla, GrafoCiclovias& grafo) {
    std::string codigo;
    std::cout << "\n--- Eliminar Estacion Existente ---\n";
    if (!leerPalabra("Ingrese el codigo de la estacion a eliminar: ", codigo)) {
        return;
    }

    if (tabla.eliminar(codigo)) {
        // Sin la estacion, sus ciclovias tampoco se pueden usar en los recorridos
        grafo.quitarEstacion(codigo);
        std::cout << "\nEstacion '" << codigo << "' eliminada correctamente.\n";
    } else {
        std::cout << "\nError: No se encontro ninguna estacion con el codigo '" << codigo << "'.\n";
    }
}

// Devuelve el valor de la estacion segun el criterio elegido
static int obtenerValorCriterio(const Estacion* estacion, int criterio) {
    if (criterio == ORDEN_POR_CAPACIDAD) {
        return estacion->obtenerCapacidadAnclajes();
    }
    return estacion->obtenerBicisDisponibles();
}

void ordenarPorSeleccion(Estacion** estaciones, size_t cantidad, int criterio, bool descendente) {
    // En cada vuelta se busca la estacion que va en la posicion i
    // (la mayor si es descendente, la menor si es ascendente) entre las que faltan ordenar
    for (size_t i = 0; i + 1 < cantidad; i++) {
        size_t elegida = i;
        for (size_t j = i + 1; j < cantidad; j++) {
            int valorJ = obtenerValorCriterio(estaciones[j], criterio);
            int valorElegida = obtenerValorCriterio(estaciones[elegida], criterio);

            bool vaAntes;
            if (descendente) {
                vaAntes = valorJ > valorElegida;
            } else {
                vaAntes = valorJ < valorElegida;
            }
            if (vaAntes) {
                elegida = j;
            }
        }

        // Se intercambia la elegida con la que estaba en la posicion i
        if (elegida != i) {
            Estacion* aux = estaciones[i];
            estaciones[i] = estaciones[elegida];
            estaciones[elegida] = aux;
        }
    }
}

void listarEstacionesOrdenadas(const TablaHash& tabla) {
    size_t cantidad = tabla.obtenerCantidad();
    if (cantidad == 0) {
        std::cout << "\nNo hay estaciones cargadas.\n";
        return;
    }

    int criterio = 0;
    std::cout << "\n--- Listar Estaciones Ordenadas ---\n";
    std::cout << "Ordenar por:\n";
    std::cout << "1. Capacidad de anclajes\n";
    std::cout << "2. Bicicletas disponibles\n";
    if (!leerEnteroValidado("Opcion: ", criterio)) {
        return;
    }
    while (criterio != ORDEN_POR_CAPACIDAD && criterio != ORDEN_POR_DISPONIBLES) {
        if (!leerEnteroValidado("Opcion no valida. Ingrese 1 o 2: ", criterio)) {
            return;
        }
    }

    int sentido = 0;
    std::cout << "Sentido:\n";
    std::cout << "1. De mayor a menor\n";
    std::cout << "2. De menor a mayor\n";
    if (!leerEnteroValidado("Opcion: ", sentido)) {
        return;
    }
    while (sentido != 1 && sentido != 2) {
        if (!leerEnteroValidado("Opcion no valida. Ingrese 1 o 2: ", sentido)) {
            return;
        }
    }

    // Se copian los punteros a un arreglo auxiliar para ordenar sin tocar la tabla hash
    Estacion** estaciones = new Estacion*[cantidad];
    tabla.obtenerTodas(estaciones);
    ordenarPorSeleccion(estaciones, cantidad, criterio, sentido == 1);

    std::cout << "\nEstaciones ordenadas por "
              << (criterio == ORDEN_POR_CAPACIDAD ? "capacidad de anclajes" : "bicicletas disponibles")
              << " (" << (sentido == 1 ? "mayor a menor" : "menor a mayor") << "):\n";
    for (size_t i = 0; i < cantidad; i++) {
        std::cout << (i + 1) << ". ";
        estaciones[i]->mostrar();
    }

    delete[] estaciones;
}

void buscarRecorridoInteractivo(const TablaHash& tabla, const GrafoCiclovias& grafo) {
    std::string origen, destino;
    std::cout << "\n--- Recorrido por Ciclovias (BFS) ---\n";
    if (!leerPalabra("Codigo de la estacion de origen: ", origen) ||
        !leerPalabra("Codigo de la estacion de destino: ", destino)) {
        return;
    }

    if (tabla.buscar(origen) == nullptr) {
        std::cout << "\nError: No existe la estacion '" << origen << "'.\n";
        return;
    }
    if (tabla.buscar(destino) == nullptr) {
        std::cout << "\nError: No existe la estacion '" << destino << "'.\n";
        return;
    }

    Lista<std::string> recorrido;
    int tramos = grafo.buscarRecorrido(origen, destino, recorrido);

    if (tramos == -1) {
        std::cout << "\nNo existe un recorrido por ciclovias entre "
                  << origen << " y " << destino << ".\n";
        return;
    }

    std::cout << "\nRecorrido con menos tramos (" << tramos << " tramo"
              << (tramos == 1 ? "" : "s") << "):\n";
    auto it = recorrido.obtenerIterador();
    bool primera = true;
    while (it.tieneSiguiente()) {
        if (!primera) {
            std::cout << " -> ";
        }
        std::cout << it.siguiente();
        primera = false;
    }
    std::cout << "\n";
}

void ejecutarMenuEstaciones(TablaHash& tabla, GrafoCiclovias& grafo) {
    int opcion = -1;
    while (opcion != 0) {
        std::cout << "\n----------------------------------------\n";
        std::cout << "   PARTE A: GESTION DE ESTACIONES\n";
        std::cout << "----------------------------------------\n";
        std::cout << "1. Consultar informacion de una estacion por codigo\n";
        std::cout << "2. Agregar una estacion nueva al sistema\n";
        std::cout << "3. Eliminar una estacion existente\n";
        std::cout << "4. Listar estaciones ordenadas (anclajes o bicis disponibles)\n";
        std::cout << "5. Listar estaciones con menos del 20% de ocupacion\n";
        std::cout << "6. Buscar recorrido por ciclovias entre dos estaciones (BFS)\n";
        std::cout << "7. Ver metricas de la Tabla Hash\n";
        std::cout << "8. Ver estructura interna de la Tabla Hash\n";
        std::cout << "0. Volver al Menu Principal\n";
        if (!leerEnteroValidado("Seleccione una opcion: ", opcion)) {
            return;
        }

        if (opcion == 1) {
            std::string cod;
            if (leerPalabra("\nIngrese codigo de la estacion a consultar: ", cod)) {
                mostrarEstacionPorCodigo(tabla, cod);
            }
        } else if (opcion == 2) {
            agregarEstacionInteractivo(tabla, grafo);
        } else if (opcion == 3) {
            eliminarEstacionInteractivo(tabla, grafo);
        } else if (opcion == 4) {
            listarEstacionesOrdenadas(tabla);
        } else if (opcion == 5) {
            tabla.listarOcupacionBaja(20.0);
        } else if (opcion == 6) {
            buscarRecorridoInteractivo(tabla, grafo);
        } else if (opcion == 7) {
            std::cout << "\n--- Metricas de la Tabla Hash ---\n";
            std::cout << "Cantidad de estaciones (N):  " << tabla.obtenerCantidad() << "\n";
            std::cout << "Capacidad de la tabla (M):    " << tabla.obtenerCapacidad() << "\n";
            std::cout << "Factor de carga (alpha=N/M): " << tabla.obtenerFactorCarga() << "\n";
            std::cout << "Colisiones detectadas:        " << tabla.obtenerColisiones() << "\n";
        } else if (opcion == 8) {
            tabla.imprimirTabla(false);
        } else if (opcion == 0) {
            std::cout << "\nVolviendo al menu principal...\n";
        } else {
            std::cout << "\nOpcion no valida. Intente de nuevo.\n";
        }
    }
}
