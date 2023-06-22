//
// Created by user on 5/27/23.
//

#include <cmath>
#include <cstdio>
#include <mpfr.h>
#include "SpaceShip.h"

#ifndef IRA_SPACESHIPWRAPPER_H
#define IRA_SPACESHIPWRAPPER_H

class SpaceShipWrapper : SpaceShip {
public:

    // ========== CREATORS ==========
    void addStage(long double dryMass, long double fuelMass, const Engine* engine, int stageIdx = -1) {
        mpfr_t dryMass_mpfr, fuelMass_mpfr;
        mpfr_init(dryMass_mpfr);
        mpfr_init(fuelMass_mpfr);
        mpfr_set_ld(dryMass_mpfr, dryMass, MPFR_RNDN);
        mpfr_set_ld(fuelMass_mpfr, fuelMass, MPFR_RNDN);

        this->SpaceShip::addStage(dryMass_mpfr, fuelMass_mpfr, engine, stageIdx);

        mpfr_clear(dryMass_mpfr);
        mpfr_clear(fuelMass_mpfr);
    }

    void setStage(long double dryMass, long double fuelMass, const Engine* engine, int stageIdx) {
        mpfr_t dryMass_mpfr, fuelMass_mpfr;
        mpfr_init(dryMass_mpfr);
        mpfr_init(fuelMass_mpfr);
        mpfr_set_ld(dryMass_mpfr, dryMass, MPFR_RNDN);
        mpfr_set_ld(fuelMass_mpfr, fuelMass, MPFR_RNDN);

        this->SpaceShip::setStage(dryMass_mpfr, fuelMass_mpfr, engine, stageIdx);

        mpfr_clear(dryMass_mpfr);
        mpfr_clear(fuelMass_mpfr);
    }

    // ========== GETTERS ==========

    /**
    * @brief Returns ld from remaining mass of type mpfr_t.
    * @param stageIdx index of the stage.
    * @return remaining mass of stages above + specified stage.
    */
    long double getRemainingMass(uint stageIdx) {
        mpfr_t remainingMass;
        mpfr_init(remainingMass);
        mpfr_set_zero(remainingMass, 0);
        for (long i = getStages()->size() - 1; i >= stageIdx; i--) {                        // don't change to unsigned long
            mpfr_add(remainingMass, remainingMass, stages[i]->dryMass, MPFR_RNDN);          // as it will cause infinite loop
            mpfr_add(remainingMass, remainingMass, stages[i]->fuelMass, MPFR_RNDN);         // at stage 0
            mpfr_add(remainingMass, remainingMass, stages[i]->engine->mass, MPFR_RNDN);
        }
        auto result = mpfr_get_ld(remainingMass, MPFR_RNDN);
        mpfr_clear(remainingMass);
        return result;
    }

    SpaceShip* getRawShip() {
        return this;
    }

    /**
    * @brief Returns ld from dryMass of type mpfr_t.
    * @note If you stage pointer, use stageptr->getDryMass instead.
    * @param stageIdx index of the stage.
    * @return Dry mass of the stage.
    */
    long double getStageDryMass(uint stageIdx) {
        return mpfr_get_ld(stages[stageIdx]->dryMass, MPFR_RNDN);
    }

    /**
    * @brief Returns ld from fuel mass of type mpfr_t.
    * @note If you stage pointer, use stageptr->getFuelMass instead.
    * @param stageIdx index of the stage.
    * @return Fuel mass of the stage.
    */
    long double getStageFuelMass(uint stageIdx) {
        return mpfr_get_ld(stages[stageIdx]->fuelMass, MPFR_RNDN);
    }

    /**
    * @brief Returns ld from delta-v of type mpfr_t.
    * @note If you stage pointer, use stageptr->getDeltaV instead.
    * @param stageIdx index of the stage.
    * @return Delta-v of the stage.
    */
    long double getStageDeltaV(uint stageIdx) {
        return mpfr_get_ld(stages[stageIdx]->deltaV, MPFR_RNDN);
    }

    /**
    * @brief Returns ld from total mass of type mpfr_t.
    * @note If you stage pointer, use stageptr->getTotalMass instead.
    * @param stageIdx index of the stage.
    * @return Total mass of the stage.
    */
    long double getStageTotalMass(uint stageIdx) {
        return mpfr_get_ld(stages[stageIdx]->totalMass, MPFR_RNDN);
    }

