#include <iostream>
#include <limits>
#include "TablaHash.hpp"
#include "GestionEstaciones.hpp"
#include "MenuBicicletas.h"
#include "Redistribucion.hpp"

int main()
{
    TablaHash tablaEstaciones(31);
    RegistroBicicletas registroBicicletas;

    if (!cargarEstaciones("datos/estaciones.txt", tablaEstaciones))
    {
        std::cerr << "Advertencia: no se pudo cargar el archivo de estaciones.\n";
    }

    // El grafo se arma con las estaciones ya cargadas y despues se le agregan las ciclovias
    GrafoCiclovias grafoCiclovias(tablaEstaciones);
    if (!grafoCiclovias.cargarDesdeArchivo("datos/ciclovias.txt"))
    {
        std::cerr << "Advertencia: no se pudo cargar el archivo de ciclovias.\n";
    }

    int opcion = -1;
    while (opcion != 0)
    {
        std::cout << "\n============================================\n";
        std::cout << "  SISTEMA DE BICICLETAS PUBLICAS (UNAHUR)\n";
        std::cout << "============================================\n";
        std::cout << "1. Gestion de Estaciones (Parte A)\n";
        std::cout << "2. Viajes y Colas de Espera (Parte B)\n";
        std::cout << "3. Registro de Bicicletas con ABB (Parte C)\n";
        std::cout << "4. Redistribucion con Backtracking (Parte D)\n";
        std::cout << "0. Salir\n";
        if (!leerEnteroValidado("Seleccione una opcion: ", opcion))
        {
            break;
        }

        switch (opcion)
        {
        case 1:
            ejecutarMenuEstaciones(tablaEstaciones, grafoCiclovias);
            // El menu de estaciones deja pendiente el salto de linea.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        case 2:
            std::cout << "\n[Aviso: Parte B en desarrollo]\n";
            break;
        case 3:
            mostrarMenuBicicletas(registroBicicletas, tablaEstaciones);
            break;
        case 4:
            mostrarMenuRedistribucion();
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
