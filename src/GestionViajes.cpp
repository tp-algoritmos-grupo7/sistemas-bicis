#include "GestionViajes.hpp"
#include "MinHeapTaller.hpp"
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

// 2. Registrar Devolución (Con automatización de cola de espera)
void registrarDevolucion(Lista<Viaje>& listaViajes, TablaHash& tablaEstaciones) {
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

    
    if (destino->tieneUsuariosEsperando()) {
        long long dniUsuarioEspera = destino->desencolarUsuario();
        
        int biciId = 501; 
        int viajeId = rand() % 90000 + 10000;
        int fecha = 20261026;
        int duracionMin = 0;

        
        Viaje nuevoViaje(viajeId, biciId, dniUsuarioEspera, codigoDestino, codigoDestino, fecha, duracionMin);
        listaViajes.insertarAlFinal(nuevoViaje);

        
        std::ofstream archivo("datos/viajes.txt", std::ios::app);
        if (archivo.is_open()) {
            archivo << viajeId << " " 
                    << biciId << " " 
                    << dniUsuarioEspera << " " 
                    << codigoDestino << " " 
                    << codigoDestino << " " 
                    << fecha << " " 
                    << duracionMin << "\n";
            archivo.close();
        }

        std::cout << "\n[EXITO] Devolucion procesada.\n";
        std::cout << "[AUTOMATICO] Habia usuarios en espera. Se le asigno la bicicleta devuelta al usuario con DNI: " << dniUsuarioEspera << ".\n";
        std::cout << "Bicicletas actuales en la estacion (" << codigoDestino << "): " << destino->obtenerBicisDisponibles() << " (Stock compensado por asignacion inmediata).\n";
    } else {
        
        int stockDestino = destino->obtenerBicisDisponibles();
        destino->asignarBicisDisponibles(stockDestino + 1);

        std::cout << "\n[EXITO] Devolucion registrada con exito.\n";
        std::cout << "Bicicletas actuales en la estacion (" << codigoDestino << "): " << destino->obtenerBicisDisponibles() << "\n";
    }
}


// 1. Calcular cantidad de viajes por estación en un rango de fechas (Punto 1 de Parte B)
void contarViajesPorEstacionYFecha(const Lista<Viaje>& listaViajes) {
    std::cout << "\n--- CANTIDAD DE VIAJES POR ESTACION Y RANGO DE FECHAS ---\n";
    std::string estacionBuscada;
    int fechaInicio, fechaFin;

    std::cout << "Ingrese codigo de la estacion de origen: ";
    std::cin >> estacionBuscada;
    std::cout << "Ingrese fecha inicial (AAAAMMDD): ";
    std::cin >> fechaInicio;
    std::cout << "Ingrese fecha final (AAAAMMDD): ";
    std::cin >> fechaFin;

    if (listaViajes.estaVacia()) {
        std::cout << "No hay viajes registrados en el sistema.\n";
        return;
    }

    int contador = 0;
    Lista<Viaje>::Iterador it = listaViajes.obtenerIterador();
    while (it.tieneSiguiente()) {
        Viaje v = it.siguiente();
        if (v.obtenerEstacionOrigen() == estacionBuscada && 
            v.obtenerFecha() >= fechaInicio && 
            v.obtenerFecha() <= fechaFin) {
            contador++;
        }
    }

    std::cout << "\n[RESULTADO] Salieron " << contador << " viaje(s) desde la estacion " 
              << estacionBuscada << " entre el " << fechaInicio << " y el " << fechaFin << ".\n";
}

