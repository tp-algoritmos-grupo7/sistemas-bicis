#ifndef BICICLETA_H
#define BICICLETA_H

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
    int codigoEstacion;
    Tipo tipoBicicleta;
    Estado estado;
    int kilometros;

public:
    Bicicleta();
    Bicicleta(int biciId, int codigoEstacion, Tipo tipoBicicleta, Estado estado, int kilometros);

    int getBiciId() const;
    int getCodigoEstacion() const;
    Tipo getTipo() const;
    Estado getEstado() const;
    int getKilometros() const;

    void setEstado(Estado estado);
    void setCodigoEstacion(int codigoEstacion);

    void comenzarViaje();
    void finalizarViaje();
};

#endif // BICICLETA_H