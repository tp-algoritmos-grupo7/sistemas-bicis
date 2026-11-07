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
// Inserta por biciId. Devuelve false si el ID ya existe.
bool Abb::insertar(const Bicicleta &bicicleta)
{
    // Si el arbol esta vacio, la bicicleta pasa a ser la raiz.
    if (raiz == nullptr)
    {
        raiz = new Nodo(bicicleta);
        return true;
    }
    Nodo *actual = raiz;
    int biciId = bicicleta.getBiciId();

    while (actual != nullptr)
    {
        // Si el ID ya existe, se retorna false.
        if (biciId == actual->obtenerDato().getBiciId())
            return false;

        // Si el ID es menor, buscamos un lugar a la izquierda.
        if (biciId < actual->obtenerDato().getBiciId())
        {
            if (actual->obtenerIzquierdo() == nullptr)
            {
                // Si el ID es menor, se inserta a la izquierda.
                actual->cambiarIzquierdo(new Nodo(bicicleta));
                return true;
            }
            
            actual = actual->obtenerIzquierdo();
        }
        // Si el ID es mayor, buscamos un lugar a la derecha.
        else
        {
            if (actual->obtenerDerecho() == nullptr)
            {
                actual->cambiarDerecho(new Nodo(bicicleta));
                return true;
            }
            actual = actual->obtenerDerecho();
        }
    }

    return false;
}

// metodo publico para buscar una bicicleta por biciId
// retorna un puntero a la bicicleta guardada en el arbol, o nullptr si no existe.
// El puntero pertenece al arbol: quien lo recibe no debe liberarlo
const Bicicleta *Abb::buscar(int biciId) const
{
    Nodo *actual = raiz;

    // En cada paso avanzamos al unico subarbol que puede contener el ID.
    while (actual != nullptr)
    {
        if (biciId == actual->obtenerDato().getBiciId())
            return &actual->obtenerDato();

        if (biciId < actual->obtenerDato().getBiciId())
            actual = actual->obtenerIzquierdo();
        else
            actual = actual->obtenerDerecho();
    }

    return nullptr;
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

bool Abb::eliminar(int biciId)
{
    Nodo* padre = buscarPadre(biciId);
    Nodo* nodoAEliminar = nullptr;

    if (padre != nullptr)
    {
        if (biciId < padre->obtenerDato().getBiciId())
            nodoAEliminar = padre->obtenerIzquierdo();
        else
            nodoAEliminar = padre->obtenerDerecho();
    }
    else if (raiz != nullptr &&
             raiz->obtenerDato().getBiciId() == biciId)
    {
        nodoAEliminar = raiz;
    }

    if (nodoAEliminar == nullptr)
        return false;

    if (nodoAEliminar->esHoja())
        eliminarCasoHoja(nodoAEliminar, padre);
    else if (nodoAEliminar->soloHijoIzquierdo() ||
             nodoAEliminar->soloHijoDerecho())
        eliminarCasoUnHijo(nodoAEliminar, padre);
    else
        eliminarCasoDosHijos(nodoAEliminar, padre);

    return true;

}

void Abb::eliminarCasoHoja(Nodo *nodoAEliminar, Nodo *padre)
{
    if (nodoAEliminar == raiz)
        raiz = nullptr;
    else if (padre->obtenerIzquierdo() == nodoAEliminar)
        padre->cambiarIzquierdo(nullptr);
    else
        padre->cambiarDerecho(nullptr);
    delete nodoAEliminar;
}

void Abb::eliminarCasoUnHijo(Nodo *nodoAEliminar, Nodo *padre)
{
    Nodo *aux = nodoAEliminar;
    Nodo *hijo;

    // Obtener el unico hijo no nulo.
    if (aux->obtenerIzquierdo() != nullptr)
        hijo = aux->obtenerIzquierdo();
    else
        hijo = aux->obtenerDerecho();

    // El hijo ocupa el lugar de aux en el enlace del padre o en la raiz.
    if (aux == raiz)
        raiz = hijo;
    else if (padre->obtenerIzquierdo() == aux)
        padre->cambiarIzquierdo(hijo);
    else
        padre->cambiarDerecho(hijo);

    delete aux;
}

// Se llama para un nodo con dos hijos: su subarbol derecho existe.
Nodo *Abb::pPadreMenorMayores(Nodo *nodo) const
{
    Nodo *padre = nodo;
    Nodo *actual = nodo->obtenerDerecho();

    while (actual->obtenerIzquierdo() != nullptr)
    {
        padre = actual;
        actual = actual->obtenerIzquierdo();
    }

    return padre;
}

void Abb::eliminarCasoDosHijos(Nodo *nodoAEliminar, Nodo * /*padre*/)
{
    Nodo *aux = nodoAEliminar;
    Nodo *aux2 = pPadreMenorMayores(aux);
    Nodo *menorMayores;

    // Puede ser el hijo derecho inmediato o uno mas a la izquierda.
    if (aux2 == aux)
        menorMayores = aux->obtenerDerecho();
    else
        menorMayores = aux2->obtenerIzquierdo();

    // Copiar toda la bicicleta para mantener los datos asociados al ID.
    aux->cambiarDato(menorMayores->obtenerDato());

    // El sucesor no tiene hijo izquierdo: es hoja o tiene un hijo derecho.
    if (menorMayores->esHoja())
        eliminarCasoHoja(menorMayores, aux2);
    else
        eliminarCasoUnHijo(menorMayores, aux2);

    // aux permanece en el arbol; el caso anterior ya elimino el sucesor.
}
