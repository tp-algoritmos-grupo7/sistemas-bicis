#include "GestionViajes.hpp"
#include <fstream>
#include <iostream>

bool cargarViajes(const std::string& rutaArchivo, Lista<Viaje>& listaViajes) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo abrir " << rutaArchivo << std::endl;
        return false;
    }

    int viajeId, biciId, fecha, duracion;
    long long dni;
    std::string origen, destino;

    while (archivo >> viajeId >> biciId >> dni >> origen >> destino >> fecha >> duracion) {
        Viaje v(viajeId, biciId, dni, origen, destino, fecha, duracion);
        listaViajes.insertarAlFinal(v);
    }

    archivo.close();
    return true;
}

void contarViajesPorEstacionYFechas(const Lista<Viaje>& listaViajes) {
    std::string estacionBuscada;
    int fechaInicio, fechaFin;

    std::cout << "\n--- Cantidad de Viajes por Estacion y Rango de Fechas ---\n";
    std::cout << "Codigo de estacion de origen (ej. PLZ): ";
    std::cin >> estacionBuscada;
    std::cout << "Fecha inicio (AAAAMMDD): ";
    std::cin >> fechaInicio;
    std::cout << "Fecha fin (AAAAMMDD): ";
    std::cin >> fechaFin;

    int contador = 0;
    auto it = listaViajes.obtenerIterador();
    while (it.tieneSiguiente()) {
        const Viaje& v = it.obtenerDato();
        if (v.obtenerEstacionOrigen() == estacionBuscada &&
            v.obtenerFecha() >= fechaInicio && v.obtenerFecha() <= fechaFin) {
            contador++;
        }
        it.avanzar();
    }

    std::cout << "\nResultado: Salieron " << contador << " viajes de la estacion '" 
              << estacionBuscada << "' entre " << fechaInicio << " y " << fechaFin << ".\n";
}

void buscarViajesPorDni(const Lista<Viaje>& listaViajes) {
    long long dniBuscado;
    std::cout << "\n--- Buscar Viajes por DNI de Usuario ---\n";
    std::cout << "Ingrese el DNI: ";
    std::cin >> dniBuscado;

    int totalMinutos = 0;
    int cantidadViajes = 0;
    auto it = listaViajes.obtenerIterador();

    std::cout << "\nViajes encontrados para el DNI " << dniBuscado << ":\n";
    while (it.tieneSiguiente()) {
        const Viaje& v = it.obtenerDato();
        if (v.obtenerDniUsuario() == dniBuscado) {
            v.mostrar();
            totalMinutos += v.obtenerDuracionMin();
            cantidadViajes++;
        }
        it.avanzar();
    }

    if (cantidadViajes == 0) {
        std::cout << "No se registran viajes para ese DNI.\n";
    } else {
        std::cout << "\nTotal de viajes: " << cantidadViajes 
                  << " | Total de minutos pedaleados: " << totalMinutos << " min\n";
    }
}

void ejecutarMenuViajes(const Lista<Viaje>& listaViajes) {
    int opcion = -1;
    while (opcion != 0) {
        std::cout << "\n----------------------------------------\n";
        std::cout << "   PARTE B: VIAJES Y COLAS DE ESPERA\n";
        std::cout << "----------------------------------------\n";
        std::cout << "1. Cantidad de viajes por estacion y rango de fechas\n";
        std::cout << "2. Buscar viajes de un usuario por DNI y minutos totales\n";
        std::cout << "0. Volver al Menu Principal\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;

        if (opcion == 1) {
            contarViajesPorEstacionYFechas(listaViajes);
        } else if (opcion == 2) {
            buscarViajesPorDni(listaViajes);
        } else if (opcion == 0) {
            std::cout << "\nVolviendo al menu principal...\n";
        } else {
            std::cout << "\nOpcion no valida. Intente de nuevo.\n";
        }
    }
}