#include <iostream>
#include <fstream>
#include <string>
#include "Estacion.hpp"
#include "Lista.hpp"

// Carga las estaciones desde el archivo e inserta en la lista enlazada
bool cargarEstaciones(const std::string& rutaArchivo, Lista<Estacion>& lista) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo abrir " << rutaArchivo << std::endl;
        return false;
    }

    std::string codigo, nombre, barrio;
    int capacidad, disponibles;

    while (archivo >> codigo >> nombre >> barrio >> capacidad >> disponibles) {
        Estacion e(codigo, nombre, barrio, capacidad, disponibles);
        lista.insertarAlFinal(e);
    }

    archivo.close();
    return true;
}

// Busca una estacion por su codigo 
// Retorna un puntero a la Estacion encontrada o nullptr si no existe.
Estacion* buscarEstacionPorCodigo(Lista<Estacion>& lista, const std::string& codigo) {
    auto it = lista.obtenerIterador();
    while (it.tieneSiguiente()) {
        if (it.obtenerDato().obtenerCodigo() == codigo) {
            return &(it.obtenerDato());
        }
        it.avanzar();
    }
    return nullptr;
}

// Muestra la informacion de una estacion dado su codigo
void mostrarEstacionPorCodigo(Lista<Estacion>& lista, const std::string& codigo) {
    Estacion* estacion = buscarEstacionPorCodigo(lista, codigo);
    if (estacion != nullptr) {
        std::cout << "\n[Estacion Encontrada]" << std::endl;
        estacion->mostrar();
    } else {
        std::cout << "\nError: No se encontro ninguna estacion con el codigo '" << codigo << "'." << std::endl;
    }
}

int main() {
    std::cout << "=== Sistema de Bicicletas Publicas UNAHUR ===" << std::endl;
    std::cout << "--- Carga de estaciones ---\n" << std::endl;

    Lista<Estacion> estaciones;

    if (!cargarEstaciones("datos/estaciones.txt", estaciones)) {
        return 1;
    }

    // Mostrar todas las estaciones cargadas usando el iterador del TDA
    auto it = estaciones.obtenerIterador();
    while (it.tieneSiguiente()) {
        it.obtenerDato().mostrar();
        it.avanzar();
    }

    std::cout << "\nTotal cargadas en la lista: " << estaciones.obtenerTamanio() << std::endl;

    // Item: Mostrar la informacion de una estacion dado su codigo
    std::cout << "\n--- Consulta de Estacion por Codigo ---" << std::endl;
    std::cout << "Ingrese el codigo de la estacion a consultar (o 'FIN' para salir): ";
    
    std::string codigoBuscado;
    while (std::cin >> codigoBuscado && codigoBuscado != "FIN" && codigoBuscado != "fin") {
        mostrarEstacionPorCodigo(estaciones, codigoBuscado);
        std::cout << "\nIngrese otro codigo (o 'FIN' para salir): ";
    }

    std::cout << "\nPrograma finalizado." << std::endl;
    return 0;
}