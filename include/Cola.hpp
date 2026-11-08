#ifndef COLA_HPP
#define COLA_HPP

#include <cstddef>
#include "Lista.hpp"

// Cola FIFO
// Se apoya en Lista: se encola al final y se desencola del inicio,
// las dos operaciones son O(1) porque Lista guarda puntero a cabeza y a cola.
template <typename T>
class Cola {
private:
    Lista<T> elementos;

public:
    Cola() {}

    void encolar(const T& dato) {
        elementos.insertarAlFinal(dato);
    }

    // Saca y devuelve el primer elemento (lanza excepcion si esta vacia)
    T desencolar() {
        return elementos.eliminarDelInicio();
    }

    bool estaVacia() const {
        return elementos.estaVacia();
    }

    size_t obtenerTamanio() const {
        return elementos.obtenerTamanio();
    }
};

#endif // COLA_HPP
