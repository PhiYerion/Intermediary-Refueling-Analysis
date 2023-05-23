#ifndef SRC_SPACESHIP_H
#define SRC_SPACESHIP_H

#include <vector>
#include <iostream>
#include <valarray>
#include <cmath>
#include <iostream>
#include <cstdio>
#include "lib/mpfr.h"

/**
 * @brief Represents an engine.
 */
struct Engine {
    long double mass;               /**< Mass of the engine. */
    long double exhaustVelocity;    /**< Exhaust velocity of the engine. */
    const char* name;          /**< Name of the engine. */
};

/**
 * @brief Represents a stage of a spaceship.
 *
 * Any loss in non-fuel mass mid-stage will not be accounted for.
 */
struct Stage {
    Engine engine;             /**< Engine used in the stage. */
    long double deltaV,             /**< Delta-V of the stage. */
    dryMass,            /**< Dry mass of the stage (excluding engine mass). */
    fuelMass,           /**< Fuel mass of the stage. */
    totalMass = 0;      /**< Total mass of the stage (including engine mass). */
};

/**
 * @brief Spaceship class with full functionality.
 *
 * This class provides getters and setters for accessing and modifying the spaceship's attributes.
 * Directly setting variables will produce inaccurate measurements, so they are protected.
 */
class SpaceShip {
public:
    SpaceShip() :
            mass(0), deltaV(0), stages({}) {}
            
    ~SpaceShip() {
        mpfr_free_cache();
    }

    /**
      * @brief Returns the vector of stages.
      * @return Vector of stages.
      */
    std::vector<Stage>* getStages () {
        return &stages;
    }

    /**
     * @brief Returns the mass of all stages above the specified stage + the specified stage's mass.
     * @param inputStage The stage.
     * @return Mass of rocket during this stage prior to any burn or mass loss.
     */
    long double getRemainingMass (const Stage* inputStage) {
        long double remainingMass = mass;
        for (auto & stage : stages) {
            if (&stage == inputStage) {
                return remainingMass;
            }
            remainingMass -= stage.totalMass;
        }
        return remainingMass;
    }
    long double getRemainingMass (const int inputStageIndex) {
        long double remainingMass = mass;
        uint i = 0;
        for (const auto& stage : stages) {
            if (i == inputStageIndex) {
                return remainingMass;
            }
            remainingMass -= stage.totalMass;
            i++;
        }
        std::cerr << "Error: Stage index out of range." << std::endl;
    }

    /**
    * @brief Returns the engine of a stage.
    * @param stage Pointer to the stage.
    * @return Engine of the stage.
    */
    Engine getEngine (Stage* stage) {
        return stage -> engine;
    }

    /**
    * @brief Returns the dry mass of a stage.
    * @param stage Pointer to the stage.
    * @return Dry mass of the stage.
    */
    long double getStageDryMass(Stage* stage) {
        return stage->dryMass;
    }

    /**
     * @brief Returns the fuel mass of a stage.
     * @param stage Pointer to the stage.
     * @return Fuel mass of the stage.
     */
    long double getStageFuelMass(Stage* stage) {
        return stage->fuelMass;
    }

    /**
     * @brief Returns the delta-V of a stage.
     * @param stage Pointer to the stage.
     * @return Delta-V of the stage.
     */
    long double getStageDeltaV(Stage* stage) {
        return stage->deltaV;
    }

    /**
     * @brief Returns the total mass of a stage.
     * @param stage Pointer to the stage.
     * @return Total mass of the stage.
     */
    long double getStrageTotalMass(Stage* stage) {
        return stage->totalMass;
    }

    /**
     * @brief Returns the total mass of the spaceship.
     * @return Total mass of the spaceship.
     */
    long double getMass() {
        return mass;
    }

    /**
     * @brief Returns the total delta-V of the spaceship.
     * @return Total delta-V of the spaceship.
     */
    long double getDeltaV() {
        return deltaV;
    }

    /**
     * @brief Sets the dry mass of a stage.
     * @param stage Pointer to the stage.
     * @param newMass The new dry mass.
     */
    void setStageDryMass(Stage* stage, const long double newMass) {
        mass += newMass - stage->dryMass;
        stage->dryMass = newMass;
        stage->totalMass = stage->dryMass + stage->fuelMass + stage->engine.mass;
        genDeltaV();
    }

