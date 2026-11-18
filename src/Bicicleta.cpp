#include "Bicicleta.h"
#include <string>
Bicicleta::Bicicleta()
    : biciId(0),
      codigoEstacion(""),
      tipoBicicleta(Tipo::Mecanica),
      estado(Estado::OK),
      kilometros(0),
      enViaje(false),
      enTaller(false)
{
}

Bicicleta::Bicicleta(int biciId, std::string codigoEstacion, Tipo tipoBicicleta, Estado estado, int kilometros)
    : biciId(biciId),
      codigoEstacion(codigoEstacion),
      tipoBicicleta(tipoBicicleta),
      estado(estado),
      kilometros(kilometros),
      enViaje(false),
      enTaller(false)
{
}

int Bicicleta::getBiciId() const
{
    return biciId;
}

std::string Bicicleta::getCodigoEstacion() const
{
    return codigoEstacion;
}

Bicicleta::Tipo Bicicleta::getTipo() const
{
    return tipoBicicleta;
}

Bicicleta::Estado Bicicleta::getEstado() const
{
    return estado;
}

int Bicicleta::getKilometros() const
{
    return kilometros;
}

void Bicicleta::setEstado(Estado estado)
{
    this->estado = estado;
}

void Bicicleta::setCodigoEstacion(std::string codigoEstacion)
{
    this->codigoEstacion = codigoEstacion;
}

void Bicicleta::comenzarViaje()
{
    enViaje = true;
    codigoEstacion.clear();
}

void Bicicleta::finalizarViaje(const std::string &codigoDestino)
{
    codigoEstacion = codigoDestino;
    enViaje = false;
}

bool Bicicleta::estaEnViaje() const
{
    return enViaje;
}

bool Bicicleta::estaEnTaller() const
{
    return enTaller;
}

void Bicicleta::ingresarAlTaller()
{
    estado = Estado::Averiada;
    enTaller = true;
    codigoEstacion.clear();
}

void Bicicleta::finalizarReparacion(const std::string &codigoDestino)
{
    codigoEstacion = codigoDestino;
    estado = Estado::OK;
    enTaller = false;
}
