#include "MenuBicicletas.h"
#include <iostream>
#include <sstream>
#include <string>

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
            std::cout << "agregar bicicleta\n";
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
