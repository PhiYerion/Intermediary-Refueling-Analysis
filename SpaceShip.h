#ifndef SRC_SPACESHIP_H
#define SRC_SPACESHIP_H

#include <vector>
#include <iostream>
#include <valarray>
#include <cmath>
#include <iostream>
#include <cstdio>
#include <mpfr.h>
#include "cstring"
#define PRECISION 1024
/**
 * @brief Represents an engine.
 */
struct Engine {

    mpfr_t mass,                /**< Mass of the engine. */
    exhaustVelocity;            /**< Exhaust velocity of the engine. */
    char* name;                 /**< Name of the engine. */

    Engine() {
        mpfr_set_default_prec(PRECISION);
        mpfr_init(mass);
        mpfr_init(exhaustVelocity);
    }
    ~Engine() {

        if (mass[0]._mpfr_d != nullptr) {                                    // Errors pretaining to erroneous clearing
            free(name);                                                 // should be resolved, so the lack of hard
            mpfr_clear(mass);                                                // errors when nullptr is encountered is
            mpfr_clear(exhaustVelocity);                                     // acceptable and needed due to nullptrs
        }                                                                    // in the case that there is a move operation
    }

    // Copy operations:
    Engine& operator=(const Engine& other) {
        if (this == &other) {
            return *this; // Handle self-assignment
        }
        mpfr_set(mass, other.mass, MPFR_RNDN);
        mpfr_set(exhaustVelocity, other.exhaustVelocity, MPFR_RNDN);

        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);

        return *this;
    }
    Engine(const Engine& other) {
        mpfr_init(mass);
        mpfr_init(exhaustVelocity);
        mpfr_set(mass, other.mass, MPFR_RNDN);
        mpfr_set(exhaustVelocity, other.exhaustVelocity, MPFR_RNDN);

        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
    }

    // Move operations:
    Engine& operator=(Engine&& other)  noexcept {
        if (this == &other) {
            return *this; // Handle self-assignment
        }

        mass[0] = other.mass[0];
        other.mass[0]._mpfr_d = nullptr;

        exhaustVelocity[0] = other.exhaustVelocity[0];
        other.exhaustVelocity[0]._mpfr_d = nullptr;

        name = other.name;
        other.name = nullptr;

        return *this;
    }
    Engine(Engine&& other) noexcept {
        mass[0] = other.mass[0];
        other.mass[0]._mpfr_d = nullptr;

        exhaustVelocity[0] = other.exhaustVelocity[0];
        other.exhaustVelocity[0]._mpfr_d = nullptr;

        name = other.name;
        other.name = nullptr;
    }
};

/**
 * @brief Represents a stage of a spaceship.
 *
 * Any loss in non-fuel mass mid-stage will not be accounted for.
 */
struct Stage {
    Engine engine;                /**< Engine used in the stage. */
    mpfr_t deltaV,                /**< Delta-V of the stage. */
    dryMass,                      /**< Dry mass of the stage (excluding engine mass). */
    fuelMass,                     /**< Fuel mass of the stage. */
    totalMass;                    /**< Total mass of the stage (including engine mass). */

    Stage() {
        mpfr_set_default_prec(PRECISION);
        mpfr_init(deltaV);
        mpfr_init(dryMass);
        mpfr_init(fuelMass);
        mpfr_init(totalMass);
    }
    ~Stage() {
        mpfr_clear(deltaV);
        mpfr_clear(dryMass);
        mpfr_clear(fuelMass);
        mpfr_clear(totalMass);
    }

