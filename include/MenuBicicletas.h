#ifndef MENU_BICICLETAS_H
#define MENU_BICICLETAS_H

#include "RegistroBicicleta.h"
#include <string>

// Devuelve false si termina la entrada. Solo modifica valor al leer un entero valido.
bool leerEnteroValidado(const std::string &mensaje, int &valor);

void agregarBicicletaDesdeConsola(RegistroBicicletas &registro);
void buscarBicicletaPorId(RegistroBicicletas &registro);

void mostrarMenuBicicletas(RegistroBicicletas &registro);

#endif // MENU_BICICLETAS_H
