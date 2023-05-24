#include <vector>
#include <iostream>
#include "SpaceShip.h"

int main () {
    auto* ship = new SpaceShip();
    Engine newEngine;
    mpfr_set_ld(newEngine.mass, 100, MPFR_RNDN);
    mpfr_set_ld(newEngine.exhaustVelocity, 1000, MPFR_RNDN);
    newEngine.name = "S0";

    ship->addStage(100, 1, newEngine);
    ship->addStage(50, 20, newEngine);
    ship->printStats();
    delete(ship);
    return 1;
}