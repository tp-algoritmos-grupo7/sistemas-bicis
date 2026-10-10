#ifndef ESTACION_HPP
#define ESTACION_HPP

#include <string>
#include <iostream>

class Estacion {
private:
    std::string codigo;
    std::string nombre;
    std::string barrio;
    int capacidadAnclajes;
    int bicisDisponibles;

public:
    Estacion();
    Estacion(const std::string& codigo, const std::string& nombre, 
             const std::string& barrio, int capacidadAnclajes, int bicisDisponibles);

    std::string obtenerCodigo() const;
    std::string obtenerNombre() const;
    std::string obtenerBarrio() const;
    int obtenerCapacidadAnclajes() const;
    int obtenerBicisDisponibles() const;

    double obtenerPorcentajeOcupacion() const;
    void mostrar() const;
};

#endif // ESTACION_HPP