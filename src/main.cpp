#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Estacion.hpp"

bool leerLineaEstacion(const std::string& linea, Estacion& salida) {
    std::istringstream iss(linea);
    std::string codigo;
    if (!(iss >> codigo)) return false;

    std::string resto;
    std::getline(iss, resto);
    if (resto.empty()) return false;

    std::istringstream issResto(resto);
    std::string palabras[20];
    int cantidad = 0;
    while (issResto >> palabras[cantidad] && cantidad < 20) {
        cantidad++;
    }

    if (cantidad < 4) return false;

    int bicisDisponibles = std::stoi(palabras[cantidad - 1]);
    int capacidadAnclajes = std::stoi(palabras[cantidad - 2]);
    std::string barrio = palabras[cantidad - 3];

    std::string nombre = "";
    for (int i = 0; i < cantidad - 3; ++i) {
        if (i > 0) nombre += " ";
        nombre += palabras[i];
    }

    salida = Estacion(codigo, nombre, barrio, capacidadAnclajes, bicisDisponibles);
    return true;
}

int main() {
    std::cout << "=== Sistema de Bicicletas Publicas UNAHUR ===" << std::endl;
    std::cout << "--- Carga de estaciones desde archivo ---\n" << std::endl;

    std::ifstream archivo("datos/estaciones.txt");
    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo abrir datos/estaciones.txt" << std::endl;
        return 1;
    }

    std::string linea;
    int contador = 0;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;
        Estacion e;
        if (leerLineaEstacion(linea, e)) {
            e.mostrar();
            contador++;
        }
    }
    archivo.close();

    std::cout << "\nTotal procesadas: " << contador << std::endl;
    return 0;
}