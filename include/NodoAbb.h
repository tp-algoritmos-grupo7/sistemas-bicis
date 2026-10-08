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

    int obtener_dato() const;
    Nodo *obtener_izquierdo() const;
    Nodo *obtener_derecho() const;

    void cambiar_izquierdo(Nodo *nodo);
    void cambiar_derecho(Nodo *nodo);
    bool es_hoja();
    bool solo_hijo_izquierdo();
    bool solo_hijo_derecho();
};

#endif // NODO_H_INCLUDED