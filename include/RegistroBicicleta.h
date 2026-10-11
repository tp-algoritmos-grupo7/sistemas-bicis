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
    // Puntero de lectura prestado del ABB; no liberarlo. nullptr si no existe.
    const Bicicleta *buscar(int id) const;
    void listarPorId() const;
    int contarAveriadas() const;
    int obtenerAltura() const;
    // Agrega al registro existente. Omite duplicados y devuelve false si los hubo.
    bool cargarDesdeArchivo(const std::string &ruta, std::ostream &diagnosticos);
};

#endif // REGISTRO_BICICLETA_H
