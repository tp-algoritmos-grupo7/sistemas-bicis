#ifndef LISTA_HPP
#define LISTA_HPP

#include <cstddef>
#include <stdexcept>
#include <iostream>

template <typename T>
class Lista {
private:
    struct Nodo {
        T dato;
        Nodo* siguiente;
        Nodo(const T& d) : dato(d), siguiente(nullptr) {}
    };

    Nodo* cabeza;
    Nodo* cola;
    size_t tamanio;

public:
    Lista() : cabeza(nullptr), cola(nullptr), tamanio(0) {}

    ~Lista() {
        vaciar();
    }

    Lista(const Lista&) = delete;
    Lista& operator=(const Lista&) = delete;

    void insertarAlFinal(const T& dato) {
        Nodo* nuevo = new Nodo(dato);
        if (estaVacia()) {
            cabeza = nuevo;
            cola = nuevo;
        } else {
            cola->siguiente = nuevo;
            cola = nuevo;
        }
        tamanio++;
    }

    void insertarAlInicio(const T& dato) {
        Nodo* nuevo = new Nodo(dato);
        if (estaVacia()) {
            cabeza = nuevo;
            cola = nuevo;
        } else {
            nuevo->siguiente = cabeza;
            cabeza = nuevo;
        }
        tamanio++;
    }

    T eliminarDelInicio() {
        if (estaVacia()) {
            throw std::runtime_error("Error: la lista esta vacia.");
        }
        Nodo* aBorrar = cabeza;
        T valor = aBorrar->dato;
        cabeza = cabeza->siguiente;
        delete aBorrar;
        tamanio--;
        if (cabeza == nullptr) {
            cola = nullptr;
        }
        return valor;
    }

    bool estaVacia() const {
        return cabeza == nullptr;
    }

    size_t obtenerTamanio() const {
        return tamanio;
    }

    void vaciar() {
        while (!estaVacia()) {
            eliminarDelInicio();
        }
    }

    void mostrar() const {
        Nodo* actual = cabeza;
        if (estaVacia()) {
            std::cout << "[Lista vacia] -> nullptr" << std::endl;
            return;
        }
        while (actual != nullptr) {
            std::cout << "[" << actual->dato << "] -> ";
            actual = actual->siguiente;
        }
        std::cout << "nullptr" << std::endl;
    }

    class Iterador {
    private:
        Nodo* actual;
    public:
        Iterador(Nodo* n) : actual(n) {}

        bool tieneSiguiente() const {
            return actual != nullptr;
        }

        T& obtenerDato() const {
            return actual->dato;
        }

        void avanzar() {
            if (actual != nullptr) {
                actual = actual->siguiente;
            }
        }
    };

    Iterador obtenerIterador() const {
        return Iterador(cabeza);
    }
};

#endif // LISTA_HPP