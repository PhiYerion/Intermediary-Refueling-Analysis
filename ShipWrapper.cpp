//
// Created by user on 5/27/23.
//

#include "ShipWrapper.h"

// ========== CREATORS ==========
void ShipWrapper::addStage(const std::string& dryMass, const std::string& fuelMass, const Engine* engine, int stageIdx) {
    mpfr_t dryMass_mpfr, fuelMass_mpfr;
    mpfr_init(dryMass_mpfr);
    mpfr_init(fuelMass_mpfr);
    mpfr_set_str(dryMass_mpfr, dryMass.c_str(), 10, MPFR_RNDN);
    mpfr_set_str(fuelMass_mpfr, fuelMass.c_str(), 10, MPFR_RNDN);

    this->Ship::addStage(dryMass_mpfr, fuelMass_mpfr, engine, stageIdx);

    mpfr_clear(dryMass_mpfr);
    mpfr_clear(fuelMass_mpfr);
}

void ShipWrapper::setStage(const std::string& dryMass, const std::string& fuelMass, const Engine* engine, int stageIdx) {
    mpfr_t dryMass_mpfr, fuelMass_mpfr;
    mpfr_init(dryMass_mpfr);
    mpfr_init(fuelMass_mpfr);
    mpfr_set_str(dryMass_mpfr, dryMass.c_str(), 10, MPFR_RNDN);
    mpfr_set_str(fuelMass_mpfr, fuelMass.c_str(), 10, MPFR_RNDN);

    this->Ship::setStage(dryMass_mpfr, fuelMass_mpfr, engine, stageIdx);

    mpfr_clear(dryMass_mpfr);
    mpfr_clear(fuelMass_mpfr);
}

// ========== GETTERS ==========

/**
* @brief Returns ld from remaining mass of type mpfr_t.
* @param stageIdx index of the stage.
* @return remaining mass of stages above + specified stage.
*/
long double ShipWrapper::getRemainingMass(uint stageIdx) {
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

Ship* ShipWrapper::getRawShip() {
    return this;
}

/**
* @brief Returns ld from dryMass of type mpfr_t.
* @note If you stage pointer, use stageptr->getDryMass instead.
* @param stageIdx index of the stage.
* @return Dry mass of the stage.
*/
long double ShipWrapper::getStageDryMass(uint stageIdx) {
    return mpfr_get_ld(stages[stageIdx]->dryMass, MPFR_RNDN);
}

/**
* @brief Returns ld from fuel mass of type mpfr_t.
* @note If you stage pointer, use stageptr->getFuelMass instead.
* @param stageIdx index of the stage.
* @return Fuel mass of the stage.
*/
long double ShipWrapper::getStageFuelMass(uint stageIdx) {
    return mpfr_get_ld(stages[stageIdx]->fuelMass, MPFR_RNDN);
}

/**
* @brief Returns ld from delta-v of type mpfr_t.
* @note If you stage pointer, use stageptr->getDeltaV instead.
* @param stageIdx index of the stage.
* @return Delta-v of the stage.
*/
long double ShipWrapper::getStageDeltaV(uint stageIdx) {
    return mpfr_get_ld(stages[stageIdx]->deltaV, MPFR_RNDN);
}

/**
* @brief Returns ld from total mass of type mpfr_t.
* @note If you stage pointer, use stageptr->getTotalMass instead.
* @param stageIdx index of the stage.
* @return Total mass of the stage.
*/
long double ShipWrapper::getStageTotalMass(uint stageIdx) {
    return mpfr_get_ld(stages[stageIdx]->totalMass, MPFR_RNDN);
}

const Engine* ShipWrapper::getStageEngine(uint stageIdx) {
    return stages[stageIdx]->engine;
}

/**
* @brief Returns ld from engine mass of type mpfr_t.
* @note If you stage pointer, use stageptr->getEngineMass instead.
* @param stageIdx index of the stage.
* @return Engine mass of the stage.
*/
long double ShipWrapper::getStageEngineMass(uint stageIdx) {
    return mpfr_get_ld(stages[stageIdx]->engine->mass, MPFR_RNDN);
}

/**
* @brief Returns ld from exhaust velocity of type mpfr_t.
* @note If you stage pointer, use stageptr->getExhaustVelocity instead.
* @param stageIdx index of the stage.
* @return Exhaust velocity of the stage.
*/
long double ShipWrapper::getStageExhaustVelocity(uint stageIdx) {
    return mpfr_get_ld(stages[stageIdx]->engine->exhaustVelocity, MPFR_RNDN);
}

/**
 * @brief Returns the total mass of the spaceship.
 * @return Total mass of the spaceship.
 */
long double ShipWrapper::getMass() {
    return mpfr_get_ld(mass, MPFR_RNDN);
}

/**
 * @brief Returns the total delta-V of the spaceship.
 * @return Total delta-V of the spaceship.
 */
long double ShipWrapper::getDeltaV() {
    return mpfr_get_ld(deltaV, MPFR_RNDN);
}

const std::vector<Stage*>* ShipWrapper::getStages() {
    return &stages;
}


// ========== SETTERS ==========
//   This is unoptimized from version before Handler merge, but abstraction
//   and modularity is more important when working in this environment.

/**
* @brief Sets the dry mass of a stage.
* @param stage Pointer to the stage.
* @param newMass The new dry mass.
*/
void ShipWrapper::setStageDryMass(uint stageIdx, const std::string& newMass) {
    mpfr_t newMass_mpfr;
    mpfr_init(newMass_mpfr);
    mpfr_set_str(newMass_mpfr, newMass.c_str(), 10, MPFR_RNDN);
    Ship::setStageDryMass(stages[stageIdx], newMass_mpfr);
    mpfr_clear(newMass_mpfr);
}

/**
 * @brief Sets the fuel mass of a stage.
 * @param stage Pointer to the stage.
 * @param newMass The new fuel mass.
 */
void ShipWrapper::setStageFuelMass(uint stageIdx, const std::string& newMass) {
    mpfr_t newMassMPFR;
    mpfr_init(newMassMPFR);
    mpfr_set_str(newMassMPFR, newMass.c_str(), 10, MPFR_RNDN);
    Ship::setStageFuelMass(stages[stageIdx], newMassMPFR);
    mpfr_clear(newMassMPFR);
}

void ShipWrapper::setStageEngine(uint stageIdx, const Engine* newEngine) {
    Ship::setStageEngine(stages[stageIdx], newEngine);
}

// ===== MPFR GETTERS =====
void ShipWrapper::getRawDeltaV(mpfr_t result) {
    mpfr_init(result);
    mpfr_set(result, deltaV, MPFR_RNDN);
}

void ShipWrapper::getRawMass(mpfr_t result) {
    mpfr_init(result);
    mpfr_set(result, mass, MPFR_RNDN);
}

// ========== MISC ==========
void ShipWrapper::printStats() {
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
