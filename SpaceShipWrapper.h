//
// Created by user on 5/27/23.
//

#ifndef IRA_SPACESHIPWRAPPER_H
#define IRA_SPACESHIPWRAPPER_H

#include <cmath>
#include <mpfr.h>
#include "SpaceShip.h"

class SpaceShipWrapper : SpaceShip {
public:
    SpaceShipWrapper(uint precision) {
        mpfr_set_default_prec(precision);
    }

    // ========== CREATORS ==========
    void addStage(uint stageIdx, const long double dryMass, const long double fuelMass, Engine* engine) {
        mpfr_t dryMass_mpfr, fuelMass_mpfr;
        mpfr_init(dryMass_mpfr);
        mpfr_init(fuelMass_mpfr);
        mpfr_set_ld(dryMass_mpfr, dryMass, MPFR_RNDN);
        mpfr_set_ld(fuelMass_mpfr, fuelMass, MPFR_RNDN);

        addStage(dryMass_mpfr, fuelMass_mpfr, engine, stageIdx);

        mpfr_clear(dryMass_mpfr);
        mpfr_clear(fuelMass_mpfr);
    }

    unsigned long createEngine(const double mass, const double exhaustVelocity, char* name) {
        auto* newEngine = new Engine();

        mpfr_set_ld(newEngine->mass, mass, MPFR_RNDN);
        mpfr_set_ld(newEngine->exhaustVelocity, exhaustVelocity, MPFR_RNDN);

        newEngine->name = new char[strlen(name) + 1];
        strcpy(newEngine->name, name);

        engineList.push_back(newEngine);

        return engineList.size() - 1;
    }

    // ========== GETTERS ==========

    /**
    * @brief Returns ld from remaining mass of type mpfr_t.
    * @param stageIdx index of the stage.
    * @param shipIdx index of the ship.
    * @return remaining mass of stages above + specified stage.
    */
    long double getRemainingMass(uint shipIdx, int stageIdx) {
        mpfr_t remainingMass;
        mpfr_init2(remainingMass, precision);
        mpfr_set_zero(remainingMass, 0);
        auto ship = shipList.at(shipIdx);
        for (int i = (*ship->getStages()).size() - 1; i >= stageIdx; i--) {
            mpfr_add(remainingMass, remainingMass, ship->stages[i]->dryMass, MPFR_RNDN);
            mpfr_add(remainingMass, remainingMass, ship->stages[i]->fuelMass, MPFR_RNDN);
            mpfr_add(remainingMass, remainingMass, ship->stages[i]->engine.mass, MPFR_RNDN);
        }
        auto result = mpfr_get_ld(remainingMass, MPFR_RNDN);
        mpfr_clear(remainingMass);
        return result;
    }

    /**
    * @brief Returns ld from dryMass of type mpfr_t.
    * @note If you stage pointer, use stageptr->getDryMass instead.
    * @param stageIdx index of the stage.
    * @param shipIdx index of the ship.
    * @return Dry mass of the stage.
    */
    long double getStageDryMass(uint shipIdx, uint stageIdx) {
        return mpfr_get_ld(shipList[shipIdx]->stages[stageIdx]->dryMass, MPFR_RNDN);
    }

    /**
    * @brief Returns ld from fuel mass of type mpfr_t.
    * @note If you stage pointer, use stageptr->getFuelMass instead.
    * @param stageIdx index of the stage.
    * @param shipIdx index of the ship.
    * @return Fuel mass of the stage.
    */
    long double getStageFuelMass(uint shipIdx, uint stageIdx) {
        return mpfr_get_ld(shipList[shipIdx]->stages[stageIdx]->fuelMass, MPFR_RNDN);
    }

    /**
    * @brief Returns ld from delta-v of type mpfr_t.
    * @note If you stage pointer, use stageptr->getDeltaV instead.
    * @param stageIdx index of the stage.
    * @param shipIdx index of the ship.
    * @return Delta-v of the stage.
    */
    long double getStageDeltaV(uint shipIdx, uint stageIdx) {
        return mpfr_get_ld(shipList[shipIdx]->stages[stageIdx]->deltaV, MPFR_RNDN);
    }

    /**
    * @brief Returns ld from total mass of type mpfr_t.
    * @note If you stage pointer, use stageptr->getTotalMass instead.
    * @param stageIdx index of the stage.
    * @param shipIdx index of the ship.
    * @return Total mass of the stage.
    */
    long double getStageTotalMass(uint shipIdx, uint stageIdx) {
        return mpfr_get_ld(shipList[shipIdx]->stages[stageIdx]->totalMass, MPFR_RNDN);
    }

