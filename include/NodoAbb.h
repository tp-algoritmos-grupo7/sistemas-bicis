#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

#include "Bicicleta.h"

class Nodo
{
private:
    Bicicleta dato;
    Nodo *izquierdo;
    Nodo *derecho;

public:
    explicit Nodo(const Bicicleta &bicicleta);
    const Bicicleta &obtenerDato() const;
    Nodo *obtenerIzquierdo() const;
    Nodo *obtenerDerecho() const;

    void cambiarIzquierdo(Nodo *nodo);
    void cambiarDerecho(Nodo *nodo);
    bool esHoja();
    bool soloHijoIzquierdo();
    bool soloHijoDerecho();
};

#endif // NODO_H_INCLUDED