// 2. Consultar Historial de Viajes por Usuario con Total de Minutos (Punto 2 de Parte B)
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

    if (listaViajes.estaVacia()) {
        std::cout << "No hay viajes registrados en el sistema.\n";
        return;
    }

    int contadorViajes = 0;
    int minutosTotales = 0;

    Lista<Viaje>::Iterador it = listaViajes.obtenerIterador();
    while (it.tieneSiguiente()) {
        Viaje v = it.siguiente();
        if (v.obtenerDniUsuario() == dniBuscado) {
            contadorViajes++;
            minutosTotales += v.obtenerDuracionMin();
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
        std::cout << "\n----------------------------------------\n";
        std::cout << "[EXITO] Se encontraron " << contadorViajes << " viaje(s) para el DNI " << dniBuscado << ".\n";
        std::cout << "[TOTAL] Minutos totales pedaleados: " << minutosTotales << " mins.\n";
    }
}

// 3. Gestionar Taller de Reparaciones con Min-Heap (Punto 5 de Parte B)
void gestionarTallerReparaciones() {
    MinHeapTaller taller;
    int opcionTaller = -1;

    while (opcionTaller != 0) {
        std::cout << "\n=====================================\n";
        std::cout << "    TALLER DE REPARACIONES (MIN-HEAP)\n";
        std::cout << "=====================================\n";
        std::cout << "1. Ingresar bicicleta averiada\n";
        std::cout << "2. Atender/Extraer bicicleta mas urgente\n";
        std::cout << "0. Volver al menu de viajes\n";
        std::cout << "Seleccione una opcion: ";
        
        if (!(std::cin >> opcionTaller)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (opcionTaller == 1) {
            int biciId, prioridad;
            std::string tipo;
            std::cout << "Ingrese ID de la bicicleta: ";
            std::cin >> biciId;
            std::cout << "Ingrese tipo (Mecanica / Electrica): ";
            std::cin >> tipo;
            std::cout << "Ingrese prioridad (1: Muy urgente, 2: Urgente, 3: Normal): ";
            std::cin >> prioridad;

            taller.insertar(biciId, tipo, prioridad);
            std::cout << "[EXITO] Bicicleta ingresada al taller correctamente.\n";
        } 
        else if (opcionTaller == 2) {
            ElementoTaller b = taller.extraerMin();
            if (b.biciId == -1) {
                std::cout << "[ERROR] El taller no tiene bicicletas averiadas en espera.\n";
            } else {
                std::cout << "\n[ATENCION TALLER] Reparando bicicleta:\n";
                std::cout << " - ID Bici: " << b.biciId << "\n";
                std::cout << " - Tipo: " << b.tipo << "\n";
                std::cout << " - Prioridad asignada: " << b.prioridad << "\n";
            }
        }
    }
}
// Menú general de la Parte B
void ejecutarMenuViajes(Lista<Viaje>& listaViajes, TablaHash& tablaEstaciones) {
    int opcion = -1;
    do {
        std::cout << "\n=====================================\n";
        std::cout << "     GESTION DE VIAJES Y TALLER\n";
        std::cout << "=====================================\n";
        std::cout << "1. Registrar Viaje (Prestamo)\n";
        std::cout << "2. Registrar Devolucion (con cola de espera)\n";
        std::cout << "3. Consultar Viajes por Usuario (DNI y minutos)\n";
        std::cout << "4. Contar viajes por estacion y fecha (Punto 1)\n";
        std::cout << "5. Gestionar Taller de Reparaciones (Min-Heap)\n";
        std::cout << "6. Volver al Menu Principal\n";
        std::cout << "Seleccione una opcion: ";

        if (!(std::cin >> opcion)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        switch (opcion) {
            case 1:
                registrarViaje(listaViajes, tablaEstaciones);
                break;
            case 2:
                registrarDevolucion(listaViajes, tablaEstaciones);
                break;
            case 3:
                listarViajesPorUsuario(listaViajes);
                break;
            case 4:
                contarViajesPorEstacionYFecha(listaViajes);
                break;
            case 5:
                gestionarTallerReparaciones();
                break;
            case 6:
                std::cout << "Regresando al menu principal...\n";
                break;
            default:
                std::cout << "Opcion invalida.\n";
        }
    } while (opcion != 6);
}