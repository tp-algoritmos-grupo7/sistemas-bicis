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

void agregarBicicletaDesdeConsola(RegistroBicicletas &registro, const TablaHash &estaciones)
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
        else if (estaciones.buscar(codigoEstacion) == nullptr)
        {
            std::cout << "No existe una estacion con ese codigo.\n";
            codigoValido = false;
        }
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

void buscarBicicletaPorId(RegistroBicicletas &registro)
{
    int id;
    do
    {
        if (!leerEnteroValidado("ID a buscar: ", id))
            return;

        if (id <= 0)
            std::cout << "El ID debe ser positivo.\n";
    } while (id <= 0);

    const Bicicleta *bicicleta = registro.buscar(id);

    if (bicicleta == nullptr)
    {
        std::cout << "No existe una bicicleta con ese ID.\n";
        return;
    }

    std::string tipo = "Mecanica";
    if (bicicleta->getTipo() == Bicicleta::Tipo::Electrica)
        tipo = "Electrica";

    std::string estado = "OK";
    if (bicicleta->getEstado() == Bicicleta::Estado::Averiada)
        estado = "Averiada";

    std::cout << "ID: " << bicicleta->getBiciId() << '\n'
              << "Estacion: " << bicicleta->getCodigoEstacion() << '\n'
              << "Tipo: " << tipo << '\n'
              << "Estado: " << estado << '\n'
              << "Kilometros: " << bicicleta->getKilometros() << '\n';
}

void mostrarMenuBicicletas(RegistroBicicletas &registro, const TablaHash &estaciones)
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
                  << "0. Volver al menu principal\n"
                  << "Seleccione una opcion: ";

        std::string linea;
        if (!std::getline(std::cin, linea))
            return;

        // Leer una linea completa evita que una entrada invalida bloquee cin.
        std::istringstream entrada(linea);
        std::string sobrante;
        if (!(entrada >> opcion) || (entrada >> sobrante))
        {
            std::cout << "Opcion invalida. Ingrese un numero del 0 al 6.\n";
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
            agregarBicicletaDesdeConsola(registro, estaciones);
            break;
        case 3:
            buscarBicicletaPorId(registro);
            break;
        case 4:
            registro.listarPorId();
            break;
        case 5:
            std::cout << "Bicicletas averiadas: " << registro.contarAveriadas() << '\n';
            break;
        case 6:
        {
            int altura = registro.obtenerAltura();
            if (altura == -1)
                std::cout << "El ABB esta vacio (altura: -1).\n";
            else
                std::cout << "Altura del ABB (en aristas): " << altura << '\n';
            break;
        }
        case 0:
            std::cout << "Volviendo al menu principal...\n";
            break;
        default:
            std::cout << "Opcion invalida. Ingrese un numero del 0 al 6.\n";
            break;
        }
    } while (opcion != 0);
}