    // Copy operations:
    Stage& operator=(const Stage& other) {
        if (this == &other) {                                               // Handle self-assignment
            return *this;
        }
        mpfr_set(deltaV, other.deltaV, MPFR_RNDN);                          // Make sure that the underlying mpfr_t values
        mpfr_set(dryMass, other.dryMass, MPFR_RNDN);                        // are copied instead of just the pointers
        mpfr_set(fuelMass, other.fuelMass, MPFR_RNDN);
        mpfr_set(totalMass, other.totalMass, MPFR_RNDN);
        engine = other.engine;                                              // This is a copy operation

        return *this;
    }
    Stage(const Stage& other) {
        mpfr_init(deltaV);                                                  // Make sure that the underlying mpfr_t values
        mpfr_init(dryMass);                                                 // are copied instead of just the pointers.
        mpfr_init(fuelMass);                                                // This is effectively the same as the
        mpfr_init(totalMass);                                               // move operator above, but also initializes
        mpfr_set(deltaV, other.deltaV, MPFR_RNDN);                          // the mpfr_t values.
        mpfr_set(dryMass, other.dryMass, MPFR_RNDN);
        mpfr_set(fuelMass, other.fuelMass, MPFR_RNDN);
        mpfr_set(totalMass, other.totalMass, MPFR_RNDN);

        engine = other.engine;                                              // This is a copy operation
    }

    // Move operations:
    Stage& operator=(Stage&& other)  noexcept {
        if (this == &other) {
            return *this;                                                   // Handle self-assignment
        }

        deltaV[0] = other.deltaV[0];                                        // Point the new mpfr_t pointers to the data
        dryMass[0] = other.dryMass[0];                                      // of the original object.
        fuelMass[0] = other.fuelMass[0];
        totalMass[0] = other.totalMass[0];
        engine = std::move(other.engine);

        return *this;
    }
    Stage(Stage&& other) noexcept {
        deltaV[0] = other.deltaV[0];
        dryMass[0] = other.dryMass[0];
        fuelMass[0] = other.fuelMass[0];
        totalMass[0] = other.totalMass[0];
        engine = std::move(other.engine);
    }
};

/**
 * @brief Spaceship class with full functionality.
 *
 * This class provides getters and setters for accessing and modifying the spaceship's attributes.
 * Directly setting variables will produce inaccurate measurements, so they are protected.
 */
class SpaceShip {
protected:
    std::vector<Stage*> stages;  /**< Vector of stages. */
    mpfr_t mass,                /**< Total mass of the spaceship. */
    deltaV;                     /**< Total delta-V of the spaceship. */

    /**
     * @brief Generates the delta-V for the spaceship or a specific stage.
     * @param inpStage Pointer to the input stage (optional).
     *
     * If inpStage is specified, it calculates the delta-V only for stages before the new stage.
     * Otherwise, it calculates the delta-V for all stages.
     */
    void genDeltaV () {       // for addStage, this should implement only calcs on stages before new

        mpfr_t denominator;
        mpfr_init2(denominator, PRECISION);

        mpfr_t remainingMass;
        mpfr_init2(remainingMass, PRECISION);
        mpfr_set(remainingMass, mass, MPFR_RNDN);
        mpfr_set_zero(deltaV, 0);
        for (auto &stage: stages) {
            mpfr_set(stage->deltaV, remainingMass, MPFR_RNDN);
            mpfr_sub(denominator, remainingMass, stage->fuelMass, MPFR_RNDN);
            mpfr_div(stage->deltaV, stage->deltaV, denominator, MPFR_RNDN);
            mpfr_log(stage->deltaV, stage->deltaV, MPFR_RNDN);
            mpfr_mul(stage->deltaV, stage->deltaV, stage->engine.exhaustVelocity, MPFR_RNDN);

            mpfr_add(deltaV, deltaV, stage->deltaV, MPFR_RNDN);
            mpfr_sub(remainingMass, remainingMass, stage->totalMass, MPFR_RNDN);
        }
        mpfr_clear(denominator);
        mpfr_clear(remainingMass);
    }

public:
    SpaceShip() {
        mpfr_set_default_prec(PRECISION);
        mpfr_init(mass);
        mpfr_init(deltaV);
        mpfr_set_zero(mass, 0);
        mpfr_set_zero(deltaV, 0);
    }
            
    ~SpaceShip() {
        mpfr_clear(mass);
        mpfr_clear(deltaV);
        for (auto & stage : stages) {
            delete(stage);
        }
        mpfr_free_cache();
    }

    /**
      * @brief Returns the vector of stages.
      * @return Vector of stages.
      */
    std::vector<Stage*>* getStages () {
        return &stages;
    }

