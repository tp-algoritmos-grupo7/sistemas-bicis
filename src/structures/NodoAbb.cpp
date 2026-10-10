#include "NodoAbb.h"

Nodo::Nodo(int valor)
    : dato(valor),
      izquierdo(nullptr),
      derecho(nullptr)

{
}

int Nodo::obtenerDato() const
{
    return dato;
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
