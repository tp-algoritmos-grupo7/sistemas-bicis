#include <iostream>
#include <fstream>
#include <string>
#include "Estacion.hpp"

int main() {
    std::cout << "=== Sistema de Bicicletas Publicas UNAHUR ===" << std::endl;
    std::cout << "--- Carga de estaciones ---\n" << std::endl;

    std::ifstream archivo("datos/estaciones.txt");
    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo abrir datos/estaciones.txt" << std::endl;
        return 1;
    }

    std::string codigo, nombre, barrio;
    int capacidad, disponibles;
    int contador = 0;

    while (archivo >> codigo >> nombre >> barrio >> capacidad >> disponibles) {
        Estacion e(codigo, nombre, barrio, capacidad, disponibles);
        e.mostrar();
        contador++;
    }

    archivo.close();
    std::cout << "\nTotal procesadas: " << contador << std::endl;
    return 0;
}