    /**
     * @brief Returns the mass of all stages above the specified stage + the specified stage's mass.
     * @param inputStage The stage.
     * @return Mass of rocket during this stage prior to any burn or mass loss.
     */
    void getRemainingMass (mpfr_t result, const Stage* inputStage) {
        mpfr_set(result, mass, MPFR_RNDN);
        for (auto & stage : stages) {
            if (stage == inputStage) {
                return;
            }
            mpfr_sub(result, result, stage->totalMass, MPFR_RNDN);
        }
    }
    void getRemainingMass (mpfr_t result, const int inputStageIndex) {
        mpfr_set(result, mass, MPFR_RNDN);
        uint i = 0;
        for (const auto& stage : stages) {
            if (i == inputStageIndex) {
                return;
            }
            mpfr_sub(result, result, stage->totalMass, MPFR_RNDN);
            i++;
        }
    }

    /**
    * @brief Returns the engine of a stage.
    * @param stage Pointer to the stage.
    * @return Engine of the stage.
    */
    Engine* getEngine (Stage* stage) {
        return &stage -> engine;
    }

    /**
    * @brief Returns the dry mass of a stage.
    * @param stage Pointer to the stage.
    * @return Dry mass of the stage.
    */
    long double getStageDryMass(Stage* stage) {
        return mpfr_get_ld(stage->dryMass, MPFR_RNDN);
    }

    /**
     * @brief Returns the fuel mass of a stage.
     * @param stage Pointer to the stage.
     * @return Fuel mass of the stage.
     */
    long double getStageFuelMass(Stage* stage) {
        return mpfr_get_ld(stage->fuelMass, MPFR_RNDN);
    }

    /**
     * @brief Returns the delta-V of a stage.
     * @param stage Pointer to the stage.
     * @return Delta-V of the stage.
     */
    long double getStageDeltaV(Stage* stage) {
        return mpfr_get_ld(stage->deltaV, MPFR_RNDN);
    }

