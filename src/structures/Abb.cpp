#include "Abb.h"
#include <iostream>
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

// destructor: libera todos los nodos creados con new
Abb::~Abb()
{
    liberar(raiz);
    raiz = nullptr;
}

// recorrido postorden - primero izq, luego der, al final el nodo.
// El nodo se libera ultimo porque sus punteros a hijos se pierden con el delete
void Abb::liberar(Nodo *nodo)
{
    if (nodo == nullptr)
        return;

    liberar(nodo->obtenerIzquierdo());
    liberar(nodo->obtenerDerecho());
    delete nodo;
}
// metodo publico para insertar una bicicleta en el árbol
bool Abb::insertar(const Bicicleta &bicicleta)
{
    if (raiz == nullptr)
    {
        raiz = new Nodo(bicicleta);
        return true;
    }
    else
        return insertar(bicicleta, raiz);
}

// metodo privado para insertar una bicicleta en el árbol a partir de un nodo
// dado. La clave de ordenamiento es el biciId
// retorna true si se inserta correctamente, false si ya existe
bool Abb::insertar(const Bicicleta &bicicleta, Nodo *nodo)
{
    // si el biciId es menor, se inserta a la izquierda
    if (bicicleta.getBiciId() < nodo->obtenerDato().getBiciId())
    {
        // si el nodo izquierdo es nulo, se inserta el nuevo nodo
        if (nodo->obtenerIzquierdo() == nullptr)
        {
            nodo->cambiarIzquierdo(new Nodo(bicicleta));
            return true;
        }
        // si el nodo izquierdo no es nulo, se inserta el nuevo nodo en el nodo izquierdo
        else
            return insertar(bicicleta, nodo->obtenerIzquierdo());
    }
    else if (bicicleta.getBiciId() > nodo->obtenerDato().getBiciId())
    {
        if (nodo->obtenerDerecho() == nullptr)
        {
            nodo->cambiarDerecho(new Nodo(bicicleta));
            return true;
        }
        else
            return insertar(bicicleta, nodo->obtenerDerecho());
    }
    // si el biciId es igual, se retorna false
    else
        return false;
}

// metodo publico para buscar una bicicleta por biciId
// retorna un puntero a la bicicleta guardada en el arbol, o nullptr si no existe.
// El puntero pertenece al arbol: quien lo recibe no debe liberarlo
const Bicicleta *Abb::buscar(int biciId) const
{
    return buscar(biciId, raiz);
}

// metodo privado para buscar a partir de un nodo dado. En cada paso descarta
// el subarbol que no puede contener el biciId
const Bicicleta *Abb::buscar(int biciId, Nodo *nodo) const
{
    if (nodo == nullptr)
        return nullptr;

    if (biciId < nodo->obtenerDato().getBiciId())
        return buscar(biciId, nodo->obtenerIzquierdo());
    else if (biciId > nodo->obtenerDato().getBiciId())
        return buscar(biciId, nodo->obtenerDerecho());
    else
        return &nodo->obtenerDato();
}

Nodo *Abb::buscarPadre(int biciId) const
{
    Nodo *padre = nullptr;
    Nodo *actual = raiz;

    // Mientras el nodo actual no sea nulo, se busca el padre del nodo actual
    while (actual != nullptr)
    {
        if (actual->obtenerDato().getBiciId() == biciId)
            return padre;

        padre = actual;
        // Si el biciId es menor, se busca en el subarbol izquierdo
        if (biciId < actual->obtenerDato().getBiciId())
            actual = actual->obtenerIzquierdo();
        // Si el biciId es mayor, se busca en el subarbol derecho
        else
            actual = actual->obtenerDerecho();
    }
    // Si el nodo actual es nulo, se retorna nullptr
    return nullptr;
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

// metodo publico para contar las bicicletas averiadas en el arbol
int Abb::contarBicicletasAveriadas() const
{
    return contarBicicletasAveriadas(raiz);
}

// metodo privado para contar las bicicletas averiadas en el arbol a partir de un nodo dado
int Abb::contarBicicletasAveriadas(Nodo *nodo) const
{
    if (nodo == nullptr)
        return 0;

    int actual = (nodo->obtenerDato().getEstado() == Bicicleta::Estado::Averiada)
                     ? 1
                     : 0;

    return actual
           + contarBicicletasAveriadas(nodo->obtenerIzquierdo())
           + contarBicicletasAveriadas(nodo->obtenerDerecho());
}
