#ifndef GRAFO_CICLOVIAS_HPP
#define GRAFO_CICLOVIAS_HPP

#include <string>
#include "Lista.hpp"
#include "TablaHash.hpp"

class GrafoCiclovias {
private:
    std::string* codigos;     // Arreglo de codigos de estaciones
    Lista<int>* adyacentes;   // Listas de adyacencia
    int cantidad;             // Cantidad de vertices

    // Devuelve el indice de la estacion o -1 si no existe
    int buscarIndice(const std::string& codigo) const;

public:
    GrafoCiclovias(const TablaHash& tabla);
    ~GrafoCiclovias();

    // Deshabilitar copia
    GrafoCiclovias(const GrafoCiclovias&) = delete;
    GrafoCiclovias& operator=(const GrafoCiclovias&) = delete;

    bool agregarCiclovia(const std::string& codigoA, const std::string& codigoB);

    bool cargarDesdeArchivo(const std::string& rutaArchivo);

    void quitarEstacion(const std::string& codigo);

    // Devuelve la cantidad de tramos del camino mas corto (BFS) o -1 si no hay camino
    int buscarRecorrido(const std::string& origen, const std::string& destino,
                        Lista<std::string>& recorrido) const;
};

#endif // GRAFO_CICLOVIAS_HPP
