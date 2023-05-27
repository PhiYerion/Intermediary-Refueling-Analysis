//
// Created by user on 5/24/23.
//

#ifndef IRA_SPACESHIPHANDLER_H
#define IRA_SPACESHIPHANDLER_H

#include "SpaceShip.h"
#include "Stage.h"
#include "Engine.h"
#include <mpfr.h>
#include <vector>
#include <cmath>
#include <cstring>
#include <unordered_map>
#include "SpaceShipWrapper.h"

/**
 * @brief A largely stable class that abstracts SpaceShip class
 * @details Intended as a long term interface for the space ship class.
 *          This class won't change unless a new major version comes out (excluding 0.X.X).
 */
class SpaceShipHandler {
protected:
    // Hash map will not be implemented for SpaceShipWrapper. This vector is for keeping internal tabs on the ships
    // created (mainly for memory management but also other internal functions).
    std::vector<SpaceShipWrapper*> shipList;                         /**< Pointer list to the ships. */

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
        for (auto &ship : shipList) {
            delete ship;
        }
        for (auto &enginePair : engineList) {
            delete enginePair.second;
        }
    }

    // ========== CREATORS ==========
    SpaceShipWrapper* addShip() {
        auto newShip = new SpaceShipWrapper();
        shipList.push_back(newShip);
        return newShip;
    }

    void createEngine(std::string name, const long double mass, const long double exhaustVelocity) {
        auto* newEngine = new Engine();

        mpfr_set_ld(newEngine->mass, mass, MPFR_RNDN);
        mpfr_set_ld(newEngine->exhaustVelocity, exhaustVelocity, MPFR_RNDN);

        // Accessing the element should be faster than having to copy over a string. More preferable would be to
        // get the pointer and then use that for both, and it is unknown if compiler will do this automatically.
        engineList.insert({name, newEngine});
        engineList.at(name)->name = std::move(name);
    }

    // ========== SETTERS ==========
    /**
     * @brief Sets the mass of the ship.
     * @param mass To-be mass.
     * @param ship Pointer to the ship.
     * @return 0 if successful, -1 if not.
     */
    int setEngineDryMass(const long double mass, const std::string& name) {
        try {
            mpfr_set_ld(engineList.at(name)->mass, mass, MPFR_RNDN);
        } catch (const std::out_of_range& e) {
            std::cerr << "[SpaceShipHandler::setEngineDryMass] Engine " << name << " does not exist." << std::endl;
            return -1;
        }
        return 0;
    }

    /**
     * @brief Sets the engine exhaust velocity.
     * @param exhaustVelocity To-be exhaust velocity.
     * @param name Name of the engine.
     * @return 0 if successful, -1 if not.
     */
    int setEngineExhaustVelocity(const long double exhaustVelocity, const std::string& name) {
        try {
            mpfr_set_ld(engineList.at(name)->exhaustVelocity, exhaustVelocity, MPFR_RNDN);
        } catch (const std::out_of_range& e) {
            std::cerr << "[SpaceShipHandler::setEngineExhaustVelocity] Engine " << name << " does not exist." << std::endl;
            return -1;
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

    std::vector<SpaceShipWrapper*>* getShipList() {
        return &shipList;
    }

};


#endif //IRA_SPACESHIPHANDLER_H