    /**
     * @brief Returns the total mass of a stage.
     * @param stage Pointer to the stage.
     * @return Total mass of the stage.
     */
    long double getStrageTotalMass(Stage* stage) {
        return mpfr_get_ld(stage->totalMass, MPFR_RNDN);
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

    /**
     * @brief Sets the dry mass of a stage.
     * @param stage Pointer to the stage.
     * @param newMass The new dry mass.
     */
    void setStageDryMass(Stage* stage, const double newMass) {
        mpfr_sub(mass, mass, stage->dryMass, MPFR_RNDN);
        mpfr_sub(stage->totalMass, stage->totalMass, stage->dryMass, MPFR_RNDN);

        mpfr_add_d(mass, mass, newMass, MPFR_RNDN);
        mpfr_add_d(stage->totalMass, stage->totalMass, newMass, MPFR_RNDN);

        mpfr_set_d(stage->dryMass, newMass, MPFR_RNDN); // stage->dryMass = newMass;
        genDeltaV();
    }

    /**
     * @brief Sets the fuel mass of a stage.
     * @param stage Pointer to the stage.
     * @param newMass The new fuel mass.
     */
    void setStageFuelMass(Stage* stage, const double newMass) {
        mpfr_sub(mass, mass, stage->fuelMass, MPFR_RNDN);
        mpfr_sub(stage->totalMass, stage->totalMass, stage->fuelMass, MPFR_RNDN);

        mpfr_add_d(mass, mass, newMass, MPFR_RNDN);
        mpfr_add_d(stage->totalMass, stage->totalMass, newMass, MPFR_RNDN);

        mpfr_set_d(stage->fuelMass, newMass, MPFR_RNDN); // stage->dryMass = newMass;
        genDeltaV();
    }

    /**
     * @brief Sets the engine of a stage.
     * @param stage Pointer to the stage.
     * @param newEngine The new engine.
     */
    void setStageEngine(Stage* stage, Engine newEngine) {
        mpfr_sub(mass, mass, stage->engine.mass, MPFR_RNDN);
        mpfr_sub(stage->totalMass, stage->totalMass, stage->engine.mass, MPFR_RNDN);

        mpfr_add(mass, mass, newEngine.mass, MPFR_RNDN);
        mpfr_add(stage->totalMass, stage->totalMass, newEngine.mass, MPFR_RNDN);

        stage->engine = std::move(newEngine);
        genDeltaV();
    }

    /**
     * @brief Adds a stage to the spaceship.
     * @param dryMass The dry mass of the stage.
     * @param fuelMass The fuel mass of the stage.
     * @param engine The engine used in the stage.
     * @param index The index at which to insert the stage (optional).
     */
    void addStage(const double dryMass, const double fuelMass, const double engineMass, const double engineExhaustVelocity, char* engineName, const int index = -1) {

        Stage* stage;
        if (index != -1) {
            stages.insert(stages.begin() + index, new Stage());
            stage = stages[index];
        } else {
            stages.push_back(new Stage());
            stage = stages.back();
            //std::cerr << "Warning: index not specified for addStage, appending to end of stages\n";
        }
        stage->engine.name = engineName;

        mpfr_set_ld(stage->dryMass, dryMass, MPFR_RNDN);
        mpfr_set_ld(stage->fuelMass, fuelMass, MPFR_RNDN);
        mpfr_set_ld(stage->engine.mass, engineMass, MPFR_RNDN);
        mpfr_set_ld(stage->engine.exhaustVelocity, engineExhaustVelocity, MPFR_RNDN);

        mpfr_add(stage->totalMass, stage->dryMass, stage->fuelMass, MPFR_RNDN);
        mpfr_add(stage->totalMass, stage->totalMass, stage->fuelMass, MPFR_RNDN);

        mpfr_add(mass, mass, stage->totalMass, MPFR_RNDN);
        genDeltaV();
    }

    void addStage(const double dryMass, const double fuelMass, Engine engine, const int index = -1) {

        Stage* stage;
        if (index != -1) {
            stages.insert(stages.begin() + index, new Stage());
            stage = stages[index];
        } else {
            stages.push_back(new Stage());
            stage = stages.back();
            //std::cerr << "Warning: index not specified for addStage, appending to end of stages\n";
        }
        stage->engine = std::move(engine);

        mpfr_set_ld(stage->dryMass, dryMass, MPFR_RNDN);
        mpfr_set_ld(stage->fuelMass, fuelMass, MPFR_RNDN);

        mpfr_add(stage->totalMass, stage->dryMass, stage->fuelMass, MPFR_RNDN);
        mpfr_add(stage->totalMass, stage->totalMass, stage->engine.mass, MPFR_RNDN);

        mpfr_add(mass, mass, stage->totalMass, MPFR_RNDN);
        genDeltaV();
    }

    long double toLongDouble(mpfr_t mpfr) {
        return mpfr_get_ld(mpfr, MPFR_RNDN);
    }

    void printStats() {
        printf("DeltaV: %Lfm/s\n", this->getDeltaV());
        printf("Mass: %Lfkg\n", this->getMass());
        uint i = 0;
        for (auto stagesiter = this->stages.begin(); stagesiter != this->stages.end(); stagesiter++) {
            printf("Stage %u:\n", i);
            printf("\tTotal Stage Mass: %Lfkg\n", toLongDouble((*stagesiter)->totalMass));
            mpfr_t temp;
            mpfr_init(temp);
            this->getRemainingMass(temp, (*stagesiter));
            printf("\tTotal Mass for remaining stages: %Lfkg\n", toLongDouble(temp));
            printf("\tDeltaV: %Lfm/s\n", toLongDouble((*stagesiter)->deltaV));
            printf("\tDryMass: %Lfkg\n", toLongDouble((*stagesiter)->dryMass));
            printf("\tFuelMass: %Lfkg\n", toLongDouble((*stagesiter)->fuelMass));
            printf("\tEngine %s:\n", (*stagesiter)->engine.name);
            printf("\t\tMass: %Lfkg\n", toLongDouble((*stagesiter)->engine.mass));
            printf("\t\tExhaustVelocity: %Lfm/s\n", toLongDouble((*stagesiter)->engine.exhaustVelocity));
            i++;
        }
    }
};


#endif //SRC_SPACESHIP_H
