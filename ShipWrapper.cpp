//
// Created by user on 5/27/23.
//

#include "ShipWrapper.h"
/*

    Handler(long precision) {
        mpfr_set_default_prec(precision);                                   // Consider changing this so that other mpfr
    }                                                                       // actions don't override this.
    Handler    for (auto &shipPair : shipList) {
            delete shipPair.second;
        }
        for (auto &enginePair : engineList) {
            delete enginePair.second;
        }
        mpfr_free_cache();

    }

    // ========== CREATORS ==========
    ShipWrapper* addShip(const std::string& name) {
        auto newShip = new ShipWrapper();
        if (shipList.find(name) != shipList.end()) {
            std::cerr << "[Handler::addShip] Ship " << name << " already exists." << std::endl;
        }
        shipList.insert({name, newShip});
        return newShip;
    }

    int createEngine(std::string name, const long double mass, const long double exhaustVelocity) {
        // There cannot be conflicts for multiple reasons. One, it makes it impossible to find the engine. Two,
        // It generates a memory leak. Three, it should prompt the user on the fact that it already exists.
        if (engineList.find(name) != engineList.end()) {
            std::cerr << "[Handler::createEngine] Engine " << name << " already exists." << std::endl;
            return 1;
        }

        auto newEngine = new Engine();

        mpfr_set_ld(newEngine->mass, mass, MPFR_RNDN);
        mpfr_set_ld(newEngine->exhaustVelocity, exhaustVelocity, MPFR_RNDN);

        newEngine->name = name;
        engineList.insert({name, newEngine});
        return 0;
    }

    // ========== SETTERS ==========
    */
/**
     * @brief Sets the mass of the ship.
     * @param mass To-be mass.
     * @param ship Pointer to the ship.
     * @return 0 if successful, 1 if not.
     *//*

    int setEngineDryMass(const long double mass, const std::string& name) {
        try {
            mpfr_set_ld(engineList.at(name)->mass, mass, MPFR_RNDN);
        } catch (const std::out_of_range& e) {
            std::cerr << "[Handler::setEngineDryMass] Engine " << name << " does not exist." << std::endl;
            return 1;
        }
        return 0;
    }

    */
/**
     * @brief Sets the engine exhaust velocity.
     * @param exhaustVelocity To-be exhaust velocity.
     * @param name Name of the engine.
     * @return 0 if successful, 1 if not.
     *//*

    int setEngineExhaustVelocity(const long double exhaustVelocity, const std::string& name) {
        try {
            mpfr_set_ld(engineList.at(name)->exhaustVelocity, exhaustVelocity, MPFR_RNDN);
        } catch (const std::out_of_range& e) {
            std::cerr << "[Handler::setEngineExhaustVelocity] Engine " << name << " does not exist." << std::endl;
            return 1;
        }
        return 0;
    }

    // ========== GETTERS ==========
    */
/**
     * @brief Gets the engine by name. Errors have to be handled.
     * @param name Name of the Engine.
     * @return Pointer to the Engine.
     *//*

    ShipWrapper * getShip(const std::string& name) {
        return shipList.at(name);
    }

    const Engine* getEngine(const std::string& name) {
        return engineList.at(name);
    }

    double getEngineMass(const std::string& name) {
        return mpfr_get_d(engineList.at(name)->mass, MPFR_RNDN);
    }

    double getEngineExhaustVelocity(const std::string& name) {
        return mpfr_get_d(engineList.at(name)->exhaustVelocity, MPFR_RNDN);
    }

    const std::unordered_map<std::string, Engine*>* getEngineList() {
        return &engineList;
    }

    std::unordered_map<std::string, ShipWrapper*>* getShipList() {
        return &shipList;
    }
*/

