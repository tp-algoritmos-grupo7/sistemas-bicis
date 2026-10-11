#ifndef REGISTRO_BICICLETA_H
#define REGISTRO_BICICLETA_H

#include "Abb.h"
#include <ostream>

class RegistroBicicletas
{
private:
    Abb bicicletas;

public:
    bool agregar(const Bicicleta &bicicleta);
    // Agrega al registro existente. Omite duplicados y devuelve false si los hubo.
    bool cargarDesdeArchivo(const std::string &ruta, std::ostream &diagnosticos);
};

#endif // REGISTRO_BICICLETA_H
