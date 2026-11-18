#ifndef MIN_HEAP_TALLER_HPP
#define MIN_HEAP_TALLER_HPP

#include <iostream>
#include <string>

// Estructura auxiliar para la bici en el taller
struct ElementoTaller {
    int biciId;
    std::string tipo; // "Mecanica" o "Electrica"
    int prioridad;    // 1 (mas urgente) a 3
};

class MinHeapTaller {
private:
    ElementoTaller* datos;
    size_t capacidad;
    size_t tamanio;

    bool esMenor(const ElementoTaller& a, const ElementoTaller& b) const {
        if (a.prioridad != b.prioridad) {
            return a.prioridad < b.prioridad; // Menor número de prioridad = más urgente
        }
        // Desempate por tipo ante igual prioridad: Las eléctricas tienen prioridad sobre las mecánicas
        if (a.tipo == "Electrica" && b.tipo == "Mecanica") {
            return true;
        }
        return false;
    }

    void flotar(size_t i) {
        while (i > 0) {
            size_t padre = (i - 1) / 2;
            if (esMenor(datos[i], datos[padre])) { // <-- USAMOS LA NUEVA CONDICIÓN
                // Intercambiar
                ElementoTaller temp = datos[i];
                datos[i] = datos[padre];
                datos[padre] = temp;
                i = padre;
            } else {
                break;
            }
        }
    }

    void hundir(size_t i) {
        while (2 * i + 1 < tamanio) {
            size_t izq = 2 * i + 1;
            size_t der = 2 * i + 2;
            size_t menor = i;

            if (izq < tamanio && esMenor(datos[izq], datos[menor])) { // <-- USAMOS LA NUEVA CONDICIÓN
                menor = izq;
            }
            if (der < tamanio && esMenor(datos[der], datos[menor])) { // <-- USAMOS LA NUEVA CONDICIÓN
                menor = der;
            }

            if (menor != i) {
                ElementoTaller temp = datos[i];
                datos[i] = datos[menor];
                datos[menor] = temp;
                i = menor;
            } else {
                break;
            }
        }
    }

public:
    MinHeapTaller(size_t cap = 100) : capacidad(cap), tamanio(0) {
        datos = new ElementoTaller[capacidad];
    }

    ~MinHeapTaller() {
        delete[] datos;
    }

    MinHeapTaller(const MinHeapTaller&) = delete;
    MinHeapTaller& operator=(const MinHeapTaller&) = delete;

    bool estaVacio() const {
        return tamanio == 0;
    }

    bool estaLleno() const {
        return tamanio >= capacidad;
    }

    // Consultar antes de extraer permite validar el destino sin perder la bicicleta.
    const ElementoTaller* consultarMin() const {
        return estaVacio() ? nullptr : &datos[0];
    }

    bool insertar(int biciId, const std::string& tipo, int prioridadOriginal) {
        if (estaLleno() || biciId <= 0 || prioridadOriginal < 1 || prioridadOriginal > 3 ||
            (tipo != "Electrica" && tipo != "Mecanica"))
            return false;

        // Regla: Las electricas entran siempre con prioridad 1
        int prioridadFinal = prioridadOriginal;
        if (tipo == "Electrica") {
            prioridadFinal = 1; 
        }

        datos[tamanio] = {biciId, tipo, prioridadFinal};
        flotar(tamanio);
        tamanio++;
        return true;
    }

    ElementoTaller extraerMin() {
        if (estaVacio()) {
            return {-1, "", -1};
        }
        ElementoTaller minElem = datos[0];
        datos[0] = datos[tamanio - 1];
        tamanio--;
        hundir(0);
        return minElem;
    }
};

#endif // MIN_HEAP_TALLER_HPP
