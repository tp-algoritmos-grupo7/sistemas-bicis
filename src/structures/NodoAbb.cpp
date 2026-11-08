#include "NodoAbb.h"

Nodo::Nodo(const Bicicleta &bicicleta)
    : dato(bicicleta),
      izquierdo(nullptr),
      derecho(nullptr)

{
}

const Bicicleta &Nodo::obtenerDato() const
{
    return dato;
}

void Nodo::cambiarDato(const Bicicleta &bicicleta)
{
    dato = bicicleta;
}

Nodo *Nodo::obtenerIzquierdo() const
{
    return izquierdo;
}

Nodo *Nodo::obtenerDerecho() const
{
    return derecho;
}

void Nodo::cambiarIzquierdo(Nodo *nodo)
{
    izquierdo = nodo;
}

void Nodo::cambiarDerecho(Nodo *nodo)
{
    derecho = nodo;
}

bool Nodo::esHoja()
{
    return izquierdo == nullptr && derecho == nullptr;
}

bool Nodo::soloHijoIzquierdo()
{
    return izquierdo != nullptr && derecho == nullptr;
}

bool Nodo::soloHijoDerecho()
{
    return izquierdo == nullptr && derecho != nullptr;
}
