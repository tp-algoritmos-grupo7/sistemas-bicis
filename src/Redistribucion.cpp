#include "Redistribucion.hpp"
#include <iostream>
#include <limits>

/* Función recursiva auxiliar de backtracking utilizando arreglos dinámicos puros */
static void backtrackingRedistribucion(const EstacionFaltante *estaciones,
                                       int cantidadEstaciones,
                                       int capacidadCamion,
                                       int indiceActual,
                                       int sumaActual,
                                       std::string *caminoActual,
                                       int tamanioCamino,
                                       bool &encontradoAlguna)
{
    // Si la suma actual supera la capacidad del camión, cortamos esta rama (poda)
    if (sumaActual > capacidadCamion)
    {
        return;
    }

    // CASO BASE: Si la suma actual es exactamente igual a la capacidad C
    if (sumaActual == capacidadCamion)
    {
        std::cout << "{ ";
        for (int i = 0; i < tamanioCamino; ++i)
        {
            std::cout << caminoActual[i] << (i + 1 < tamanioCamino ? ", " : "");
        }
        std::cout << " }\n";
        encontradoAlguna = true;
        return;
    }

    // Si llegamos al final del arreglo de estaciones sin alcanzar C, retornamos
    if (indiceActual == cantidadEstaciones)
    {
        return;
    }

    // ELECCIÓN: Incluir la estación actual en el subconjunto (solo si tiene faltantes > 0)
    if (estaciones[indiceActual].faltantes > 0)
    {
        caminoActual[tamanioCamino] = estaciones[indiceActual].codigo;
        backtrackingRedistribucion(estaciones, cantidadEstaciones, capacidadCamion,
                                   indiceActual + 1,
                                   sumaActual + estaciones[indiceActual].faltantes,
                                   caminoActual, tamanioCamino + 1, encontradoAlguna);
    }

    // ALTERNATIVA: Excluir la estación actual (o pasar si su faltante era 0) y avanzar a la siguiente
    backtrackingRedistribucion(estaciones, cantidadEstaciones, capacidadCamion,
                               indiceActual + 1,
                               sumaActual,
                               caminoActual, tamanioCamino, encontradoAlguna);
}

void resolverRedistribucion(const EstacionFaltante *estaciones, int cantidadEstaciones, int capacidadC)
{
    std::cout << "\n=== REDISTRIBUCION DE BICICLETAS (Capacidad C = " << capacidadC << ") ===\n";

    // Creamos un arreglo dinamico propio para el camino en lugar de std::vector
    std::string *caminoActual = new std::string[cantidadEstaciones];
    bool encontradoAlguna = false;

    backtrackingRedistribucion(estaciones, cantidadEstaciones, capacidadC, 0, 0, caminoActual, 0, encontradoAlguna);

    // Liberamos la memoria del arreglo
    delete[] caminoActual;

    if (!encontradoAlguna)
    {
        std::cout << "No se encontro ningun subconjunto de estaciones cuyos faltantes sumen exactamente C.\n";
    }
}

extern bool leerEnteroValidado(const std::string &mensaje, int &destino);

void mostrarMenuRedistribucion()
{
    std::cout << "\n=== REDISTRIBUCION DE BICICLETAS ===" << std::endl;
    int capacidadC = 0;
    if (!leerEnteroValidado("Ingrese la capacidad C del camion: ", capacidadC) || capacidadC <= 0)
    {
        std::cout << "Capacidad invalida. Debe ser mayor a 0.\n";
        return;
    }

    int cantidadEstaciones = 0;
    if (!leerEnteroValidado("Ingrese la cantidad de estaciones a evaluar: ", cantidadEstaciones) || cantidadEstaciones <= 0)
    {
        std::cout << "Cantidad invalida.\n";
        return;
    }

    EstacionFaltante *estacionesFaltantes = new EstacionFaltante[cantidadEstaciones];
    int estacionesConFaltantes = 0;

    for (int i = 0; i < cantidadEstaciones; ++i)
    {
        std::cout << "\nEstacion #" << (i + 1) << "\n";
        std::cout << "  Ingrese el codigo de la estacion (ej. PLZ): ";
        std::cin >> estacionesFaltantes[i].codigo;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        int faltantes = 0;
        if (!leerEnteroValidado("  Ingrese bicicletas faltantes (>= 0): ", faltantes) || faltantes < 0)
        {
            std::cout << "Valor invalido. Se cancela la operacion.\n";
            delete[] estacionesFaltantes;
            return;
        }

        estacionesFaltantes[i].faltantes = faltantes;
        if (faltantes > 0)
        {
            estacionesConFaltantes++;
        }
    }

    if (estacionesConFaltantes == 0)
    {
        std::cout << "\nAviso: No hay estaciones con bicicletas faltantes (> 0) para redistribuir.\n";
        delete[] estacionesFaltantes;
        return;
    }

    // Ejecutar el algoritmo de backtracking con el arreglo dinámico
    resolverRedistribucion(estacionesFaltantes, cantidadEstaciones, capacidadC);

    // Liberar la memoria del arreglo principal
    delete[] estacionesFaltantes;
}