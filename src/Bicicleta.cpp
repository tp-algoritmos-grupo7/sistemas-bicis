#include "Bicicleta.h"

Bicicleta::Bicicleta()
    : biciId(0),
      codigoEstacion(0),
      tipoBicicleta(Tipo::Mecanica),
      estado(Estado::OK),
      kilometros(0)
{
}

Bicicleta::Bicicleta(int biciId, int codigoEstacion, Tipo tipoBicicleta, Estado estado, int kilometros)
    : biciId(biciId),
      codigoEstacion(codigoEstacion),
      tipoBicicleta(tipoBicicleta),
      estado(estado),
      kilometros(kilometros)
{
}

int Bicicleta::getBiciId() const
{
    return biciId;
}

int Bicicleta::getCodigoEstacion() const
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

void Bicicleta::setCodigoEstacion(int codigoEstacion)
{
    this->codigoEstacion = codigoEstacion;
}

void Bicicleta::comenzarViaje()
{
}

void Bicicleta::finalizarViaje()
{
}
