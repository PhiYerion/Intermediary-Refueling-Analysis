#include <vector>
#include <iostream>
#include <random>
#include "SpaceShipHandler.h"

int main () {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<long double> valRange(1, 1'000'000'000.0);

    SpaceShipHandler handler(1024);
    std::vector<SpaceShipWrapper*> ships;
    ships.reserve(3);
    for (int i = 0; i < 3; i++) {
        auto ship = handler.addShip();
        handler.createEngine("S1fds", valRange(gen), valRange(gen));
        handler.createEngine("S2fds", valRange(gen), valRange(gen));
        ship->addStage(valRange(gen), valRange(gen), handler.getEngine("S1fds"));
        ship->addStage(valRange(gen), valRange(gen), handler.getEngine("S2fds"));
        ship->printStats();
    }
   /* for (const auto& ship : *handler.getShipList()) {
        ship->printStats();
    }*/

    return 1;
}