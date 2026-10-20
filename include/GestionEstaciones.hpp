#ifndef GESTION_ESTACIONES_HPP
#define GESTION_ESTACIONES_HPP

#include <string>
#include "TablaHash.hpp"

bool cargarEstaciones(const std::string& rutaArchivo, TablaHash& tabla);

void mostrarEstacionPorCodigo(const TablaHash& tabla, const std::string& codigo);

void agregarEstacionInteractivo(TablaHash& tabla);

void eliminarEstacionInteractivo(TablaHash& tabla);

void ejecutarMenuEstaciones(TablaHash& tabla);

#endif // GESTION_ESTACIONES_HPP
