//
// Created by user on 5/24/23.
//

#include "SpaceShip.h"
#include "Stage.h"
#include "Engine.h"
#include "cstdio"
#include <mpfr.h>
#include <vector>
#include <cmath>
#include <cstring>
#include <unordered_map>
#include "iostream"
#include "SpaceShipWrapper.h"

#ifndef IRA_SPACESHIPHANDLER_H
#define IRA_SPACESHIPHANDLER_H

/**
 * @brief A largely stable class that abstracts SpaceShip class
 * @details Intended as a long term interface for the space ship class.
 *          This class won't change unless a new major version comes out (excluding 0.X.X).
 */
class SpaceShipHandler {
protected:
    // Hash map will not be implemented for SpaceShipWrapper. This vector is for keeping internal tabs on the ships
    // created (mainly for memory management but also other internal functions).
    std::unordered_map<std::string, SpaceShipWrapper*> shipList;                         /**< Pointer list to the ships. */

    // It is the handler's job to keep track of engines, all other references to engines are (or at least should be)
    // immutable.
    // Hash map for the engines. When accessing engines, it should be done by its name. A name will not repeat and is
    // required for the engine.
    std::unordered_map<std::string, Engine*> engineList;             /**< Hash map for engine list by name. */

public:
    /**
     * @brief Construct a new Space Ship Handler object.
     * @param precision Sets "global" mpfr precision, not just for this handler.
     * @note There shouldn't be more than one of these at a time.
     */
    SpaceShipHandler(long precision) {
        mpfr_set_default_prec(precision);                                   // Consider changing this so that other mpfr
    }                                                                       // actions don't override this.
    ~SpaceShipHandler() {
        for (auto &shipPair : shipList) {
            delete shipPair.second;
        }
        for (auto &enginePair : engineList) {
            delete enginePair.second;
        }
        mpfr_free_cache();

    }

    // ========== CREATORS ==========
    SpaceShipWrapper* addShip(const std::string& name) {
        auto newShip = new SpaceShipWrapper();
        if (shipList.find(name) != shipList.end()) {
            std::cerr << "[SpaceShipHandler::addShip] Ship " << name << " already exists." << std::endl;
        }
        shipList.insert({name, newShip});
        return newShip;
    }

    int createEngine(std::string name, const long double mass, const long double exhaustVelocity) {
        // There cannot be conflicts for multiple reasons. One, it makes it impossible to find the engine. Two,
        // It generates a memory leak. Three, it should prompt the user on the fact that it already exists.
        if (engineList.find(name) != engineList.end()) {
            std::cerr << "[SpaceShipHandler::createEngine] Engine " << name << " already exists." << std::endl;
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
    /**
     * @brief Sets the mass of the ship.
     * @param mass To-be mass.
     * @param ship Pointer to the ship.
     * @return 0 if successful, 1 if not.
     */
    int setEngineDryMass(const long double mass, const std::string& name) {
        try {
            mpfr_set_ld(engineList.at(name)->mass, mass, MPFR_RNDN);
        } catch (const std::out_of_range& e) {
            std::cerr << "[SpaceShipHandler::setEngineDryMass] Engine " << name << " does not exist." << std::endl;
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
    int setEngineExhaustVelocity(const long double exhaustVelocity, const std::string& name) {
        try {
            mpfr_set_ld(engineList.at(name)->exhaustVelocity, exhaustVelocity, MPFR_RNDN);
        } catch (const std::out_of_range& e) {
            std::cerr << "[SpaceShipHandler::setEngineExhaustVelocity] Engine " << name << " does not exist." << std::endl;
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
    const Engine* getEngine(const std::string& name) {
        return engineList.at(name);
    }

    std::unordered_map<std::string, SpaceShipWrapper*>* getShipList() {
        return &shipList;
    }

};


#endif //IRA_SPACESHIPHANDLER_H
