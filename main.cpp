#include <vector>
#include <iostream>
#include <random>
#include <cstdlib>
#include "SpaceShipHandler.h"

int main () {
    std::vector<SpaceShipWrapper*> ships;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint> magnitude(2, 10);
    std::uniform_real_distribution<long double> valRange(0.001, 10);

    auto rnd = [&valRange, &gen, &magnitude]() {return valRange(gen) * pow(10, magnitude(gen));};

    SpaceShipHandler handler(8192);
    ships.reserve(3);
    for (int i = 0; i < 3; i++) {
        auto ship = handler.addShip();
        for (int j = 0; j < 5; j++) {
            auto engineName = "S" + std::to_string(i) + "." + std::to_string(j);
            long double vals[4] = {rnd(), rnd(), rnd(), rnd()};
            handler.createEngine(engineName, vals[0], vals[1]);
            ship->addStage(vals[2], vals[3], handler.getEngine(engineName));
            //printf("ln((%.64Lf + %.64Lf + %.64Lf) / (%.64Lf + %.64Lf)) * %.64Lf\n", vals[0], vals[2], vals[3], vals[0], vals[2], vals[1]);
        }
        ship->printStats();
    }
   /* for (const auto& ship : *handler.getShipList()) {
        ship->printStats();
    }*/

    return 1;
}