#include "Viaje.hpp"

Viaje::Viaje() 
    : viajeId(0), biciId(0), dniUsuario(0), estacionOrigen(""), 
      estacionDestino(""), fecha(0), duracionMin(0) {}

Viaje::Viaje(int viajeId, int biciId, long long dniUsuario, 
             const std::string& estacionOrigen, const std::string& estacionDestino, 
             int fecha, int duracionMin)
    : viajeId(viajeId), biciId(biciId), dniUsuario(dniUsuario), 
      estacionOrigen(estacionOrigen), estacionDestino(estacionDestino), 
      fecha(fecha), duracionMin(duracionMin) {}

int Viaje::obtenerViajeId() const { return viajeId; }
int Viaje::obtenerBiciId() const { return biciId; }
long long Viaje::obtenerDniUsuario() const { return dniUsuario; }
std::string Viaje::obtenerEstacionOrigen() const { return estacionOrigen; }
std::string Viaje::obtenerEstacionDestino() const { return estacionDestino; }
int Viaje::obtenerFecha() const { return fecha; }
int Viaje::obtenerDuracionMin() const { return duracionMin; }

void Viaje::mostrar() const {
    std::cout << "Viaje ID: " << viajeId 
              << " | Bici: " << biciId 
              << " | DNI: " << dniUsuario 
              << " | Ruta: " << estacionOrigen << " -> " << estacionDestino 
              << " | Fecha: " << fecha 
              << " | Duracion: " << duracionMin << " min" << std::endl;
}

