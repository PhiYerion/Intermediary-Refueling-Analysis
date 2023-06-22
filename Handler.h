#include "Ship.h"
#include "Stage.h"
#include "Engine.h"
#include "cstdio"
#include <mpfr.h>
#include <vector>
#include <cmath>
#include <cstring>
#include <unordered_map>
#include "iostream"
#include "ShipWrapper.h"

#ifndef IRA_SPACESHIPHANDLER_H
#define IRA_SPACESHIPHANDLER_H

/**
 * @brief A largely stable class that abstracts Ship class
 * @details Intended as a long term interface for the space ship class.
 *          This class won't change unless a new major version comes out (excluding 0.X.X).
 */
class Handler {
protected:
    // Hash map will not be implemented for ShipWrapper. This vector is for keeping internal tabs on the ships
    // created (mainly for memory management but also other internal functions).
    std::unordered_map<std::string, ShipWrapper*> shipList;                         /**< Pointer list to the ships. */

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
    explicit Handler(long precision);

    ~Handler();

    // ========== CREATORS ==========
    ShipWrapper* addShip(const std::string& name);

    int createEngine(const std::string& name, const std::string& mass, const std::string& exhaustVelocity);

    // ========== SETTERS ==========
    /**
     * @brief Sets the mass of the ship.
     * @param mass To-be mass.
     * @param ship Pointer to the ship.
     * @return 0 if successful, 1 if not.
     */
    int setEngineDryMass(const std::string& mass, const std::string& name);

    /**
     * @brief Sets the engine exhaust velocity.
     * @param exhaustVelocity To-be exhaust velocity.
     * @param name Name of the engine.
     * @return 0 if successful, 1 if not.
     */
    int setEngineExhaustVelocity(const std::string& exhaustVelocity, const std::string& name);

    // ========== GETTERS ==========
    /**
     * @brief Gets the engine by name. Errors have to be handled.
     * @param name Name of the Engine.
     * @return Pointer to the Engine.
     */
    ShipWrapper * getShip(const std::string& name);

    const Engine* getEngine(const std::string& name);

    double getEngineMass(const std::string& name);

    double getEngineExhaustVelocity(const std::string& name);

    const std::unordered_map<std::string, Engine*>* getEngineList();

    std::unordered_map<std::string, ShipWrapper*>* getShipList();

};


#endif //IRA_SPACESHIPHANDLER_H
