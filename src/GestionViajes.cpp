#include "GestionViajes.hpp"
#include "MinHeapTaller.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <limits>

namespace {
    // Leer lineas completas permite alternar estos menus con los de estaciones y ABB.
    template <typename T>
    bool leerDato(const std::string& mensaje, T& valor) {
        std::string linea;
        while (true) {
            std::cout << mensaje;
            if (!std::getline(std::cin, linea))
                return false;
            std::istringstream entrada(linea);
            T leido;
            std::string sobrante;
            if ((entrada >> leido) && !(entrada >> sobrante)) {
                valor = leido;
                return true;
            }
            std::cout << "Entrada invalida. Ingrese un solo valor.\n";
        }
    }

    int fechaActual() {
        const std::time_t ahora = std::time(nullptr);
        const std::tm* fecha = std::localtime(&ahora);
        if (fecha == nullptr)
            return 0;
        return (fecha->tm_year + 1900) * 10000 + (fecha->tm_mon + 1) * 100 + fecha->tm_mday;
    }

    int siguienteIdViaje(const Lista<Viaje>& viajes) {
        int mayor = 0;
        auto it = viajes.obtenerIterador();
        while (it.tieneSiguiente()) {
            const int id = it.siguiente().obtenerViajeId();
            if (id > mayor)
                mayor = id;
        }
        return mayor == std::numeric_limits<int>::max() ? 0 : mayor + 1;
    }

    Viaje* buscarViajeEnCurso(Lista<Viaje>& viajes, int biciId) {
        auto it = viajes.obtenerIterador();
        while (it.tieneSiguiente()) {
            Viaje& viaje = it.siguiente();
            if (viaje.obtenerBiciId() == biciId && viaje.estaEnCurso())
                return &viaje;
        }
        return nullptr;
    }

    // El formato del TP describe viajes terminados. Los activos viven en memoria.
    bool guardarViajeFinalizado(const Viaje& viaje) {
        std::ofstream archivo("datos/viajes.txt", std::ios::app);
        if (!archivo.is_open())
            return false;
        archivo << viaje.obtenerViajeId() << ' ' << viaje.obtenerBiciId() << ' '
                << viaje.obtenerDniUsuario() << ' ' << viaje.obtenerEstacionOrigen() << ' '
                << viaje.obtenerEstacionDestino() << ' ' << viaje.obtenerFecha() << ' '
                << viaje.obtenerDuracionMin() << '\n';
        archivo.close();
        return static_cast<bool>(archivo);
    }

    // Tanto una devolucion como una reparacion pueden abastecer la cola de espera.
    void asignarBicicletaEnEspera(int biciId, int viajeId, int fecha, Estacion& destino,
                                 Lista<Viaje>& viajes, TablaHash& estaciones, RegistroBicicletas& registro) {
        std::string aviso;
        const std::string codigoDestino = destino.obtenerCodigo();
        if (registro.prestar(biciId, codigoDestino, estaciones, aviso)) {
            const long long dniEspera = destino.desencolarUsuario();
            Viaje nuevoViaje(viajeId, biciId, dniEspera, codigoDestino, "", fecha, 0, true);
            viajes.insertarAlFinal(nuevoViaje);
            std::cout << "[AUTOMATICO] Bicicleta " << biciId << " asignada al DNI " << dniEspera
                      << ". Nuevo viaje: " << viajeId << ".\n";
        } else {
            std::cout << "[AVISO] El usuario sigue en espera. " << aviso << '\n';
        }
    }
}

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
void registrarViaje(Lista<Viaje>& listaViajes, TablaHash& tablaEstaciones, RegistroBicicletas& registro) {
    std::cout << "\n--- REGISTRAR NUEVO VIAJE (PRESTAMO) ---\n";
    
    long long dni;
    if (!leerDato("Ingrese DNI del usuario: ", dni))
        return;
    if (!validarDni(dni)) {
        std::cout << "[ERROR] DNI invalido. Intente nuevamente.\n";
        return;
    }

    std::string codigoOrigen;
    if (!leerDato("Ingrese codigo de la Estacion de Origen: ", codigoOrigen))
        return;

    Estacion* origen = tablaEstaciones.buscar(codigoOrigen);
    if (origen == nullptr) {
        std::cout << "[ERROR] La estacion de origen no existe en el sistema.\n";
        return;
    }

    // Control de stock en origen
    if (origen->obtenerBicisDisponibles() <= 0) {
        std::cout << "[AVISO] La estacion no tiene bicicletas disponibles.\n";
        origen->encolarUsuario(dni);
        std::cout << "[EXITO] Usuario agregado a la cola de espera de " << codigoOrigen << ".\n";
        return;
    }

    int biciId;
    if (!leerDato("Ingrese ID de la bicicleta a retirar: ", biciId))
        return;
    if (buscarViajeEnCurso(listaViajes, biciId) != nullptr) {
        std::cout << "[ERROR] La bicicleta ya tiene un viaje en curso.\n";
        return;
    }

    const int viajeId = siguienteIdViaje(listaViajes);
    const int fecha = fechaActual();
    if (viajeId == 0 || fecha == 0) {
        std::cout << "[ERROR] No se pudo obtener un ID o una fecha para el viaje.\n";
        return;
    }

    std::string aviso;
    if (!registro.prestar(biciId, codigoOrigen, tablaEstaciones, aviso)) {
        std::cout << "[ERROR] " << aviso << '\n';
        return;
    }
    // El destino real y la duracion se completan al devolver la bicicleta.
    Viaje nuevoViaje(viajeId, biciId, dni, codigoOrigen, "", fecha, 0, true);
    listaViajes.insertarAlFinal(nuevoViaje);

    std::cout << "\n[EXITO] Viaje " << viajeId << " iniciado con bicicleta " << biciId << ".\n";
    std::cout << "Bicicletas restantes en origen (" << codigoOrigen << "): " << origen->obtenerBicisDisponibles() << "\n";
}

