#include <iostream>
#include "TablaHash.hpp"
#include "GestionEstaciones.hpp"
#include "GestionViajes.hpp"

int main() {
    TablaHash tablaEstaciones(31);

    if (!cargarEstaciones("datos/estaciones.txt", tablaEstaciones)) {
        std::cerr << "Advertencia: no se pudo cargar el archivo de estaciones.\n";
    }

    Lista<Viaje> listaViajes;
    if (!cargarViajes("datos/viajes.txt", listaViajes)) {
    std::cerr << "Advertencia: no se pudo cargar el archivo de viajes.\n";
    }

    int opcion = -1;
    while (opcion != 0) {
        std::cout << "\n============================================\n";
        std::cout << "  SISTEMA DE BICICLETAS PUBLICAS (UNAHUR)\n";
        std::cout << "============================================\n";
        std::cout << "1. Gestion de Estaciones (Parte A)\n";
        std::cout << "2. Viajes y Colas de Espera (Parte B)\n";
        std::cout << "3. Registro de Bicicletas con ABB (Parte C)\n";
        std::cout << "4. Redistribucion con Backtracking (Parte D)\n";
        std::cout << "0. Salir\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                ejecutarMenuEstaciones(tablaEstaciones);
                break;
            case 2:
                ejecutarMenuViajes(listaViajes, tablaEstaciones);
                break;
            case 3:
                std::cout << "\n[Aviso: Parte C en desarrollo]\n";
                break;
            case 4:
                std::cout << "\n[Aviso: Parte D en desarrollo]\n";
                break;
            case 0:
                std::cout << "\nGracias por usar el Sistema de Bicicletas Publicas. Hasta luego!\n";
                break;
            default:
                std::cout << "\nOpcion invalida. Ingrese una opcion entre 0 y 4.\n";
                break;
        }
    }

    return 0;
}