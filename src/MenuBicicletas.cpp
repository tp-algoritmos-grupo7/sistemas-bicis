#include "MenuBicicletas.h"
#include <iostream>
#include <sstream>
#include <string>

bool leerEnteroValidado(const std::string &mensaje, int &valor)
{
    while (true)
    {
        std::cout << mensaje;

        std::string linea;
        if (!std::getline(std::cin, linea))
            return false;

        std::istringstream entrada(linea);
        int numero;
        std::string sobrante;
        if ((entrada >> numero) && !(entrada >> sobrante))
        {
            valor = numero;
            return true;
        }

        std::cout << "Entrada invalida. Ingrese un numero entero.\n";
    }
}

void agregarBicicletaDesdeConsola(RegistroBicicletas &registro)
{
    int id;
    do
    {
        if (!leerEnteroValidado("ID: ", id))
            return;
        if (id <= 0)
            std::cout << "El ID debe ser positivo.\n";
    } while (id <= 0);

    std::string codigoEstacion;
    bool codigoValido;
    do
    {
        std::cout << "Codigo de estacion: ";
        if (!std::getline(std::cin, codigoEstacion))
            return;
        codigoValido = !codigoEstacion.empty()
                       && codigoEstacion.find_first_of(" \t\r\n\v\f") == std::string::npos;
        if (!codigoValido)
            std::cout << "Ingrese un codigo no vacio y sin espacios.\n";
    } while (!codigoValido);

    int opcionTipo;
    do
    {
        if (!leerEnteroValidado("Tipo (1. Mecanica / 2. Electrica): ", opcionTipo))
            return;
        if (opcionTipo != 1 && opcionTipo != 2)
            std::cout << "Elija tipo 1 o 2.\n";
    } while (opcionTipo != 1 && opcionTipo != 2);

    int opcionEstado;
    do
    {
        if (!leerEnteroValidado("Estado (1. OK / 2. Averiada): ", opcionEstado))
            return;
        if (opcionEstado != 1 && opcionEstado != 2)
            std::cout << "Elija estado 1 o 2.\n";
    } while (opcionEstado != 1 && opcionEstado != 2);

    int kilometros;
    do
    {
        if (!leerEnteroValidado("Kilometros: ", kilometros))
            return;
        if (kilometros < 0)
            std::cout << "Los kilometros no pueden ser negativos.\n";
    } while (kilometros < 0);

    Bicicleta::Tipo tipo = Bicicleta::Tipo::Mecanica;
    if (opcionTipo == 2)
        tipo = Bicicleta::Tipo::Electrica;

    Bicicleta::Estado estado = Bicicleta::Estado::OK;
    if (opcionEstado == 2)
        estado = Bicicleta::Estado::Averiada;

    Bicicleta bicicleta(id, codigoEstacion, tipo, estado, kilometros);
    if (registro.agregar(bicicleta))
        std::cout << "Bicicleta agregada.\n";
    else
        std::cout << "Ya existe una bicicleta con ese ID.\n";
}

void mostrarMenuBicicletas(RegistroBicicletas &registro)
{
    int opcion = -1;

    do
    {
        std::cout << "\n=== Menu de bicicletas ===\n"
                  << "1. Cargar bicicletas desde archivo\n"
                  << "2. Agregar bicicleta\n"
                  << "3. Buscar por ID\n"
                  << "4. Listar por ID\n"
                  << "5. Contar averiadas\n"
                  << "6. Mostrar altura del ABB\n"
                  << "7. Listar bicicletas con mas de X kilometros\n"
                  << "0. Salir\n"
                  << "Seleccione una opcion: ";

        std::string linea;
        if (!std::getline(std::cin, linea))
            return;

        // Leer una linea completa evita que una entrada invalida bloquee cin.
        std::istringstream entrada(linea);
        std::string sobrante;
        if (!(entrada >> opcion) || (entrada >> sobrante))
        {
            std::cout << "Opcion invalida. Ingrese un numero del 0 al 7.\n";
            opcion = -1;
            continue;
        }

        switch (opcion)
        {
        case 1:
        {
            std::ostringstream diagnosticos;
            if (registro.cargarDesdeArchivo("datos/bicicletas.txt", diagnosticos))
                std::cout << "Carga completada.\n";
            else
                std::cout << "Carga con incidencias. Las bicicletas ya incorporadas se conservan.\n";
            break;
        }
        case 2:
            agregarBicicletaDesdeConsola(registro);
            break;
        case 3:
            std::cout << "buscar bicicleta por id\n";
            break;
        case 4:
            registro.listarPorId();
            break;
        case 5:
            std::cout << "contar bicicletas averiadas\n";
            break;
        case 6:
            std::cout << "mostrar altura del ABB\n";
            break;
        case 7:
            std::cout << "listar bicicletas con mas de X kilometros\n";
            break;
        case 0:
            std::cout << "Saliendo del menu de bicicletas.\n";
            break;
        default:
            std::cout << "Opcion invalida. Ingrese un numero del 0 al 7.\n";
            break;
        }
    } while (opcion != 0);
}
