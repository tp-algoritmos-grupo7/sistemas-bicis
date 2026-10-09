#include "ABB.h"
#include <iostream>
#include <stack>
using namespace std;
ABB::ABB()
    : raiz(nullptr)
{
}
// metodo publico para insertar un valor en el árbol
void ABB::insertar(int valor)
{
    if (raiz == nullptr)
        raiz = new Nodo(valor);
    else
        insertar(valor, raiz);
}

// metodo privado para insertar un valor en el árbol a partir de un nodo dado
void ABB::insertar(int valor, Nodo *nodo)
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

int ABB::contar_nodos() const
{
    return contar_nodos(raiz);
}

int ABB::contar_nodos(Nodo *nodo) const
{
    if (nodo == nullptr)
        return 0;

    return 1 + contar_nodos(nodo->obtener_izquierdo()) + contar_nodos(nodo->obtener_derecho());
}

int ABB::altura() const
{
    return altura(raiz);
}

int ABB::altura(Nodo *nodo) const
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
void ABB::recorrido_inorden(Nodo *raiz)
{

    if (raiz == nullptr)
        return;

    recorrido_inorden(raiz->obtener_izquierdo());
    cout << raiz->obtener_dato() << " ";
    recorrido_inorden(raiz->obtener_derecho());
}