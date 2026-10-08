#ifndef ABB_H_INCLUDED
#define ABB_H_INCLUDED
#include "NodoAbb.h"

class ABB
{
private:
    Nodo *raiz;

    void insertar(int valor, Nodo *nodo);
    int contar_nodos(Nodo *nodo) const;
    int altura(Nodo *nodo) const;

public:
    ABB();

    void insertar(int valor);
    int contar_nodos() const;
    int altura() const;
    void recorrido_inorden(Nodo *raiz);
};

#endif // ABB_H_INCLUDED