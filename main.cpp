#include <vector>
#include <iostream>
#include <random>
#include "SpaceShipHandler.h"

int main () {
    SpaceShipHandler handler;
    auto ship1 = handler.addShip();
    auto ship2 = handler.addShip();
    handler.addStage(ship1, 0, 100, 100, handler.createEngine(100, 1000, "test"));
    handler.addStage(ship1, 1, 50, 20, handler.createEngine(100, 1000, "test2"));
    handler.addStage(ship2, 0, 100, 100, handler.createEngine(100, 1000, "test3"));
    handler.addStage(ship2, 1, 50, 20, handler.createEngine(100, 1000, "test4"));
    handler.addStage(ship2, 2, 50, 20, handler.createEngine(100, 1000, "test5"));

    std::cout << "Ship 1: " << std::endl;
    handler.printStats(ship1);
    std::cout << "Ship 2: " << std::endl;
    handler.printStats(ship2);
    /*{
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
    delete(ship);*/

    return 1;
}