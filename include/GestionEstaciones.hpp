#ifndef GESTION_ESTACIONES_HPP
#define GESTION_ESTACIONES_HPP

#include <string>
#include <cstddef>
#include "TablaHash.hpp"
#include "GrafoCiclovias.hpp"

// Criterios para ordenar el listado de estaciones
const int ORDEN_POR_CAPACIDAD = 1;
const int ORDEN_POR_DISPONIBLES = 2;

bool cargarEstaciones(const std::string& rutaArchivo, TablaHash& tabla);

void mostrarEstacionPorCodigo(const TablaHash& tabla, const std::string& codigo);

void agregarEstacionInteractivo(TablaHash& tabla);

void eliminarEstacionInteractivo(TablaHash& tabla, GrafoCiclovias& grafo);

// Ordenamiento por seleccion hecho a mano sobre un arreglo de punteros a estaciones
void ordenarPorSeleccion(Estacion** estaciones, size_t cantidad, int criterio, bool descendente);

void listarEstacionesOrdenadas(const TablaHash& tabla);

void buscarRecorridoInteractivo(const TablaHash& tabla, const GrafoCiclovias& grafo);

void ejecutarMenuEstaciones(TablaHash& tabla, GrafoCiclovias& grafo);

#endif // GESTION_ESTACIONES_HPP
