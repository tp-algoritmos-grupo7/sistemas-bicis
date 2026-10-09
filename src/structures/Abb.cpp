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
    if (valor < nodo->obtener_dato())
    {
        if (nodo->obtener_izquierdo() == nullptr)
            nodo->cambiar_izquierdo(new Nodo(valor));
        else
            insertar(valor, nodo->obtener_izquierdo());
    }
    else if (valor > nodo->obtener_dato())
    {
        if (nodo->obtener_derecho() == nullptr)
            nodo->cambiar_derecho(new Nodo(valor));
        else
            insertar(valor, nodo->obtener_derecho());
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

    return 1 + contarNodos(nodo->obtener_izquierdo()) + contarNodos(nodo->obtener_derecho());
}

int Abb::altura() const
{
    return altura(raiz);
}

int Abb::altura(Nodo *nodo) const
{
    if (nodo == nullptr)
        return -1;

    int alturaIzquierda = altura(nodo->obtener_izquierdo());
    int alturaDerecha = altura(nodo->obtener_derecho());

    return 1 + (alturaIzquierda > alturaDerecha
                    ? alturaIzquierda
                    : alturaDerecha);
}
// recorrido inorden - primero izq, luego nodo, luego der - recursivo
void Abb::recorridoInorden(Nodo *raiz)
{

    if (raiz == nullptr)
        return;

    recorridoInorden(raiz->obtener_izquierdo());
    cout << raiz->obtener_dato() << " ";
    recorridoInorden(raiz->obtener_derecho());
}