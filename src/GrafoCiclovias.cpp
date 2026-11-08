#include "GrafoCiclovias.hpp"
#include "Cola.hpp"
#include <iostream>
#include <fstream>

GrafoCiclovias::GrafoCiclovias(const TablaHash& tabla) {
    cantidad = static_cast<int>(tabla.obtenerCantidad());
    codigos = new std::string[cantidad];
    adyacentes = new Lista<int>[cantidad];

    Estacion** todas = new Estacion*[cantidad];
    tabla.obtenerTodas(todas);
    for (int i = 0; i < cantidad; i++) {
        codigos[i] = todas[i]->obtenerCodigo();
    }
    delete[] todas;
}

GrafoCiclovias::~GrafoCiclovias() {
    delete[] codigos;
    delete[] adyacentes;
}

int GrafoCiclovias::buscarIndice(const std::string& codigo) const {
    for (int i = 0; i < cantidad; i++) {
        if (codigos[i] == codigo) {
            return i;
        }
    }
    return -1;
}

bool GrafoCiclovias::agregarCiclovia(const std::string& codigoA, const std::string& codigoB) {
    int a = buscarIndice(codigoA);
    int b = buscarIndice(codigoB);
    if (a == -1 || b == -1) {
        return false;
    }

    adyacentes[a].insertarAlFinal(b);
    adyacentes[b].insertarAlFinal(a);
    return true;
}

bool GrafoCiclovias::cargarDesdeArchivo(const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo abrir " << rutaArchivo << std::endl;
        return false;
    }

    std::string codigoA, codigoB;
    while (archivo >> codigoA >> codigoB) {
        if (!agregarCiclovia(codigoA, codigoB)) {
            std::cerr << "Aviso: ciclovia " << codigoA << "-" << codigoB
                      << " ignorada (estacion inexistente).\n";
        }
    }

    archivo.close();
    return true;
}

void GrafoCiclovias::quitarEstacion(const std::string& codigo) {
    int indice = buscarIndice(codigo);
    if (indice == -1) {
        return;
    }

    // Eliminar referencias en las estaciones vecinas
    auto it = adyacentes[indice].obtenerIterador();
    while (it.tieneSiguiente()) {
        int vecina = it.siguiente();
        adyacentes[vecina].eliminar(indice);
    }
    adyacentes[indice].vaciar();
}

int GrafoCiclovias::buscarRecorrido(const std::string& origen, const std::string& destino,
                                    Lista<std::string>& recorrido) const {
    int inicio = buscarIndice(origen);
    int fin = buscarIndice(destino);
    if (inicio == -1 || fin == -1) {
        return -1;
    }

    bool* visitado = new bool[cantidad];
    int* anterior = new int[cantidad];
    for (int i = 0; i < cantidad; i++) {
        visitado[i] = false;
        anterior[i] = -1;
    }

    // BFS
    Cola<int> pendientes;
    pendientes.encolar(inicio);
    visitado[inicio] = true;

    while (!pendientes.estaVacia() && !visitado[fin]) {
        int actual = pendientes.desencolar();

        auto it = adyacentes[actual].obtenerIterador();
        while (it.tieneSiguiente()) {
            int vecina = it.siguiente();
            if (!visitado[vecina]) {
                visitado[vecina] = true;
                anterior[vecina] = actual;
                pendientes.encolar(vecina);
            }
        }
    }

    int tramos = -1;
    if (visitado[fin]) {
        // Reconstruccion del camino
        tramos = 0;
        int paso = fin;
        while (paso != -1) {
            recorrido.insertarAlInicio(codigos[paso]);
            paso = anterior[paso];
            tramos++;
        }
        tramos--;
    }

    delete[] visitado;
    delete[] anterior;
    return tramos;
}