    /**
    * @brief Returns ld from engine mass of type mpfr_t.
    * @note If you stage pointer, use stageptr->getEngineMass instead.
    * @param stageIdx index of the stage.
    * @param shipIdx index of the ship.
    * @return Engine mass of the stage.
    */
    long double getStageEngineMass(uint shipIdx, uint stageIdx) {
        return mpfr_get_ld(shipList[shipIdx]->stages[stageIdx]->engine.mass, MPFR_RNDN);
    }

    /**
    * @brief Returns ld from exhaust velocity of type mpfr_t.
    * @note If you stage pointer, use stageptr->getExhaustVelocity instead.
    * @param stageIdx index of the stage.
    * @param shipIdx index of the ship.
    * @return Exhaust velocity of the stage.
    */
    long double getStageExhaustVelocity(uint shipIdx, uint stageIdx) {
        return mpfr_get_ld(shipList[shipIdx]->stages[stageIdx]->engine.exhaustVelocity, MPFR_RNDN);
    }

    /**
     * @brief Returns the total mass of the spaceship.
     * @param stageIdx index of the stage.
     * @return Total mass of the spaceship.
     */
    long double getMass(uint shipIdx) {
        return mpfr_get_ld(shipList[shipIdx]->mass, MPFR_RNDN);
    }

    /**
     * @brief Returns the total delta-V of the spaceship.
     * @param stageIdx index of the stage.
     * @return Total delta-V of the spaceship.
     */
    long double getDeltaV(uint shipIdx) {
        return mpfr_get_ld(shipList[shipIdx]->deltaV, MPFR_RNDN);
    }


    // ========== SETTERS ==========
    //   This is unoptimized from version before SpaceShipHandler merge, but abstraction
    //   and modularity is more important when working in this environment.

    /**
    * @brief Sets the dry mass of a stage.
    * @param stage Pointer to the stage.
    * @param newMass The new dry mass.
    */
    void setStageDryMass(uint shipIdx, uint stageIdx, const double newMass) {
        mpfr_t newMass_mpfr;
        mpfr_init(newMass_mpfr);
        mpfr_set_d(newMass_mpfr, newMass, MPFR_RNDN);
        shipList[shipIdx]->setStageDryMass(shipList[shipIdx]->stages[stageIdx], newMass_mpfr);
        mpfr_clear(newMass_mpfr);
    }

    /**
     * @brief Sets the fuel mass of a stage.
     * @param stage Pointer to the stage.
     * @param newMass The new fuel mass.
     */
    void setStageFuelMass(uint shipIdx, uint stageIdx, const double newMass) {
        mpfr_t newMassMPFR;
        mpfr_init(newMassMPFR);
        mpfr_set_d(newMassMPFR, newMass, MPFR_RNDN);
        shipList[shipIdx]->setStageFuelMass(shipList[shipIdx]->stages[stageIdx], newMassMPFR);
        mpfr_clear(newMassMPFR);
    }

    void printStats(uint shipIdx) {
        printf("DeltaV: %Lfm/s\n", getDeltaV(shipIdx));
        printf("Mass: %Lfkg\n", getMass(shipIdx));
        for (uint i = 0; i < shipList[shipIdx]->stages.size(); i++) {
            printf("Stage %u:\n", i);
            printf("\tRemaining Mass: %Lfkg\n", getRemainingMass(shipIdx, i));
            printf("\tTotal Stage Mass: %Lfkg\n", getStageTotalMass(shipIdx, i));
            printf("\tDeltaV: %Lfm/s\n", getStageDeltaV(shipIdx, i));
            printf("\tDryMass: %Lfkg\n", getStageDryMass(shipIdx, i));
            printf("\tFuelMass: %Lfkg\n", getStageFuelMass(shipIdx, i));
            printf("\tEngine %s:\n", shipList[shipIdx]->stages[i]->engine.name);
            printf("\t\tMass: %Lfkg\n", getStageEngineMass(shipIdx, i));
            printf("\t\tExhaust Velocity: %Lfm/s\n", getStageExhaustVelocity(shipIdx, i));
        }
    }

    void setEngine(uint shipIdx, uint stageIdx, Engine* newEngine) {
        auto ship = shipList.at(shipIdx);
        ship->setStageEngine((*ship->getStages())[stageIdx], *newEngine);
    }
};


#endif //IRA_SPACESHIPWRAPPER_H
