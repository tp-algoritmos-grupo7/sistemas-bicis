#ifndef MENU_BICICLETAS_H
#define MENU_BICICLETAS_H

#include "RegistroBicicleta.h"
#include "TablaHash.hpp"
#include <string>

// Devuelve false si termina la entrada. Solo modifica valor al leer un entero valido.
bool leerEnteroValidado(const std::string &mensaje, int &valor);

void agregarBicicletaDesdeConsola(RegistroBicicletas &registro, TablaHash &estaciones);
void buscarBicicletaPorId(RegistroBicicletas &registro);
void eliminarBicicletaPorId(RegistroBicicletas &registro, TablaHash &estaciones);
void listarBicicletasParaRevision(const RegistroBicicletas &registro);

void mostrarMenuBicicletas(RegistroBicicletas &registro, TablaHash &estaciones);

#endif // MENU_BICICLETAS_H
