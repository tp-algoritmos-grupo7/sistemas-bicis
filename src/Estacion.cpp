#include "Estacion.hpp"
#include <iomanip>

Estacion::Estacion() 
    : codigo(""), nombre(""), barrio(""), capacidadAnclajes(0), bicisDisponibles(0) {}

Estacion::Estacion(const std::string& codigo, const std::string& nombre, 
                   const std::string& barrio, int capacidadAnclajes, int bicisDisponibles)
    : codigo(codigo), nombre(nombre), barrio(barrio), 
      capacidadAnclajes(capacidadAnclajes), bicisDisponibles(bicisDisponibles) {}

std::string Estacion::obtenerCodigo() const {
    return codigo;
}

std::string Estacion::obtenerNombre() const {
    return nombre;
}

std::string Estacion::obtenerBarrio() const {
    return barrio;
}

int Estacion::obtenerCapacidadAnclajes() const {
    return capacidadAnclajes;
}

int Estacion::obtenerBicisDisponibles() const {
    return bicisDisponibles;
}

double Estacion::obtenerPorcentajeOcupacion() const {
    if (capacidadAnclajes <= 0) {
        return 0.0;
    }
    return (static_cast<double>(bicisDisponibles) / static_cast<double>(capacidadAnclajes)) * 100.0;
}

void Estacion::mostrar() const {
    std::cout << "[" << codigo << "] " << nombre << " (" << barrio << ") | "
              << "Bicis: " << bicisDisponibles << "/" << capacidadAnclajes << " | "
              << "Ocupacion: " << std::fixed << std::setprecision(1) 
              << obtenerPorcentajeOcupacion() << "%"
              << std::endl;
}