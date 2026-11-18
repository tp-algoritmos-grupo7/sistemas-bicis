#include "Viaje.hpp"

Viaje::Viaje() 
    : viajeId(0), biciId(0), dniUsuario(0), estacionOrigen(""), 
      estacionDestino(""), fecha(0), duracionMin(0), enCurso(false) {}

Viaje::Viaje(int viajeId, int biciId, long long dniUsuario, 
             const std::string& estacionOrigen, const std::string& estacionDestino, 
             int fecha, int duracionMin, bool enCurso)
    : viajeId(viajeId), biciId(biciId), dniUsuario(dniUsuario), 
      estacionOrigen(estacionOrigen), estacionDestino(estacionDestino), 
      fecha(fecha), duracionMin(duracionMin), enCurso(enCurso) {}

int Viaje::obtenerViajeId() const { return viajeId; }
int Viaje::obtenerBiciId() const { return biciId; }
long long Viaje::obtenerDniUsuario() const { return dniUsuario; }
std::string Viaje::obtenerEstacionOrigen() const { return estacionOrigen; }
std::string Viaje::obtenerEstacionDestino() const { return estacionDestino; }
int Viaje::obtenerFecha() const { return fecha; }
int Viaje::obtenerDuracionMin() const { return duracionMin; }
bool Viaje::estaEnCurso() const { return enCurso; }

void Viaje::finalizar(const std::string& destino, int minutos) {
    estacionDestino = destino;
    duracionMin = minutos;
    enCurso = false;
}

void Viaje::mostrar() const {
    std::cout << "Viaje ID: " << viajeId 
              << " | Bici: " << biciId 
              << " | DNI: " << dniUsuario 
              << " | Ruta: " << estacionOrigen << " -> " << (enCurso ? "En curso" : estacionDestino)
              << " | Fecha: " << fecha 
              << " | Duracion: " << duracionMin << " min" << std::endl;
}

