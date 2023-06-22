//
// Created by user on 5/27/23.
//

#include <cmath>
#include <cstdio>
#include <mpfr.h>
#include "Ship.h"

#ifndef IRA_SPACESHIPWRAPPER_H
#define IRA_SPACESHIPWRAPPER_H

class ShipWrapper : Ship {
public:

    // ========== CREATORS ==========
    void addStage(const std::string& dryMass, const std::string& fuelMass, const Engine* engine, int stageIdx = -1);

    void setStage(const std::string& dryMass, const std::string& fuelMass, const Engine* engine, int stageIdx);

    // ========== GETTERS ==========

    /**
    * @brief Returns ld from remaining mass of type mpfr_t.
    * @param stageIdx index of the stage.
    * @return remaining mass of stages above + specified stage.
    */
    long double getRemainingMass(uint stageIdx);

    Ship* getRawShip();

    /**
    * @brief Returns ld from dryMass of type mpfr_t.
    * @note If you stage pointer, use stageptr->getDryMass instead.
    * @param stageIdx index of the stage.
    * @return Dry mass of the stage.
    */
    long double getStageDryMass(uint stageIdx);

    /**
    * @brief Returns ld from fuel mass of type mpfr_t.
    * @note If you stage pointer, use stageptr->getFuelMass instead.
    * @param stageIdx index of the stage.
    * @return Fuel mass of the stage.
    */
    long double getStageFuelMass(uint stageIdx);

    /**
    * @brief Returns ld from delta-v of type mpfr_t.
    * @note If you stage pointer, use stageptr->getDeltaV instead.
    * @param stageIdx index of the stage.
    * @return Delta-v of the stage.
    */
    long double getStageDeltaV(uint stageIdx);

    /**
    * @brief Returns ld from total mass of type mpfr_t.
    * @note If you stage pointer, use stageptr->getTotalMass instead.
    * @param stageIdx index of the stage.
    * @return Total mass of the stage.
    */
    long double getStageTotalMass(uint stageIdx);

    const Engine* getStageEngine(uint stageIdx);

    /**
    * @brief Returns ld from engine mass of type mpfr_t.
    * @note If you stage pointer, use stageptr->getEngineMass instead.
    * @param stageIdx index of the stage.
    * @return Engine mass of the stage.
    */
    long double getStageEngineMass(uint stageIdx);

    /**
    * @brief Returns ld from exhaust velocity of type mpfr_t.
    * @note If you stage pointer, use stageptr->getExhaustVelocity instead.
    * @param stageIdx index of the stage.
    * @return Exhaust velocity of the stage.
    */
    long double getStageExhaustVelocity(uint stageIdx);

    /**
     * @brief Returns the total mass of the spaceship.
     * @return Total mass of the spaceship.
     */
    long double getMass();

    /**
     * @brief Returns the total delta-V of the spaceship.
     * @return Total delta-V of the spaceship.
     */
    long double getDeltaV();

    const std::vector<Stage*>* getStages();


    // ========== SETTERS ==========
    //   This is unoptimized from version before Handler merge, but abstraction
    //   and modularity is more important when working in this environment.

    /**
    * @brief Sets the dry mass of a stage.
    * @param stage Pointer to the stage.
    * @param newMass The new dry mass.
    */
    void setStageDryMass(uint stageIdx, const std::string& newMass);

    /**
     * @brief Sets the fuel mass of a stage.
     * @param stage Pointer to the stage.
     * @param newMass The new fuel mass.
     */
    void setStageFuelMass(uint stageIdx, const std::string& newMass);

    void setStageEngine(uint stageIdx, const Engine* newEngine);

    // ===== MPFR GETTERS =====
    void getRawDeltaV(mpfr_t result);

    void getRawMass(mpfr_t result);

    // ========== MISC ==========
    void printStats();

};


#endif //IRA_SPACESHIPWRAPPER_H
