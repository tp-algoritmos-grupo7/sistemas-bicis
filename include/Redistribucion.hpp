#ifndef REDISTRIBUCION_HPP
#define REDISTRIBUCION_HPP

#include <string>

/* Estructura para representar la estación y la cantidad de bicicletas que le faltan */
struct EstacionFaltante
{
    std::string codigo;
    int faltantes;
};

/* Función principal que inicia el algoritmo de backtracking con arreglos dinámicos */
void resolverRedistribucion(const EstacionFaltante *estaciones, int cantidadEstaciones, int capacidadC);

void mostrarMenuRedistribucion();

#endif // REDISTRIBUCION_HPP