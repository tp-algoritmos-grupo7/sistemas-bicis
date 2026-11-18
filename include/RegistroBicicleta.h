#ifndef REGISTRO_BICICLETA_H
#define REGISTRO_BICICLETA_H

#include "Abb.h"
#include "TablaHash.hpp"
#include <ostream>
#include <string>

class RegistroBicicletas
{
private:
    Abb bicicletas;
    bool tieneAnclajeLibre(const Estacion &estacion) const;

public:
    // Alta nueva: exige ID unico, estacion existente y anclaje libre.
    // Solo las bicicletas OK suman stock disponible. Si falla, no modifica nada y explica en aviso.
    bool agregar(const Bicicleta &bicicleta, TablaHash &estaciones, std::string &aviso);
    // Baja: descuenta stock solo si esta OK y su estacion todavia existe.
    // Rechaza un stock insuficiente sin borrar del ABB. Las averiadas no descuentan disponibles.
    bool eliminar(int id, TablaHash &estaciones, std::string &aviso);
    // Validan antes de actualizar el ABB y el stock. Una bici en viaje no se puede dar de baja.
    bool prestar(int id, const std::string &codigoOrigen, TablaHash &estaciones, std::string &aviso);
    bool devolver(int id, const std::string &codigoDestino, TablaHash &estaciones, std::string &aviso);
    // Puntero de lectura prestado del ABB; no liberarlo. nullptr si no existe.
    const Bicicleta *buscar(int id) const;
    void listarPorId() const;
    int contarAveriadas() const;
    int obtenerAltura() const;
    // Lista las bicicletas con mas de "kilometros" km y devuelve cuantas son.
    int listarConMasKilometros(int kilometros) const;
    // Carga el inventario existente sin modificar el stock inicial de las estaciones.
    // Omite duplicados y devuelve false si los hubo; conserva las bicicletas ya cargadas.
    bool cargarDesdeArchivo(const std::string &ruta, std::ostream &diagnosticos);
};

#endif // REGISTRO_BICICLETA_H
