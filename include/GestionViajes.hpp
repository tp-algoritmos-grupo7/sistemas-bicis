#ifndef GESTION_VIAJES_HPP
#define GESTION_VIAJES_HPP

#include <string>
#include "Lista.hpp"
#include "Viaje.hpp"

bool cargarViajes(const std::string& rutaArchivo, Lista<Viaje>& listaViajes);

void contarViajesPorEstacionYFechas(const Lista<Viaje>& listaViajes);

void buscarViajesPorDni(const Lista<Viaje>& listaViajes);

void ejecutarMenuViajes(const Lista<Viaje>& listaViajes);

#endif // GESTION_VIAJES_HPP