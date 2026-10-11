#include <iostream>
#include "MenuBicicletas.h"

int main() {
    std::cout << "=== Sistema de Bicicletas Publicas UNAHUR ===" << std::endl;
    RegistroBicicletas registro;
    mostrarMenuBicicletas(registro);
    return 0;
}
