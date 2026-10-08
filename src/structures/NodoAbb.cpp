#include "NodoAbb.h"

Nodo::Nodo(int valor)
    : dato(valor),
      izquierdo(nullptr),
      derecho(nullptr)

{
}

int Nodo::obtener_dato() const
{
    return dato;
}

Nodo *Nodo::obtener_izquierdo() const
{
    return izquierdo;
}

Nodo *Nodo::obtener_derecho() const
{
    return derecho;
}

void Nodo::cambiar_izquierdo(Nodo *nodo)
{
    izquierdo = nodo;
}

void Nodo::cambiar_derecho(Nodo *nodo)
{
    derecho = nodo;
}

bool Nodo::es_hoja()
{
    return izquierdo == nullptr && derecho == nullptr;
}

bool Nodo::solo_hijo_izquierdo()
{
    return izquierdo != nullptr && derecho == nullptr;
}

bool Nodo::solo_hijo_derecho()
{
    return izquierdo == nullptr && derecho != nullptr;
}
