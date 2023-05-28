#include <vector>
#include <iostream>
#include <random>
#include "SpaceShipHandler.h"

int main () {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint> magnitude(2, 10);
    std::uniform_real_distribution<long double> valRange(0.001, 10);

    auto rnd = [&valRange, &gen, &magnitude]() {return valRange(gen) * pow(10, magnitude(gen));};

    SpaceShipHandler handler(1024);
    std::vector<SpaceShipWrapper*> ships;
    ships.reserve(3);
    for (int i = 0; i < 3; i++) {
        auto ship = handler.addShip();
        for (int j = 0; j < 5; j++) {
            auto engineName = "S" + std::to_string(i) + "." + std::to_string(j);
            handler.createEngine(engineName, rnd(), rnd());
            ship->addStage(rnd(), rnd(), handler.getEngine(engineName));
            ship->printStats();
        }
    }
   /* for (const auto& ship : *handler.getShipList()) {
        ship->printStats();
    }*/

    return 1;
}