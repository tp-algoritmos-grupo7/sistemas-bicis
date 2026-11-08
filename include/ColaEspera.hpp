#ifndef COLA_ESPERA_HPP
#define COLA_ESPERA_HPP

#include <iostream>
#include <string>

class ColaEspera {
private:
    struct NodoCola {
        long long dniUsuario;
        NodoCola* siguiente;
        NodoCola(long long dni) : dniUsuario(dni), siguiente(nullptr) {}
    };

    NodoCola* frente;
    NodoCola* final;
    size_t tamanio;

public:
    ColaEspera() : frente(nullptr), final(nullptr), tamanio(0) {}

    ~ColaEspera() {
        while (!estaVacia()) {
            desencolar();
        }
    }

    // No permitir copias para evitar doble liberación de memoria
    ColaEspera(const ColaEspera&) = delete;
    ColaEspera& operator=(const ColaEspera&) = delete;

    bool estaVacia() const {
        return frente == nullptr;
    }

    void encolar(long long dniUsuario) {
        NodoCola* nuevo = new NodoCola(dniUsuario);
        if (estaVacia()) {
            frente = nuevo;
            final = nuevo;
        } else {
            final->siguiente = nuevo;
            final = nuevo;
        }
        tamanio++;
    }

    long long desencolar() {
        if (estaVacia()) {
            return -1; // Cola vacía
        }
        NodoCola* aBorrar = frente;
        long long dni = aBorrar->dniUsuario;
        frente = frente->siguiente;
        delete aBorrar;
        tamanio--;
        if (frente == nullptr) {
            final = nullptr;
        }
        return dni;
    }

    size_t obtenerTamanio() const {
        return tamanio;
    }
};

#endif // COLA_ESPERA_HPP