    const Engine* getStageEngine(uint stageIdx) {
        return stages[stageIdx]->engine;
    }

    /**
    * @brief Returns ld from engine mass of type mpfr_t.
    * @note If you stage pointer, use stageptr->getEngineMass instead.
    * @param stageIdx index of the stage.
    * @return Engine mass of the stage.
    */
    long double getStageEngineMass(uint stageIdx) {
        return mpfr_get_ld(stages[stageIdx]->engine->mass, MPFR_RNDN);
    }

    /**
    * @brief Returns ld from exhaust velocity of type mpfr_t.
    * @note If you stage pointer, use stageptr->getExhaustVelocity instead.
    * @param stageIdx index of the stage.
    * @return Exhaust velocity of the stage.
    */
    long double getStageExhaustVelocity(uint stageIdx) {
        return mpfr_get_ld(stages[stageIdx]->engine->exhaustVelocity, MPFR_RNDN);
    }

    /**
     * @brief Returns the total mass of the spaceship.
     * @return Total mass of the spaceship.
     */
    long double getMass() {
        return mpfr_get_ld(mass, MPFR_RNDN);
    }

    /**
     * @brief Returns the total delta-V of the spaceship.
     * @return Total delta-V of the spaceship.
     */
    long double getDeltaV() {
        return mpfr_get_ld(deltaV, MPFR_RNDN);
    }

    const std::vector<Stage*>* getStages() {
        return &stages;
    }


    // ========== SETTERS ==========
    //   This is unoptimized from version before SpaceShipHandler merge, but abstraction
    //   and modularity is more important when working in this environment.

    /**
    * @brief Sets the dry mass of a stage.
    * @param stage Pointer to the stage.
    * @param newMass The new dry mass.
    */
    void setStageDryMass(uint stageIdx, const long double newMass) {
        mpfr_t newMass_mpfr;
        mpfr_init(newMass_mpfr);
        mpfr_set_ld(newMass_mpfr, newMass, MPFR_RNDN);
        SpaceShip::setStageDryMass(stages[stageIdx], newMass_mpfr);
        mpfr_clear(newMass_mpfr);
    }

    /**
     * @brief Sets the fuel mass of a stage.
     * @param stage Pointer to the stage.
     * @param newMass The new fuel mass.
     */
    void setStageFuelMass(uint stageIdx, const long double newMass) {
        mpfr_t newMassMPFR;
        mpfr_init(newMassMPFR);
        mpfr_set_d(newMassMPFR, newMass, MPFR_RNDN);
        SpaceShip::setStageFuelMass(stages[stageIdx], newMassMPFR);
        mpfr_clear(newMassMPFR);
    }

    void setStageEngine(uint stageIdx, const Engine* newEngine) {
        SpaceShip::setStageEngine(stages[stageIdx], newEngine);
    }

    // ===== MPFR GETTERS =====
    void getRawDeltaV(mpfr_t result) {
        mpfr_init(result);
        mpfr_set(result, deltaV, MPFR_RNDN);
    }

    void getRawMass(mpfr_t result) {
        mpfr_init(result);
        mpfr_set(result, mass, MPFR_RNDN);
    }

    // ========== MISC ==========
    void printStats() {
        printf("DeltaV: %.32Lf m/s\n", getDeltaV());
        printf("Mass: %.32Lf kg\n", getMass());
        for (uint i = 0; i < stages.size(); i++) {
            printf("Stage %u:\n", i);
            printf("\tRemaining Mass: %.32Lf kg\n", getRemainingMass(i));
            printf("\tTotal Stage Mass: %.32Lf kg\n", getStageTotalMass(i));
            printf("\tDeltaV: %.32Lf m/s\n", getStageDeltaV(i));
            printf("\tDryMass: %.32Lf kg\n", getStageDryMass(i));
            printf("\tFuelMass: %.32Lf kg\n", getStageFuelMass(i));
            printf("\tEngine %s:\n", stages[i]->engine->name.c_str());
            printf("\t\tMass: %.32Lf kg\n", getStageEngineMass(i));
            printf("\t\tExhaust Velocity: %.32Lf m/s\n", getStageExhaustVelocity(i));
        }
    }

};


#endif //IRA_SPACESHIPWRAPPER_H
