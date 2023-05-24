#include <vector>
#include <iostream>
#include <random>
#include "SpaceShip.h"

int main () {
    {
        auto *ship = new SpaceShip();
        Engine newEngine = Engine();
        mpfr_set_ld(newEngine.mass, 100, MPFR_RNDN);
        mpfr_set_ld(newEngine.exhaustVelocity, 1000, MPFR_RNDN);
        const char *name = "test";
        newEngine.name = new char[strlen(name) + 1];
        std::strcpy(newEngine.name, name);

        ship->addStage(100, 1, newEngine);
        ship->addStage(50, 20, newEngine);
        Engine newEngine2 = std::move(Engine());
        ship->printStats();
        delete (ship);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<long double> valRange(1, 1'000'000'000.0);
    std::uniform_int_distribution<uint> stageRange(10, 30);


    auto *ship = new SpaceShip();
    const uint stageCount = 10;
    for (uint i = 0; i < stageCount; i++) {
        Engine newEngine;
        mpfr_set_ld(newEngine.mass, valRange(gen), MPFR_RNDN);
        mpfr_set_ld(newEngine.exhaustVelocity, valRange(gen), MPFR_RNDN);

        const char* name = ("S" + std::to_string(i)).c_str();
        newEngine.name = new char[strlen(name) + 1];
        std::strcpy(newEngine.name, name);

        ship->addStage(valRange(gen), valRange(gen), newEngine);
    }
    ship->printStats();
    delete(ship);

    return 1;
}