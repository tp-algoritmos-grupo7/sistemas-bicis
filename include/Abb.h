#ifndef ABB_H_INCLUDED
#define ABB_H_INCLUDED
#include "NodoAbb.h"

class Abb
{
private:
    Nodo *raiz;

    bool insertar(const Bicicleta &bicicleta, Nodo *nodo);
    const Bicicleta *buscar(int biciId, Nodo *nodo) const;
    int contarNodos(Nodo *nodo) const;
    int altura(Nodo *nodo) const;
    void recorridoInorden(Nodo *nodo) const;

public:
    Abb();

    bool insertar(const Bicicleta &bicicleta);
    const Bicicleta *buscar(int biciId) const;
    int contarNodos() const;
    int altura() const;
    void recorridoInorden() const;
};

#endif // ABB_H_INCLUDED
