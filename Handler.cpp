//
// Created by user on 5/24/23.
//

#include "Handler.h"
Handler::Handler(long precision) {
    mpfr_set_default_prec(precision);                                   // Consider changing this so that other mpfr
}

Handler::~Handler() {
    for (auto &shipPair : shipList) {
        delete shipPair.second;
    }
    for (auto &enginePair : engineList) {
        delete enginePair.second;
    }
    mpfr_free_cache();
}

// ========== CREATORS ==========
ShipWrapper* Handler::addShip(const std::string& name) {
    auto newShip = new ShipWrapper();
    if (shipList.find(name) != shipList.end()) {
        std::cerr << "[Handler::addShip] Ship " << name << " already exists." << std::endl;
    }
    shipList.insert({name, newShip});
    return newShip;
}

int Handler::createEngine(const std::string& name, const std::string& mass, const std::string& exhaustVelocity) {
    // There cannot be conflicts for multiple reasons. One, it makes it impossible to find the engine. Two,
    // It generates a memory leak. Three, it should prompt the user on the fact that it already exists.
    if (engineList.find(name) != engineList.end()) {
        std::cerr << "[Handler::createEngine] Engine " << name << " already exists." << std::endl;
        return 1;
    }

    auto newEngine = new Engine();

    mpfr_set_str(newEngine->mass, mass.c_str(), 10, MPFR_RNDN);
    mpfr_set_str(newEngine->exhaustVelocity, exhaustVelocity.c_str(), 10, MPFR_RNDN);

    newEngine->name = name;
    engineList.insert({name, newEngine});
    return 0;
}

// ========== SETTERS ==========
/**
 * @brief Sets the mass of the ship.
 * @param mass To-be mass.
 * @param ship Pointer to the ship.
 * @return 0 if successful, 1 if not.
 */
int Handler::setEngineDryMass(const std::string& mass, const std::string& name) {
    try {
        mpfr_set_str(engineList.at(name)->mass, mass.c_str(), 10, MPFR_RNDN);
    } catch (const std::out_of_range& e) {
        std::cerr << "[Handler::setEngineDryMass] Engine " << name << " does not exist." << std::endl;
        return 1;
    }
    return 0;
}

/**
 * @brief Sets the engine exhaust velocity.
 * @param exhaustVelocity To-be exhaust velocity.
 * @param name Name of the engine.
 * @return 0 if successful, 1 if not.
 */
int Handler::setEngineExhaustVelocity(const std::string& exhaustVelocity, const std::string& name) {
    try {
        mpfr_set_str(engineList.at(name)->exhaustVelocity, exhaustVelocity.c_str(), 10, MPFR_RNDN);
    } catch (const std::out_of_range& e) {
        std::cerr << "[Handler::setEngineExhaustVelocity] Engine " << name << " does not exist." << std::endl;
        return 1;
    }
    return 0;
}

// ========== GETTERS ==========
/**
 * @brief Gets the engine by name. Errors have to be handled.
 * @param name Name of the Engine.
 * @return Pointer to the Engine.
 */
ShipWrapper* Handler::getShip(const std::string& name) {
    return shipList.at(name);
}

const Engine* Handler::getEngine(const std::string& name) {
    return engineList.at(name);
}

double Handler::getEngineMass(const std::string& name) {
    return mpfr_get_d(engineList.at(name)->mass, MPFR_RNDN);
}

double Handler::getEngineExhaustVelocity(const std::string& name) {
    return mpfr_get_d(engineList.at(name)->exhaustVelocity, MPFR_RNDN);
}

const std::unordered_map<std::string, Engine*>* Handler::getEngineList() {
    return &engineList;
}

std::unordered_map<std::string, ShipWrapper*>* Handler::getShipList() {
    return &shipList;
}