// 2. Registrar Devolución (Con automatización de cola de espera)
void registrarDevolucion(Lista<Viaje>& listaViajes, TablaHash& tablaEstaciones, RegistroBicicletas& registro) {
    std::cout << "\n--- REGISTRAR DEVOLUCION DE BICICLETA ---\n";
    
    int biciId;
    if (!leerDato("Ingrese ID de la bicicleta a devolver: ", biciId))
        return;
    Viaje* viaje = buscarViajeEnCurso(listaViajes, biciId);
    if (viaje == nullptr) {
        std::cout << "[ERROR] La bicicleta no tiene un viaje en curso para devolver.\n";
        return;
    }

    std::string codigoDestino;
    int duracionMin;
    if (!leerDato("Ingrese codigo de la Estacion donde se devuelve la bicicleta: ", codigoDestino) ||
        !leerDato("Ingrese duracion del viaje en minutos: ", duracionMin))
        return;
    if (duracionMin < 0) {
        std::cout << "[ERROR] La duracion no puede ser negativa.\n";
        return;
    }

    Estacion* destino = tablaEstaciones.buscar(codigoDestino);
    const Bicicleta* bicicleta = registro.buscar(biciId);
    const bool asignarEspera = destino != nullptr && destino->tieneUsuariosEsperando() &&
                              bicicleta != nullptr && bicicleta->getEstado() == Bicicleta::Estado::OK;
    const int nuevoId = asignarEspera ? siguienteIdViaje(listaViajes) : 0;
    const int nuevaFecha = asignarEspera ? fechaActual() : 0;
    if (asignarEspera && (nuevoId == 0 || nuevaFecha == 0)) {
        std::cout << "[ERROR] No se pudo preparar el viaje del primer usuario en espera.\n";
        return;
    }

    std::string aviso;
    if (!registro.devolver(biciId, codigoDestino, tablaEstaciones, aviso)) {
        std::cout << "[ERROR] " << aviso << '\n';
        return;
    }
    viaje->finalizar(codigoDestino, duracionMin);
    if (!guardarViajeFinalizado(*viaje))
        std::cout << "[AVISO] El viaje finalizo en memoria, pero no se pudo guardar en datos/viajes.txt.\n";

    std::cout << "\n[EXITO] Viaje " << viaje->obtenerViajeId() << " finalizado. Devolucion registrada.\n";
    if (asignarEspera) {
        asignarBicicletaEnEspera(biciId, nuevoId, nuevaFecha, *destino, listaViajes, tablaEstaciones, registro);
    }
    std::cout << "Bicicletas actuales en la estacion (" << codigoDestino << "): "
              << destino->obtenerBicisDisponibles() << '\n';
}


