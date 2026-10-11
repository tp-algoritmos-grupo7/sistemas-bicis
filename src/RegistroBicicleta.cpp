#include "RegistroBicicleta.h"
#include <fstream>
#include <iostream>

bool RegistroBicicletas::agregar(const Bicicleta &bicicleta)
{
    return bicicletas.insertar(bicicleta);
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

    // La consigna permite asumir registros bien formados, sin cabecera.
    while (archivo >> id >> codigoEstacion >> textoTipo >> textoEstado >> kilometros)
    {
        Bicicleta::Tipo tipo = textoTipo == "Mecanica"
                                  ? Bicicleta::Tipo::Mecanica
                                  : Bicicleta::Tipo::Electrica;
        Bicicleta::Estado estado = textoEstado == "OK"
                                      ? Bicicleta::Estado::OK
                                      : Bicicleta::Estado::Averiada;

        Bicicleta bicicleta(id, codigoEstacion, tipo, estado, kilometros);
        if (!agregar(bicicleta))
        {
            diagnosticos << "ID duplicado omitido: " << id << '\n';
            cargaCompleta = false;
        }
    }

    return cargaCompleta;
}
