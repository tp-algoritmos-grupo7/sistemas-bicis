#include "Abb.h"
#include <iostream>
#include <stack>
using namespace std;

// auxiliares de presentacion, con enlace interno a este archivo
namespace
{
    const char *textoTipo(Bicicleta::Tipo tipo)
    {
        return tipo == Bicicleta::Tipo::Mecanica ? "Mecanica" : "Electrica";
    }

    const char *textoEstado(Bicicleta::Estado estado)
    {
        return estado == Bicicleta::Estado::OK ? "OK" : "Averiada";
    }
}

Abb::Abb()
    : raiz(nullptr)
{
}
// metodo publico para insertar una bicicleta en el árbol
void Abb::insertar(const Bicicleta &bicicleta)
{
    if (raiz == nullptr)
        raiz = new Nodo(bicicleta);
    else
        insertar(bicicleta, raiz);
}

// metodo privado para insertar una bicicleta en el árbol a partir de un nodo
// dado. La clave de ordenamiento es el biciId
void Abb::insertar(const Bicicleta &bicicleta, Nodo *nodo)
{
    if (bicicleta.getBiciId() < nodo->obtenerDato().getBiciId())
    {
        if (nodo->obtenerIzquierdo() == nullptr)
            nodo->cambiarIzquierdo(new Nodo(bicicleta));
        else
            insertar(bicicleta, nodo->obtenerIzquierdo());
    }
    else if (bicicleta.getBiciId() > nodo->obtenerDato().getBiciId())
    {
        if (nodo->obtenerDerecho() == nullptr)
            nodo->cambiarDerecho(new Nodo(bicicleta));
        else
            insertar(bicicleta, nodo->obtenerDerecho());
    }
}

int Abb::contarNodos() const
{
    return contarNodos(raiz);
}

int Abb::contarNodos(Nodo *nodo) const
{
    if (nodo == nullptr)
        return 0;

    return 1 + contarNodos(nodo->obtenerIzquierdo()) + contarNodos(nodo->obtenerDerecho());
}

int Abb::altura() const
{
    return altura(raiz);
}

int Abb::altura(Nodo *nodo) const
{
    if (nodo == nullptr)
        return -1;

    int alturaIzquierda = altura(nodo->obtenerIzquierdo());
    int alturaDerecha = altura(nodo->obtenerDerecho());

    return 1 + (alturaIzquierda > alturaDerecha
                    ? alturaIzquierda
                    : alturaDerecha);
}
void Abb::recorridoInorden() const
{
    recorridoInorden(raiz);
}

// recorrido inorden - primero izq, luego nodo, luego der - recursivo
void Abb::recorridoInorden(Nodo *nodo) const
{

    if (nodo == nullptr)
        return;

    recorridoInorden(nodo->obtenerIzquierdo());

    const Bicicleta &bicicleta = nodo->obtenerDato();
    cout << bicicleta.getBiciId() << " "
         << bicicleta.getCodigoEstacion() << " "
         << textoTipo(bicicleta.getTipo()) << " "
         << textoEstado(bicicleta.getEstado()) << " "
         << bicicleta.getKilometros() << "\n";

    recorridoInorden(nodo->obtenerDerecho());
}