#ifndef GESTION_VIAJES_HPP
#define GESTION_VIAJES_HPP

#include <string>
#include "TablaHash.hpp"
#include "Estacion.hpp"
#include "Viaje.hpp"
#include "Lista.hpp"
#include "RegistroBicicleta.h"

bool cargarViajes(const std::string& ruta, Lista<Viaje>& listaViajes);
void ejecutarMenuViajes(Lista<Viaje>& listaViajes, TablaHash& tablaEstaciones, RegistroBicicletas& registro);
void registrarViaje(Lista<Viaje>& listaViajes, TablaHash& tablaEstaciones, RegistroBicicletas& registro);
void registrarDevolucion(Lista<Viaje>& listaViajes, TablaHash& tablaEstaciones, RegistroBicicletas& registro);
void listarViajesPorUsuario(const Lista<Viaje>& listaViajes);

#endif // GESTION_VIAJES_HPP
