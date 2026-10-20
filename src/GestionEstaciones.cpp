#include "GestionEstaciones.hpp"
#include <iostream>
#include <fstream>

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

void agregarEstacionInteractivo(TablaHash& tabla) {
    std::string codigo, nombre, barrio;
    int capacidad, disponibles;

    std::cout << "\n--- Agregar Nueva Estacion ---\n";
    std::cout << "Codigo (ej. RET): ";
    std::cin >> codigo;

    if (tabla.buscar(codigo) != nullptr) {
        std::cout << "Error: Ya existe una estacion con el codigo '" << codigo << "'.\n";
        return;
    }

    std::cout << "Nombre: ";
    std::cin >> nombre;
    std::cout << "Barrio: ";
    std::cin >> barrio;
    
    std::cout << "Capacidad de anclajes: ";
    std::cin >> capacidad;
    while (capacidad <= 0) {
        std::cout << "La capacidad debe ser mayor a 0. Ingrese nuevamente: ";
        std::cin >> capacidad;
    }

    std::cout << "Bicicletas disponibles: ";
    std::cin >> disponibles;
    while (disponibles < 0 || disponibles > capacidad) {
        std::cout << "Debe estar entre 0 y " << capacidad << ". Ingrese nuevamente: ";
        std::cin >> disponibles;
    }

    Estacion* nueva = new Estacion(codigo, nombre, barrio, capacidad, disponibles);
    if (tabla.insertar(nueva)) {
        std::cout << "\nEstacion '" << codigo << "' agregada con exito.\n";
    } else {
        std::cout << "\nError al insertar en la tabla.\n";
        delete nueva;
    }
}

void eliminarEstacionInteractivo(TablaHash& tabla) {
    std::string codigo;
    std::cout << "\n--- Eliminar Estacion Existente ---\n";
    std::cout << "Ingrese el codigo de la estacion a eliminar: ";
    std::cin >> codigo;

    if (tabla.eliminar(codigo)) {
        std::cout << "\nEstacion '" << codigo << "' eliminada correctamente.\n";
    } else {
        std::cout << "\nError: No se encontro ninguna estacion con el codigo '" << codigo << "'.\n";
    }
}

void ejecutarMenuEstaciones(TablaHash& tabla) {
    int opcion = -1;
    while (opcion != 0) {
        std::cout << "\n----------------------------------------\n";
        std::cout << "   PARTE A: GESTION DE ESTACIONES\n";
        std::cout << "----------------------------------------\n";
        std::cout << "1. Consultar informacion de una estacion por codigo\n";
        std::cout << "2. Agregar una estacion nueva al sistema\n";
        std::cout << "3. Eliminar una estacion existente\n";
        std::cout << "4. Listar estaciones con menos del 20% de ocupacion\n";
        std::cout << "5. Ver metricas de la Tabla Hash\n";
        std::cout << "6. Ver estructura interna de la Tabla Hash\n";
        std::cout << "0. Volver al Menu Principal\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;

        if (opcion == 1) {
            std::string cod;
            std::cout << "\nIngrese codigo de la estacion a consultar: ";
            std::cin >> cod;
            mostrarEstacionPorCodigo(tabla, cod);
        } else if (opcion == 2) {
            agregarEstacionInteractivo(tabla);
        } else if (opcion == 3) {
            eliminarEstacionInteractivo(tabla);
        } else if (opcion == 4) {
            tabla.listarOcupacionBaja(20.0);
        } else if (opcion == 5) {
            std::cout << "\n--- Metricas de la Tabla Hash ---\n";
            std::cout << "Cantidad de estaciones (N):  " << tabla.obtenerCantidad() << "\n";
            std::cout << "Capacidad de la tabla (M):    " << tabla.obtenerCapacidad() << "\n";
            std::cout << "Factor de carga (alpha=N/M): " << tabla.obtenerFactorCarga() << "\n";
            std::cout << "Colisiones detectadas:        " << tabla.obtenerColisiones() << "\n";
        } else if (opcion == 6) {
            tabla.imprimirTabla(false);
        } else if (opcion == 0) {
            std::cout << "\nVolviendo al menu principal...\n";
        } else {
            std::cout << "\nOpcion no valida. Intente de nuevo.\n";
        }
    }
}
