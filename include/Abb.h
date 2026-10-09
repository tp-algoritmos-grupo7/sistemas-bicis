#ifndef ABB_H_INCLUDED
#define ABB_H_INCLUDED
#include "NodoAbb.h"

class Abb
{
private:
    Nodo *raiz;

    void insertar(int valor, Nodo *nodo);
    int contarNodos(Nodo *nodo) const;
    int altura(Nodo *nodo) const;

public:
    Abb();

    void insertar(int valor);
    int contarNodos() const;
    int altura() const;
    void recorridoInorden(Nodo *raiz);
};

#endif // ABB_H_INCLUDED