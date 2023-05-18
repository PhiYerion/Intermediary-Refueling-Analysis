#include <vector>
#include <iostream>
#include "SpaceShip.h"

int main () {
    auto* ship = new SpaceShip();
    ship->addStage(
            ship->toPrecise(100),
            ship->toPrecise(1), {
                ship->toPrecise(100),
                    ship->toPrecise(100), "S1"});
    ship->addStage(
            ship->toPrecise(50),
            ship->toPrecise(20), {
                    ship->toPrecise(5),
                    ship->toPrecise(1000), "S1"});
    ship->printStats();
    delete(ship);
    return 1;
}