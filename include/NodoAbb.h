#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

class Nodo
{
private:
    int dato;
    Nodo *izquierdo;
    Nodo *derecho;

public:
    explicit Nodo(int valor);

    int obtenerDato() const;
    Nodo *obtenerIzquierdo() const;
    Nodo *obtenerDerecho() const;

    void cambiarIzquierdo(Nodo *nodo);
    void cambiarDerecho(Nodo *nodo);
    bool esHoja();
    bool soloHijoIzquierdo();
    bool soloHijoDerecho();
};

#endif // NODO_H_INCLUDED