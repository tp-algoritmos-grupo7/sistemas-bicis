#ifndef ABB_H_INCLUDED
#define ABB_H_INCLUDED
#include "NodoAbb.h"

class Abb
{
private:
    Nodo *raiz;

    bool insertar(const Bicicleta &bicicleta, Nodo *nodo);
    int contarNodos(Nodo *nodo) const;
    int altura(Nodo *nodo) const;
    void recorridoInorden(Nodo *nodo) const;

public:
    Abb();

    bool insertar(const Bicicleta &bicicleta);
    int contarNodos() const;
    int altura() const;
    void recorridoInorden() const;
};

#endif // ABB_H_INCLUDED
