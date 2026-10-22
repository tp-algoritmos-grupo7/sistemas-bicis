#include "Redistribucion.hpp"
#include <iostream>
#include <limits>

/*Función recursiva auxiliar de backtracking para explorar las combinaciones*/
static void backtrackingRedistribucion(const std::vector<EstacionFaltante> &estaciones,
                                       int capacidadCamion,
                                       size_t indiceActual,
                                       int sumaActual,
                                       std::vector<std::string> &caminoActual,
                                       bool &encontradoAlguna)
{
    // Si la suma actual supera la capacidad del camión, cortamos esta rama
    if (sumaActual > capacidadCamion)
    {
        return;
    }

    // CASO BASE: Si la suma actual es exactamente igual a la capacidad C
    if (sumaActual == capacidadCamion)
    {
        std::cout << "{ ";
        for (size_t i = 0; i < caminoActual.size(); ++i)
        {
            std::cout << caminoActual[i] << (i + 1 < caminoActual.size() ? ", " : "");
        }
        std::cout << " }\n";
        encontradoAlguna = true;
        return;
    }

    // Si llegamos al final del vector de estaciones sin alcanzar C, retornamos
    if (indiceActual == estaciones.size())
    {
        return;
    }

    // ELECCIÓN: Incluir la estación actual en el subconjunto
    caminoActual.push_back(estaciones[indiceActual].codigo);
    backtrackingRedistribucion(estaciones, capacidadCamion, indiceActual + 1,
                               sumaActual + estaciones[indiceActual].faltantes, caminoActual, encontradoAlguna);

    // BACKTRACKING: Deshacer la elección (quitar la estación para probar la siguiente rama)
    caminoActual.pop_back();

    // ALTERNATIVA: Excluir la estación actual y pasar a la siguiente
    backtrackingRedistribucion(estaciones, capacidadCamion, indiceActual + 1,
                               sumaActual, caminoActual, encontradoAlguna);
}

void resolverRedistribucion(const std::vector<EstacionFaltante> &estaciones, int capacidadC)
{
    std::cout << "\nREDISTRIBUCION DE BICICLETAS (Capacidad C = " << capacidadC << ") ===\n";

    std::vector<std::string> caminoActual;
    bool encontradoAlguna = false;

    backtrackingRedistribucion(estaciones, capacidadC, 0, 0, caminoActual, encontradoAlguna);

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
    if (!leerEnteroValidado("Ingrese la capacidad C del camion: ", capacidadC))
    {
        return;
    }

    int cantidadEstaciones = 0;
    if (!leerEnteroValidado("Ingrese la cantidad de estaciones a evaluar: ", cantidadEstaciones) || cantidadEstaciones <= 0)
    {
        std::cout << "Cantidad invalida.\n";
        return;
    }

    std::vector<EstacionFaltante> estacionesFaltantes;
    for (int i = 0; i < cantidadEstaciones; ++i)
    {
        EstacionFaltante est;
        std::cout << "\nEstacion #" << (i + 1) << "\n";
        std::cout << "  Ingrese el codigo de la estacion (ej. PLZ): ";
        std::cin >> est.codigo;

        // Limpiamos el buffer para que el Enter del código no interfiera con la lectura numérica
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (!leerEnteroValidado("  Ingrese bicicletas faltantes: ", est.faltantes) || est.faltantes < 0)
        {
            std::cout << "Valor invalido. Se cancela la operacion.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
        estacionesFaltantes.push_back(est);
    }

    // Ejecutar el algoritmo de backtracking con los datos ingresados
    resolverRedistribucion(estacionesFaltantes, capacidadC);
}