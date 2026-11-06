#ifndef ABB_H_INCLUDED
#define ABB_H_INCLUDED
#include "NodoAbb.h"

class Abb
{
private:
    Nodo *raiz;

    bool insertar(const Bicicleta &bicicleta, Nodo *nodo);
    const Bicicleta *buscar(int biciId, Nodo *nodo) const;
    // Devuelve nullptr si el ID no existe o corresponde a la raiz.
    Nodo *buscarPadre(int biciId) const;
    void eliminarCasoHoja(Nodo *nodoAEliminar, Nodo *padre);
    void eliminarCasoUnHijo(Nodo *nodoAEliminar, Nodo *padre);
    void eliminarCasoDosHijos(Nodo *nodoAEliminar, Nodo *padre);
    Nodo *pPadreMenorMayores(Nodo *nodo) const;
    int contarNodos(Nodo *nodo) const;
    int altura(Nodo *nodo) const;
    void recorridoInorden(Nodo *nodo) const;
    void liberar(Nodo *nodo);
    int contarBicicletasAveriadas(Nodo *nodo) const;

public:
    Abb();
    ~Abb();

    // el arbol es duenio de sus nodos: una copia compartiria los mismos
    // punteros y ambos destructores liberarian los mismos nodos
    // Se prohibe copiar el abb abb1 = abb2 o Abb(abb1)
    Abb(const Abb &otro) = delete;
    Abb &operator=(const Abb &otro) = delete;

    bool insertar(const Bicicleta &bicicleta);
    const Bicicleta *buscar(int biciId) const;
    int contarNodos() const;
    int altura() const;
    void recorridoInorden() const;
    int contarBicicletasAveriadas() const;
    bool eliminar(int biciId);
};

#endif // ABB_H_INCLUDED
