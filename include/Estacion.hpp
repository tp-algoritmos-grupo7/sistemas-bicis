#ifndef ESTACION_HPP
#define ESTACION_HPP

#include <string>
#include <iostream>
#include "ColaEspera.hpp"

class Estacion {
private:
    std::string codigo;
    std::string nombre;
    std::string barrio;
    int capacidadAnclajes;
    int bicisDisponibles;
    ColaEspera colaEspera;

public:
    Estacion();
    Estacion(const std::string& codigo, const std::string& nombre, 
             const std::string& barrio, int capacidadAnclajes, int bicisDisponibles);

    // Getters existentes
    std::string obtenerCodigo() const;
    std::string obtenerNombre() const;
    std::string obtenerBarrio() const;
    int obtenerCapacidadAnclajes() const;
    int obtenerBicisDisponibles() const;

    // NUEVOS SETTERS (Modificadores para sumar/restar stock y capacidad)
    void asignarBicisDisponibles(int bicis);
    void asignarCapacidadAnclajes(int capacidad);

    // Métodos de la cola de espera
    void encolarUsuario(long long dniUsuario);
    long long desencolarUsuario();
    bool tieneUsuariosEsperando() const;

    double obtenerPorcentajeOcupacion() const;
    void mostrar() const;
};

#endif // ESTACION_HPP