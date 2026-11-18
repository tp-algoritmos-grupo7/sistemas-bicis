#include "RegistroBicicleta.h"
#include <fstream>
#include <iostream>

bool RegistroBicicletas::agregar(const Bicicleta &bicicleta, TablaHash &estaciones, std::string &aviso)
{
    aviso.clear();
    if (bicicleta.getBiciId() <= 0 || bicicleta.getKilometros() < 0)
    {
        aviso = "El ID debe ser positivo y los kilometros no pueden ser negativos.";
        return false;
    }

    if (bicicletas.buscar(bicicleta.getBiciId()) != nullptr)
    {
        aviso = "Ya existe una bicicleta con ese ID.";
        return false;
    }

    const std::string codigo = bicicleta.getCodigoEstacion();
    Estacion *estacion = estaciones.buscar(codigo);
    if (estacion == nullptr)
    {
        aviso = "No existe la estacion '" + codigo + "'.";
        return false;
    }

    // Las averiadas ocupan anclajes, pero no forman parte del stock disponible.
    const int disponibles = estacion->obtenerBicisDisponibles();
    const int averiadas = bicicletas.contarAveriadasEnEstacion(codigo);
    if (disponibles >= estacion->obtenerCapacidadAnclajes() ||
        averiadas >= estacion->obtenerCapacidadAnclajes() - disponibles)
    {
        aviso = "La estacion '" + codigo + "' no tiene anclajes libres.";
        return false;
    }

    // Solo modificamos las estructuras una vez que todas las validaciones pasaron.
    if (!bicicletas.insertar(bicicleta))
    {
        aviso = "Ya existe una bicicleta con ese ID.";
        return false;
    }
    if (bicicleta.getEstado() == Bicicleta::Estado::OK)
        estacion->asignarBicisDisponibles(disponibles + 1);
    return true;
}

bool RegistroBicicletas::eliminar(int id, TablaHash &estaciones, std::string &aviso)
{
    aviso.clear();
    const Bicicleta *bicicleta = bicicletas.buscar(id);
    if (bicicleta == nullptr)
    {
        aviso = "No existe una bicicleta con ese ID.";
        return false;
    }

    Estacion *estacion = estaciones.buscar(bicicleta->getCodigoEstacion());
    const bool descontarStock = estacion != nullptr && bicicleta->getEstado() == Bicicleta::Estado::OK;
    if (descontarStock && estacion->obtenerBicisDisponibles() <= 0)
    {
        aviso = "El stock de la estacion es inconsistente: no hay bicicletas disponibles para descontar.";
        return false;
    }

    // Guardamos los datos necesarios antes de borrar: el ABB puede copiar el sucesor
    // sobre el nodo eliminado cuando este tiene dos hijos.
    if (!bicicletas.eliminar(id))
    {
        aviso = "No se pudo eliminar la bicicleta.";
        return false;
    }
    if (descontarStock)
        estacion->asignarBicisDisponibles(estacion->obtenerBicisDisponibles() - 1);
    // El TP permite eliminar estaciones sin reubicar sus bicicletas.
    // Si ya no existe la estacion, la baja solo afecta al registro.
    return true;
}

const Bicicleta *RegistroBicicletas::buscar(int id) const
{
    return bicicletas.buscar(id);
}

void RegistroBicicletas::listarPorId() const
{
    if (bicicletas.contarNodos() == 0)
    {
        std::cout << "No hay bicicletas registradas.\n";
        return;
    }

    std::cout << "ID Estacion Tipo Estado Kilometros\n";
    bicicletas.recorridoInorden();
}

int RegistroBicicletas::contarAveriadas() const
{
    return bicicletas.contarBicicletasAveriadas();
}

int RegistroBicicletas::obtenerAltura() const
{
    return bicicletas.altura();
}

int RegistroBicicletas::listarConMasKilometros(int kilometros) const
{
    return bicicletas.listarConMasKilometros(kilometros);
}

bool RegistroBicicletas::cargarDesdeArchivo(const std::string &ruta, std::ostream &diagnosticos)
{
    std::ifstream archivo(ruta);
    if (!archivo.is_open())
    {
        diagnosticos << "No se pudo abrir el archivo: " << ruta << '\n';
        return false;
    }

    int id;
    std::string codigoEstacion;
    std::string textoTipo;
    std::string textoEstado;
    int kilometros;
    bool cargaCompleta = true;

    // Se asume que el archivo está bien formado, sin cabecera.
    while (archivo >> id >> codigoEstacion >> textoTipo >> textoEstado >> kilometros)
    {
        Bicicleta::Tipo tipo = textoTipo == "Mecanica"
                                  ? Bicicleta::Tipo::Mecanica
                                  : Bicicleta::Tipo::Electrica;
        Bicicleta::Estado estado = textoEstado == "OK"
                                      ? Bicicleta::Estado::OK
                                      : Bicicleta::Estado::Averiada;

        Bicicleta bicicleta(id, codigoEstacion, tipo, estado, kilometros);
        // El stock inicial ya viene de estaciones.txt; cargar no es dar de alta.
        if (!bicicletas.insertar(bicicleta))
        {
            diagnosticos << "ID duplicado omitido: " << id << '\n';
            cargaCompleta = false;
        }
    }

    return cargaCompleta;
}
