#include "TablaHash.hpp"
#include <iostream>

TablaHash::TablaHash(size_t capacidadInicial) {
    if (capacidadInicial == 0) {
        capacidad = 31;
    } else {
        capacidad = capacidadInicial;
    }
    cantidad = 0;
    colisiones = 0;
    tabla = new Lista<Estacion*>[capacidad];
}

TablaHash::~TablaHash() {
    for (size_t i = 0; i < capacidad; i++) {
        auto it = tabla[i].obtenerIterador();
        while (it.tieneSiguiente()) {
            Estacion* e = it.siguiente();
            delete e;
        }
    }
    delete[] tabla;
}

size_t TablaHash::funcionHash(const std::string& clave) const {
    size_t suma = 0;
    for (size_t i = 0; i < clave.length(); i++) {
        suma += (unsigned char)clave[i];
    }
    return suma % capacidad;
}

bool TablaHash::insertar(Estacion* estacion) {
    if (estacion == nullptr) {
        return false;
    }

    std::string codigo = estacion->obtenerCodigo();
    size_t indice = funcionHash(codigo);

    // Evitamos duplicados
    auto it = tabla[indice].obtenerIterador();
    while (it.tieneSiguiente()) {
        Estacion* e = it.siguiente();
        if (e->obtenerCodigo() == codigo) {
            return false;
        }
    }

    // Si ya habia elementos en este casillero, hubo colision
    if (!tabla[indice].estaVacia()) {
        colisiones++;
    }

    tabla[indice].insertarAlFinal(estacion);
    cantidad++;
    return true;
}

Estacion* TablaHash::buscar(const std::string& codigo) const {
    size_t indice = funcionHash(codigo);
    
    auto it = tabla[indice].obtenerIterador();
    while (it.tieneSiguiente()) {
        Estacion* e = it.siguiente();
        if (e->obtenerCodigo() == codigo) {
            return e;
        }
    }

    return nullptr;
}

bool TablaHash::eliminar(const std::string& codigo) {
    size_t indice = funcionHash(codigo);
    
    auto it = tabla[indice].obtenerIterador();
    while (it.tieneSiguiente()) {
        Estacion* e = it.siguiente();
        if (e->obtenerCodigo() == codigo) {
            tabla[indice].eliminarElemento(e);
            delete e;
            cantidad--;
            return true;
        }
    }

    return false;
}

size_t TablaHash::obtenerColisiones() const {
    return colisiones;
}

double TablaHash::obtenerFactorCarga() const {
    return (double)cantidad / (double)capacidad;
}

size_t TablaHash::obtenerCantidad() const {
    return cantidad;
}

size_t TablaHash::obtenerCapacidad() const {
    return capacidad;
}

void TablaHash::obtenerTodas(Estacion** arregloSalida) const {
    if (arregloSalida == nullptr) {
        return;
    }
    size_t pos = 0;
    for (size_t i = 0; i < capacidad; i++) {
        auto it = tabla[i].obtenerIterador();
        while (it.tieneSiguiente()) {
            arregloSalida[pos] = it.siguiente();
            pos++;
        }
    }
}

void TablaHash::listarOcupacionBaja(double porcentajeLimite) const {
    std::cout << "\n--- Estaciones con ocupacion menor al " << porcentajeLimite << "% ---\n";
    bool seEncontro = false;

    for (size_t i = 0; i < capacidad; i++) {
        auto it = tabla[i].obtenerIterador();
        while (it.tieneSiguiente()) {
            Estacion* e = it.siguiente();
            if (e->obtenerPorcentajeOcupacion() < porcentajeLimite) {
                e->mostrar();
                seEncontro = true;
            }
        }
    }

    if (!seEncontro) {
        std::cout << "No hay estaciones con ocupacion menor a ese porcentaje.\n";
    }
}

void TablaHash::imprimirTabla(bool soloOcupados) const {
    std::cout << "\n=== TABLA HASH (Capacidad: " << capacidad 
              << ", Cantidad: " << cantidad 
              << ", Factor de Carga: " << obtenerFactorCarga() 
              << ", Colisiones: " << colisiones << ") ===\n";

    for (size_t i = 0; i < capacidad; i++) {
        if (tabla[i].estaVacia()) {
            if (!soloOcupados) {
                std::cout << "[" << i << "]: (vacio)\n";
            }
        } else {
            std::cout << "[" << i << "]: ";
            auto it = tabla[i].obtenerIterador();
            while (it.tieneSiguiente()) {
                Estacion* e = it.siguiente();
                std::cout << "[" << e->obtenerCodigo() << " : " << e->obtenerNombre() << "] -> ";
            }
            std::cout << "nullptr\n";
        }
    }
}
