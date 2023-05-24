#include <vector>
#include <iostream>
#include "SpaceShip.h"

int main () {
    auto* ship = new SpaceShip();

    ship->addStage(100, 1, 100, 1000, "S0");
    ship->addStage(50, 20, 1000, 100, "S1");
    ship->printStats();
    delete(ship);
    return 1;
}