// 1. Calcular cantidad de viajes por estación en un rango de fechas (Punto 1 de Parte B)
void contarViajesPorEstacionYFecha(const Lista<Viaje>& listaViajes) {
    std::cout << "\n--- CANTIDAD DE VIAJES POR ESTACION Y RANGO DE FECHAS ---\n";
    std::string estacionBuscada;
    int fechaInicio, fechaFin;

    if (!leerDato("Ingrese codigo de la estacion de origen: ", estacionBuscada) ||
        !leerDato("Ingrese fecha inicial (AAAAMMDD): ", fechaInicio) ||
        !leerDato("Ingrese fecha final (AAAAMMDD): ", fechaFin))
        return;

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
    if (!leerDato("Ingrese el DNI a consultar: ", dniBuscado)) {
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
            if (!v.estaEnCurso())
                minutosTotales += v.obtenerDuracionMin();
            std::cout << "\n----------------------------------------\n";
            std::cout << "Viaje #" << contadorViajes << " (ID: " << v.obtenerViajeId() << ")\n";
            std::cout << " - Bici ID: " << v.obtenerBiciId() << "\n";
            std::cout << " - Origen: " << v.obtenerEstacionOrigen() << "\n";
            std::cout << " - Destino: " << (v.estaEnCurso() ? "Pendiente" : v.obtenerEstacionDestino()) << "\n";
            std::cout << " - Fecha: " << v.obtenerFecha() << "\n";
            if (v.estaEnCurso())
                std::cout << " - Estado: En curso\n";
            else
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
void gestionarTallerReparaciones(Lista<Viaje>& listaViajes, TablaHash& tablaEstaciones,
                                RegistroBicicletas& registro) {
    int opcionTaller = -1;

    while (opcionTaller != 0) {
        std::cout << "\n=====================================\n";
        std::cout << "    TALLER DE REPARACIONES (MIN-HEAP)\n";
        std::cout << "=====================================\n";
        std::cout << "1. Reportar averia e ingresar bicicleta al taller\n";
        std::cout << "2. Reparar y devolver a una estacion la bicicleta mas urgente\n";
        std::cout << "0. Volver al menu de viajes\n";
        if (!leerDato("Seleccione una opcion: ", opcionTaller))
            return;

        if (opcionTaller == 1) {
            int biciId, prioridad;
            if (!leerDato("Ingrese ID de la bicicleta: ", biciId) ||
                !leerDato("Ingrese prioridad (1: Muy urgente, 2: Urgente, 3: Normal): ", prioridad))
                return;

            std::string aviso;
            if (!registro.enviarATaller(biciId, prioridad, tablaEstaciones, aviso)) {
                std::cout << "[ERROR] " << aviso << '\n';
                continue;
            }
            const Bicicleta* bicicleta = registro.buscar(biciId);
            const int prioridadAsignada = bicicleta->getTipo() == Bicicleta::Tipo::Electrica ? 1 : prioridad;
            std::cout << "[EXITO] Bicicleta " << biciId << " ingresada al taller. Prioridad: "
                      << prioridadAsignada << ".\n";
        } 
        else if (opcionTaller == 2) {
            const ElementoTaller* proxima = registro.consultarProximaReparacion();
            if (proxima == nullptr) {
                std::cout << "[ERROR] El taller no tiene bicicletas pendientes.\n";
                continue;
            }
            // Copia antes de extraer: la raiz del heap cambiara al terminar la reparacion.
            const ElementoTaller bicicleta = *proxima;
            std::cout << "\n[ATENCION TALLER] ID: " << bicicleta.biciId << " | Tipo: " << bicicleta.tipo
                      << " | Prioridad: " << bicicleta.prioridad << '\n';
            std::string codigoDestino;
            if (!leerDato("Estacion de destino despues de la reparacion: ", codigoDestino))
                return;

            Estacion* destino = tablaEstaciones.buscar(codigoDestino);
            const bool asignarEspera = destino != nullptr && destino->tieneUsuariosEsperando();
            const int nuevoId = asignarEspera ? siguienteIdViaje(listaViajes) : 0;
            const int nuevaFecha = asignarEspera ? fechaActual() : 0;
            if (asignarEspera && (nuevoId == 0 || nuevaFecha == 0)) {
                std::cout << "[ERROR] No se pudo preparar el viaje del primer usuario en espera.\n";
                continue;
            }

            std::string aviso;
            if (!registro.repararProxima(codigoDestino, tablaEstaciones, aviso)) {
                std::cout << "[ERROR] " << aviso << '\n';
                continue;
            }
            std::cout << "[EXITO] Bicicleta " << bicicleta.biciId << " reparada y devuelta a "
                      << codigoDestino << ".\n";
            if (asignarEspera) {
                asignarBicicletaEnEspera(bicicleta.biciId, nuevoId, nuevaFecha, *destino,
                                       listaViajes, tablaEstaciones, registro);
            }
            std::cout << "Bicicletas actuales en la estacion (" << codigoDestino << "): "
                      << destino->obtenerBicisDisponibles() << '\n';
        } else if (opcionTaller != 0) {
            std::cout << "Opcion invalida.\n";
        }
    }
}
// Menú general de la Parte B
void ejecutarMenuViajes(Lista<Viaje>& listaViajes, TablaHash& tablaEstaciones, RegistroBicicletas& registro) {
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
        if (!leerDato("Seleccione una opcion: ", opcion))
            return;

        switch (opcion) {
            case 1:
                registrarViaje(listaViajes, tablaEstaciones, registro);
                break;
            case 2:
                registrarDevolucion(listaViajes, tablaEstaciones, registro);
                break;
            case 3:
                listarViajesPorUsuario(listaViajes);
                break;
            case 4:
                contarViajesPorEstacionYFecha(listaViajes);
                break;
            case 5:
                gestionarTallerReparaciones(listaViajes, tablaEstaciones, registro);
                break;
            case 6:
                std::cout << "Regresando al menu principal...\n";
                break;
            default:
                std::cout << "Opcion invalida.\n";
        }
    } while (opcion != 6);
}
