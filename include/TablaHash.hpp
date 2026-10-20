#ifndef TABLA_HASH_HPP
#define TABLA_HASH_HPP

#include <string>
#include <cstddef>
#include "Lista.hpp"
#include "Estacion.hpp"

class TablaHash {
private:
    Lista<Estacion*>* tabla; // Arreglo dinamico de listas
    size_t capacidad;        // Tamaño M (numero primo)
    size_t cantidad;         // Total de elementos N
    size_t colisiones;       // Contador de colisiones 

    // Funcion hash: suma de caracteres + modulo primo
    size_t funcionHash(const std::string& clave) const;

public:
    // Constructor, capacidad inicial prima=31
    TablaHash(size_t capacidadInicial = 31);
    
    // Destructor (libera el arreglo y las estaciones almacenadas)
    ~TablaHash();

    // Deshabilitar copia para evitar doble liberacion de memoria 
    TablaHash(const TablaHash&) = delete;
    TablaHash& operator=(const TablaHash&) = delete;

    // Operaciones principales
    bool insertar(Estacion* estacion);
    Estacion* buscar(const std::string& codigo) const;
    bool eliminar(const std::string& codigo);


    size_t obtenerColisiones() const;
    double obtenerFactorCarga() const;
    size_t obtenerCantidad() const;
    size_t obtenerCapacidad() const;


    void obtenerTodas(Estacion** arregloSalida) const; //falta el ordenamiento
    void listarOcupacionBaja(double porcentajeLimite = 20.0) const;

    // ver tabla hash completa
    void imprimirTabla(bool soloOcupados = false) const;
};

#endif // TABLA_HASH_HPP