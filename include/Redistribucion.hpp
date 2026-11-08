#ifndef REDISTRIBUCION_HPP
#define REDISTRIBUCION_HPP

#include <string>
#include <vector>

/*Estructura para representar la estación y la cantidad
de bicicletas que le faltan*/
struct EstacionFaltante
{
    std::string codigo;
    int faltantes;
};

/*Función principal que inicia el algoritmo de backtracking*/
void resolverRedistribucion(const std::vector<EstacionFaltante> &estaciones, int capacidadC);

void mostrarMenuRedistribucion();

#endif // REDISTRIBUCION_HPP