#include <vector>
#include <iostream>
#include "SpaceShip.h"

int main () {
    auto* ship = new SpaceShip();
    Engine newEngine = Engine();
    mpfr_set_ld(newEngine.mass, 100, MPFR_RNDN);
    mpfr_set_ld(newEngine.exhaustVelocity, 1000, MPFR_RNDN);
    const char* name = "test";
    newEngine.name = new char[strlen(name) + 1];
    std::strcpy(newEngine.name, name);


    ship->addStage(100, 1, newEngine);
    ship->addStage(50, 20, newEngine);
    Engine newEngine2 = std::move(Engine());
    ship->printStats();
    delete(ship);
    return 1;
}