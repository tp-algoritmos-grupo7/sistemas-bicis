#include "GestionViajes.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

// Función para validar que el DNI sea lógico
bool validarDni(long long dni) {
    return dni > 0 && dni > 999999; 
}

// Carga los viajes desde el archivo de texto y los almacena en la lista enlazada
bool cargarViajes(const std::string& ruta, Lista<Viaje>& listaViajes) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        return false;
    }

    int viajeId, biciId, fecha, duracionMin;
    long long dniUsuario;
    std::string estacionOrigen, estacionDestino;

    // Lee directamente separando por espacios
    while (archivo >> viajeId >> biciId >> dniUsuario >> estacionOrigen >> estacionDestino >> fecha >> duracionMin) {
        Viaje v(viajeId, biciId, dniUsuario, estacionOrigen, estacionDestino, fecha, duracionMin);
        listaViajes.insertarAlFinal(v);
    }
    
    archivo.close();
    return true;
}

// 1. Registrar Préstamo
void registrarViaje(Lista<Viaje>& listaViajes, TablaHash& tablaEstaciones) {
    std::cout << "\n--- REGISTRAR NUEVO VIAJE (PRESTAMO) ---\n";
    
    long long dni;
    std::cout << "Ingrese DNI del usuario: ";
    if (!(std::cin >> dni) || !validarDni(dni)) {
        std::cout << "[ERROR] DNI invalido. Intente nuevamente.\n";
        return;
    }

    std::string codigoOrigen, codigoDestino;
    std::cout << "Ingrese codigo de la Estacion de Origen: ";
    std::cin >> codigoOrigen;

    Estacion* origen = tablaEstaciones.buscar(codigoOrigen);
    if (origen == nullptr) {
        std::cout << "[ERROR] La estacion de origen no existe en el sistema.\n";
        return;
    }

    // Control de stock en origen
    if (origen->obtenerBicisDisponibles() <= 0) {
        std::cout << "[AVISO] La estacion no tiene bicicletas disponibles.\n";
        std::cout << "Desea registrar al usuario en la cola de espera? (s/n): ";
        char opcion;
        std::cin >> opcion;
        if (opcion == 's' || opcion == 'S') {
            origen->encolarUsuario(dni);
            std::cout << "[EXITO] Usuario agregado a la cola de espera de " << codigoOrigen << ".\n";
        }
        return;
    }

    std::cout << "Ingrese codigo de la Estacion de Destino: ";
    std::cin >> codigoDestino;

    Estacion* destino = tablaEstaciones.buscar(codigoDestino);
    if (destino == nullptr) {
        std::cout << "[ERROR] La estacion de destino no existe en el sistema.\n";
        return;
    }

    int biciId;
    std::cout << "Ingrese ID de la bicicleta a retirar: ";
    std::cin >> biciId;

    // Restar stock en origen
    int stockOrigen = origen->obtenerBicisDisponibles();
    origen->asignarBicisDisponibles(stockOrigen - 1);

    int viajeId = rand() % 90000 + 10000;
    int fecha = 20261026;
    int duracionMin = 0;

    Viaje nuevoViaje(viajeId, biciId, dni, codigoOrigen, codigoDestino, fecha, duracionMin);
    listaViajes.insertarAlFinal(nuevoViaje);
    
    // Guardar en el archivo de texto usando ESPACIOS para mantener el formato original
    std::ofstream archivo("datos/viajes.txt", std::ios::app);
    if (archivo.is_open()) {
        archivo << viajeId << " " 
                << biciId << " " 
                << dni << " " 
                << codigoOrigen << " " 
                << codigoDestino << " " 
                << fecha << " " 
                << duracionMin << "\n";
        archivo.close();
    }

    std::cout << "\n[EXITO] ¡Viaje registrado correctamente!\n";
    std::cout << "Bicicletas restantes en origen (" << codigoOrigen << "): " << origen->obtenerBicisDisponibles() << "\n";
}

