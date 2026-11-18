#ifndef VIAJE_HPP
#define VIAJE_HPP

#include <string>
#include <iostream>

class Viaje {
private:
    int viajeId;
    int biciId;
    long long dniUsuario;
    std::string estacionOrigen;
    std::string estacionDestino;
    int fecha; // Formato AAAAMMDD (ej. 20260310)
    int duracionMin;
    bool enCurso;

public:
    Viaje();
    Viaje(int viajeId, int biciId, long long dniUsuario, 
          const std::string& estacionOrigen, const std::string& estacionDestino, 
          int fecha, int duracionMin, bool enCurso = false);

    int obtenerViajeId() const;
    int obtenerBiciId() const;
    long long obtenerDniUsuario() const;
    std::string obtenerEstacionOrigen() const;
    std::string obtenerEstacionDestino() const;
    int obtenerFecha() const;
    int obtenerDuracionMin() const;
    bool estaEnCurso() const;
    void finalizar(const std::string& destino, int minutos);

    void mostrar() const;
};

#endif // VIAJE_HPP
