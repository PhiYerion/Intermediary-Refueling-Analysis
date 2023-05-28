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
        for (int j = 0; j < 5; j++) {
            auto engineName = "S" + std::to_string(i) + "." + std::to_string(j);
            handler.createEngine(engineName, valRange(gen), valRange(gen));
            ship->addStage(valRange(gen), valRange(gen), handler.getEngine(engineName));
            ship->printStats();
        }
    }
   /* for (const auto& ship : *handler.getShipList()) {
        ship->printStats();
    }*/

    return 1;
}