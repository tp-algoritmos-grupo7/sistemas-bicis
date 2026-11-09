#include <iostream>
#include <limits>
#include "TablaHash.hpp"
#include "GestionEstaciones.hpp"
#include "GestionViajes.hpp"
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

    // Cargar tu lista de viajes (Parte B) - LLAMADA CORRECTA A cargarViajes
    Lista<Viaje> listaViajes;
    if (!cargarViajes("datos/viajes.txt", listaViajes))
    {
        std::cerr << "Advertencia: no se pudo cargar el archivo de viajes.\n";
    }

    // Cargar bicicletas de dev
    if (!registroBicicletas.cargarDesdeArchivo("datos/bicicletas.txt", std::cerr))
    {
        std::cerr << "Advertencia: carga de bicicletas con incidencias. Las bicicletas ya incorporadas se conservan.\n";
    }

    // Cargar ciclovías de dev
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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        case 2:
            // ¡Acá está tu módulo de viajes integrado con dev!
            ejecutarMenuViajes(listaViajes, tablaEstaciones);
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