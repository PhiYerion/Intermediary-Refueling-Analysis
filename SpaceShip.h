#ifndef SRC_SPACESHIP_H
#define SRC_SPACESHIP_H


#include <vector>
#include <iostream>
#include <valarray>

/**
 * @brief Represents an engine.
 */
struct Engine {
     unsigned long long mass;               /**< Mass of the engine. */
     unsigned long long exhaustVelocity;    /**< Exhaust velocity of the engine. */
    const char* name;          /**< Name of the engine. */
};

/**
 * @brief Represents a stage of a spaceship.
 *
 * Any loss in non-fuel mass mid-stage will not be accounted for.
 */
struct Stage {
    Engine engine;             /**< Engine used in the stage. */
     unsigned long long deltaV,             /**< Delta-V of the stage. */
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
    SpaceShip() {
        mass = 0;
        deltaV = 0;
        stages = {};
    }

    template <typename T>
    unsigned long long toPrecise (const T& input) {
        auto inputUll = (unsigned long long) input;
        if (inputUll > 4294967295) {
            throw std::invalid_argument("Input is too large to be converted to a precise value.");
        }
        return (unsigned long long) (inputUll <<32);
    }
    unsigned long long fromPrecise (const unsigned long long& input) {
        return input >> 32;
    }

    /**
      * @brief Returns the vector of stages.
      * @return Vector of stages.
      */
    std::vector<Stage> getStages () {
        return stages;
    }

    /**
     * @brief Returns the mass of all stages above the specified stage + the specified stage's mass.
     * @param inputStage The stage.
     * @return Mass of rocket during this stage prior to any burn or mass loss.
     */
     unsigned long long getRemainingMass (const Stage* inputStage) {
         unsigned long long remainingMass = mass;
        for (auto & stage : stages) {
            if (&stage == inputStage) {
                return remainingMass;
            }
            remainingMass -= stage.totalMass;
        }
        return remainingMass;
    }
     unsigned long long getRemainingMass (const int inputStageIndex) {
         unsigned long long remainingMass = mass;
        uint i = 0;
        for (const auto& stage : stages) {
            if (i == inputStageIndex) {
                return remainingMass;
            }
            remainingMass -= stage.totalMass;
            i++;
        }
        std::cerr << "Error: Stage index out of range." << std::endl;
        return remainingMass;
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
     unsigned long long getStageDryMass(Stage* stage) {
        return stage->dryMass;
    }

    /**
     * @brief Returns the fuel mass of a stage.
     * @param stage Pointer to the stage.
     * @return Fuel mass of the stage.
     */
     unsigned long long getStageFuelMass(Stage* stage) {
        return stage->fuelMass;
    }

    /**
     * @brief Returns the delta-V of a stage.
     * @param stage Pointer to the stage.
     * @return Delta-V of the stage.
     */
     unsigned long long getStageDeltaV(Stage* stage) {
        return stage->deltaV;
    }

    /**
     * @brief Returns the total mass of a stage.
     * @param stage Pointer to the stage.
     * @return Total mass of the stage.
     */
     unsigned long long getStrageTotalMass(Stage* stage) {
        return stage->totalMass;
    }

    /**
     * @brief Returns the total mass of the spaceship.
     * @return Total mass of the spaceship.
     */
     unsigned long long getMass() {
        return mass;
    }

    /**
     * @brief Returns the total delta-V of the spaceship.
     * @return Total delta-V of the spaceship.
     */
     unsigned long long getDeltaV() {
        return deltaV;
    }

    /**
     * @brief Sets the dry mass of a stage.
     * @param stage Pointer to the stage.
     * @param newMass The new dry mass.
     */
    void setStageDryMass(Stage* stage, const unsigned long long newMass) {
        mass += newMass - stage->dryMass;
        stage->dryMass = newMass;
        stage->totalMass = stage->dryMass + stage->fuelMass + stage->engine.mass;
        genDeltaV(stage);
    }

    /**
     * @brief Sets the fuel mass of a stage.
     * @param stage Pointer to the stage.
     * @param newMass The new fuel mass.
     */
    void setStageFuelMass(Stage* stage, const  unsigned long long newMass) {
        mass += newMass - stage->fuelMass;
        stage->fuelMass = newMass;
        stage->totalMass = stage->dryMass + stage->fuelMass + stage->engine.mass;
        genDeltaV(stage);
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
        genDeltaV(stage);
    }

    /**
     * @brief Adds a stage to the spaceship.
     * @param dryMass The dry mass of the stage.
     * @param fuelMass The fuel mass of the stage.
     * @param engine The engine used in the stage.
     * @param index The index at which to insert the stage (optional).
     */
    void addStage(const  unsigned long long dryMass, const  unsigned long long fuelMass, const Engine engine, const int index = -1) {
        if (index != -1) {
            stages.insert(stages.begin() + index, {engine, 0, dryMass, fuelMass, dryMass + fuelMass + engine.mass});
        } else {
            stages.push_back({engine, 0, dryMass, fuelMass, dryMass + fuelMass + engine.mass});
        }
        mass += dryMass + fuelMass + engine.mass;
        genDeltaV();
    }

    void printStats() {
        printf("DeltaV: %llu m/s\n", this->getDeltaV());
        printf("Mass: %llu kg\n", this->getMass());
        uint i = 0;
        for (auto stagesiter = this->stages.begin(); stagesiter != this->stages.end(); stagesiter++) {
            printf("Stage %u:\n", i);
            printf("\tTotal Stage Mass: %llu kg\n", fromPrecise(stagesiter->totalMass));
            printf("\tTotal Mass for remaining stages: %llu kg\n", fromPrecise(this->getRemainingMass(&(*stagesiter))));
            printf("\tDeltaV: %llu m/s\n", fromPrecise(stagesiter->deltaV));
            printf("\tDryMass: %llu kg\n", fromPrecise(stagesiter->dryMass));
            printf("\tFuelMass: %llu kg\n", fromPrecise(stagesiter->fuelMass));
            printf("\tEngine %s:\n", stagesiter->engine.name);
            printf("\t\tMass: %llu kg\n", fromPrecise(stagesiter->engine.mass));
            printf("\t\tExhaustVelocity: %llu m/s\n", fromPrecise(stagesiter->engine.exhaustVelocity));
        }
    }

protected:
    std::vector<Stage> stages; /**< Vector of stages. */
    unsigned long long mass;               /**< Total mass of the spaceship. */
    unsigned long long deltaV;             /**< Total delta-V of the spaceship. */

    /**
     * @brief Generates the delta-V for the spaceship or a specific stage.
     * @param inpStage Pointer to the input stage (optional).
     *
     * If inpStage is specified, it calculates the delta-V only for stages before the new stage.
     * Otherwise, it calculates the delta-V for all stages.
     */
    void genDeltaV (Stage* inpStage = nullptr) {       // for addStage, this should implement only calcs on stages before new
        if (inpStage) {
             unsigned long long remainingMass = getRemainingMass(inpStage);
            deltaV -= inpStage->deltaV;
            inpStage->deltaV = inpStage->engine.exhaustVelocity * log(remainingMass / (remainingMass - inpStage->fuelMass));
            deltaV += inpStage->deltaV;
        } else {
             unsigned long long remainingMass = mass;
            deltaV = 0;
            for (auto &stage: stages) {
                stage.deltaV = stage.engine.exhaustVelocity * log(remainingMass / (remainingMass - stage.fuelMass));
                deltaV += stage.deltaV;
                remainingMass -= stage.totalMass;
            }
        }
    }

};


#endif //SRC_SPACESHIP_H
