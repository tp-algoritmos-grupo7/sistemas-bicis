#include "Abb.h"
#include <iostream>
#include <stack>
using namespace std;
Abb::Abb()
    : raiz(nullptr)
{
}
// metodo publico para insertar un valor en el árbol
void Abb::insertar(int valor)
{
    if (raiz == nullptr)
        raiz = new Nodo(valor);
    else
        insertar(valor, raiz);
}

// metodo privado para insertar un valor en el árbol a partir de un nodo dado
void Abb::insertar(int valor, Nodo *nodo)
{
    if (valor < nodo->obtenerDato())
    {
        if (nodo->obtenerIzquierdo() == nullptr)
            nodo->cambiarIzquierdo(new Nodo(valor));
        else
            insertar(valor, nodo->obtenerIzquierdo());
    }
    else if (valor > nodo->obtenerDato())
    {
        if (nodo->obtenerDerecho() == nullptr)
            nodo->cambiarDerecho(new Nodo(valor));
        else
            insertar(valor, nodo->obtenerDerecho());
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
// recorrido inorden - primero izq, luego nodo, luego der - recursivo
void Abb::recorridoInorden(Nodo *raiz)
{

    if (raiz == nullptr)
        return;

    recorridoInorden(raiz->obtenerIzquierdo());
    cout << raiz->obtenerDato() << " ";
    recorridoInorden(raiz->obtenerDerecho());
}