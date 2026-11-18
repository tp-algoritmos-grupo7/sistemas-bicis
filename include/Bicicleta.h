#ifndef BICICLETA_H
#define BICICLETA_H
#include <string>
class Bicicleta
{
public:
    enum class Tipo
    {
        Mecanica,
        Electrica
    };

    enum class Estado
    {
        OK,
        Averiada
    };

private:
    int biciId;
    std::string codigoEstacion;
    Tipo tipoBicicleta;
    Estado estado;
    int kilometros;
    bool enViaje;
    bool enTaller;

public:
    Bicicleta();
    Bicicleta(int biciId, std::string codigoEstacion, Tipo tipoBicicleta, Estado estado, int kilometros);

    int getBiciId() const;
    std::string getCodigoEstacion() const;
    Tipo getTipo() const;
    Estado getEstado() const;
    int getKilometros() const;
    bool estaEnViaje() const;
    bool estaEnTaller() const;

    void setEstado(Estado estado);
    void setCodigoEstacion(std::string codigoEstacion);

    void comenzarViaje();
    void finalizarViaje(const std::string &codigoDestino);
    void ingresarAlTaller();
    void finalizarReparacion(const std::string &codigoDestino);
};

#endif // BICICLETA_H