// 2. Registrar Devolución
void registrarDevolucion(TablaHash& tablaEstaciones) {
    std::cout << "\n--- REGISTRAR DEVOLUCION DE BICICLETA ---\n";
    
    std::string codigoDestino;
    std::cout << "Ingrese codigo de la Estacion donde se devuelve la bicicleta: ";
    std::cin >> codigoDestino;

    Estacion* destino = tablaEstaciones.buscar(codigoDestino);
    if (destino == nullptr) {
        std::cout << "[ERROR] La estacion ingresada no existe.\n";
        return;
    }

    if (destino->obtenerBicisDisponibles() >= destino->obtenerCapacidadAnclajes()) {
        std::cout << "[ERROR] La estacion esta completa. No hay anclajes disponibles.\n";
        return;
    }

    // Sumar stock en destino
    int stockDestino = destino->obtenerBicisDisponibles();
    destino->asignarBicisDisponibles(stockDestino + 1);

    std::cout << "\n[EXITO] Devolucion registrada con exito.\n";
    std::cout << "Bicicletas actuales en la estacion (" << codigoDestino << "): " << destino->obtenerBicisDisponibles() << "\n";

    if (destino->tieneUsuariosEsperando()) {
        long long dniSiguiente = destino->desencolarUsuario();
        std::cout << "[AVISO] Habia usuarios en espera. Se notifico al usuario con DNI: " << dniSiguiente << ".\n";
    }
}

// 3. Consultar / Listar Historial de Viajes por Usuario (DNI)
void listarViajesPorUsuario(const Lista<Viaje>& listaViajes) {
    std::cout << "\n--- CONSULTAR VIAJES POR DNI DE USUARIO ---\n";
    long long dniBuscado;
    std::cout << "Ingrese el DNI a consultar: ";
    if (!(std::cin >> dniBuscado)) {
        std::cout << "[ERROR] DNI invalido.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }

    std::cout << "\nBuscando viajes para el DNI: " << dniBuscado << "...\n";
    
    if (listaViajes.estaVacia()) {
        std::cout << "No hay viajes registrados en el sistema.\n";
        return;
    }

    int contadorViajes = 0;

    // Recorremos la lista utilizando el Iterador provisto por tu clase Lista
    Lista<Viaje>::Iterador it = listaViajes.obtenerIterador();
    while (it.tieneSiguiente()) {
        Viaje v = it.siguiente();

        if (v.obtenerDniUsuario() == dniBuscado) {
            contadorViajes++;
            std::cout << "\n----------------------------------------\n";
            std::cout << "Viaje #" << contadorViajes << " (ID: " << v.obtenerViajeId() << ")\n";
            std::cout << " - Bici ID: " << v.obtenerBiciId() << "\n";
            std::cout << " - Origen: " << v.obtenerEstacionOrigen() << "\n";
            std::cout << " - Destino: " << v.obtenerEstacionDestino() << "\n";
            std::cout << " - Fecha: " << v.obtenerFecha() << "\n";
            std::cout << " - Duracion: " << v.obtenerDuracionMin() << " mins\n";
        }
    }

    if (contadorViajes == 0) {
        std::cout << "[AVISO] No se encontraron viajes registrados para el DNI " << dniBuscado << ".\n";
    } else {
        std::cout << "\n[EXITO] Se encontraron " << contadorViajes << " viaje(s) para el DNI " << dniBuscado << ".\n";
    }
}

// Menú general de la Parte B
void ejecutarMenuViajes(Lista<Viaje>& listaViajes, TablaHash& tablaEstaciones) {
    int opcion;
    do {
        std::cout << "\n===============================\n";
        std::cout << "     GESTION DE VIAJES         \n";
        std::cout << "===============================\n";
        std::cout << "1. Registrar Viaje (Prestamo)\n";
        std::cout << "2. Registrar Devolucion\n";
        std::cout << "3. Consultar Viajes por Usuario (DNI)\n";
        std::cout << "4. Volver al Menu Principal\n";
        std::cout << "Seleccione una opcion: ";
        
        if (!(std::cin >> opcion)) {
            std::cout << "Entrada invalida.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        switch (opcion) {
            case 1:
                registrarViaje(listaViajes, tablaEstaciones);
                break;
            case 2:
                registrarDevolucion(tablaEstaciones);
                break;
            case 3:
                listarViajesPorUsuario(listaViajes);
                break;
            case 4:
                std::cout << "Regresando al menu principal...\n";
                break;
            default:
                std::cout << "Opcion no valida. Ingrese una opcion correcta.\n";
        }
    } while (opcion != 4);
}