    /**
     * @brief Sets the fuel mass of a stage.
     * @param stage Pointer to the stage.
     * @param newMass The new fuel mass.
     */
    void setStageFuelMass(Stage* stage, const long double newMass) {
        mass += newMass - stage->fuelMass;
        stage->fuelMass = newMass;
        stage->totalMass = stage->dryMass + stage->fuelMass + stage->engine.mass;
        genDeltaV();
    }

    /**
     * @brief Sets the engine of a stage.
     * @param stage Pointer to the stage.
     * @param newEngine The new engine.
     */
    void setStageEngine(Stage* stage, const Engine newEngine) {
        mass += newEngine.mass - stage->engine.mass;
        stage->engine = newEngine;
        stage->totalMass = stage->dryMass + stage->fuelMass + stage->engine.mass;
        genDeltaV();
    }

    /**
     * @brief Adds a stage to the spaceship.
     * @param dryMass The dry mass of the stage.
     * @param fuelMass The fuel mass of the stage.
     * @param engine The engine used in the stage.
     * @param index The index at which to insert the stage (optional).
     */
    void addStage(const long double dryMass, const long double fuelMass, const Engine engine, const int index = -1) {
        if (index != -1) {
            stages.insert(stages.begin() + index, {engine, 0, dryMass, fuelMass, dryMass + fuelMass + engine.mass});
        } else {
            stages.push_back({engine, 0, dryMass, fuelMass, dryMass + fuelMass + engine.mass});
            //std::cerr << "Warning: index not specified for addStage, appending to end of stages\n";
        }
        mass += dryMass + fuelMass + engine.mass;
        genDeltaV();
    }

    void printStats() {
        printf("DeltaV: %Lfm/s\n", this->getDeltaV());
        printf("Mass: %Lfkg\n", this->getMass());
        uint i = 0;
        for (auto stagesiter = this->stages.begin(); stagesiter != this->stages.end(); stagesiter++) {
            printf("Stage %u:\n", i);
            printf("\tTotal Stage Mass: %Lfkg\n", stagesiter->totalMass);
            printf("\tTotal Mass for remaining stages: %Lfkg\n", this->getRemainingMass(&(*stagesiter)));
            printf("\tDeltaV: %Lfm/s\n", stagesiter->deltaV);
            printf("\tDryMass: %Lfkg\n", stagesiter->dryMass);
            printf("\tFuelMass: %Lfkg\n", stagesiter->fuelMass);
            printf("\tEngine %s:\n", stagesiter->engine.name);
            printf("\t\tMass: %Lfkg\n", stagesiter->engine.mass);
            printf("\t\tExhaustVelocity: %Lfm/s\n", stagesiter->engine.exhaustVelocity);
        }
    }

protected:
    std::vector<Stage> stages; /**< Vector of stages. */
    long double mass;               /**< Total mass of the spaceship. */
    long double deltaV;             /**< Total delta-V of the spaceship. */

    /**
     * @brief Generates the delta-V for the spaceship or a specific stage.
     * @param inpStage Pointer to the input stage (optional).
     *
     * If inpStage is specified, it calculates the delta-V only for stages before the new stage.
     * Otherwise, it calculates the delta-V for all stages.
     */
    void genDeltaV () {       // for addStage, this should implement only calcs on stages before new
        mpfr_t result;
        mpfr_init2(result, 256);

        mpfr_t denominator;
        mpfr_init2(denominator, 256);

        long double remainingMass = mass;
        deltaV = 0;
        for (auto &stage: stages) {
            mpfr_set_ld(result, remainingMass, MPFR_RNDN);
            mpfr_set_ld(denominator, remainingMass - stage.fuelMass, MPFR_RNDN);

            mpfr_div(result, result, denominator, MPFR_RNDN);
            mpfr_log(result, result, MPFR_RNDN);
            mpfr_mul_d(result, result, stage.engine.exhaustVelocity, MPFR_RNDN);

            stage.deltaV = mpfr_get_ld(result, MPFR_RNDN);
            deltaV += stage.deltaV;
            remainingMass -= stage.totalMass;
        }
        mpfr_clear(result);
        mpfr_clear(denominator);
    }

};


#endif //SRC_SPACESHIP_H
