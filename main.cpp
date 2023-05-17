#include <vector>
#include <iostream>
#include <valarray>

/**
 * @brief Represents an engine.
 */
struct Engine {
    double mass;               /**< Mass of the engine. */
    double exhaustVelocity;    /**< Exhaust velocity of the engine. */
    const char* name;          /**< Name of the engine. */
};

/**
 * @brief Represents a stage of a spaceship.
 *
 * Any loss in non-fuel mass mid-stage will not be accounted for.
 */
struct Stage {
    Engine engine;             /**< Engine used in the stage. */
    double deltaV,             /**< Delta-V of the stage. */
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
    double getRemainingMass (const Stage* inputStage) {
        double remainingMass = mass;
        for (auto stagesiter = stages.begin(); stagesiter != stages.end(); stagesiter++) {
            if (&(*stagesiter) == inputStage) {
                return remainingMass;
            }
            remainingMass -= stagesiter->totalMass;
        }
        return remainingMass;
    }

    /**
    * @brief Returns the engine of a stage.
    * @param stage Pointer to the stage.
    * @return Engine of the stage.
    */
    Engine getEngine (Stage* stage) {
        stage -> engine;
    }

    /**
    * @brief Returns the dry mass of a stage.
    * @param stage Pointer to the stage.
    * @return Dry mass of the stage.
    */
    double getStageDryMass(Stage* stage) {
        return stage->dryMass;
    }

    /**
     * @brief Returns the fuel mass of a stage.
     * @param stage Pointer to the stage.
     * @return Fuel mass of the stage.
     */
    double getStageFuelMass(Stage* stage) {
        return stage->fuelMass;
    }

    /**
     * @brief Returns the delta-V of a stage.
     * @param stage Pointer to the stage.
     * @return Delta-V of the stage.
     */
    double getStageDeltaV(Stage* stage) {
        return stage->deltaV;
    }

    /**
     * @brief Returns the total mass of a stage.
     * @param stage Pointer to the stage.
     * @return Total mass of the stage.
     */
    double getStrageTotalMass(Stage* stage) {
        return stage->totalMass;
    }

    /**
     * @brief Returns the total mass of the spaceship.
     * @return Total mass of the spaceship.
     */
    double getMass() {
        return mass;
    }

    /**
     * @brief Returns the total delta-V of the spaceship.
     * @return Total delta-V of the spaceship.
     */
    double getDeltaV() {
        return deltaV;
    }

    /**
     * @brief Sets the dry mass of a stage.
     * @param stage Pointer to the stage.
     * @param newMass The new dry mass.
     */
    void setStageDryMass(Stage* stage, const double newMass) {
        mass += newMass - stage->dryMass;
        stage->dryMass = newMass;
        genDeltaV(stage);
    }

    /**
     * @brief Sets the fuel mass of a stage.
     * @param stage Pointer to the stage.
     * @param newMass The new fuel mass.
     */
    void setStageFuelMass(Stage* stage, const double newMass) {
        mass += newMass - stage->fuelMass;
        stage->fuelMass = newMass;
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
        genDeltaV(stage);
    }

    /**
     * @brief Adds a stage to the spaceship.
     * @param dryMass The dry mass of the stage.
     * @param fuelMass The fuel mass of the stage.
     * @param engine The engine used in the stage.
     * @param index The index at which to insert the stage (optional).
     */
    void addStage(const double dryMass, const double fuelMass, const Engine engine, const int index = -1) {
        if (index != -1) {
            stages.insert(stages.begin() + index, {engine, 0, dryMass, fuelMass, dryMass + fuelMass + engine.mass});
        } else {
            stages.push_back({engine, 0, dryMass, fuelMass, dryMass + fuelMass + engine.mass});
            std::cerr << "Warning: index not specified for addStage, appending to end of stages\n";
        }
        mass += dryMass + fuelMass + engine.mass;
        genDeltaV();
    }

    void printStats() {
        printf("DeltaV: %fm/s\n", this->getDeltaV());
        printf("Mass: %fkg\n", this->getMass());
        uint i = 0;
        for (auto stagesiter = this->stages.begin(); stagesiter != this->stages.end(); stagesiter++) {
            printf("Stage %u:\n", i);
            printf("\tTotal Stage Mass: %fkg\n", stagesiter->totalMass);
            printf("\tTotal Mass for remaining stages: %fkg\n", this->getRemainingMass(&(*stagesiter)));
            printf("\tDeltaV: %fm/s\n", stagesiter->deltaV);
            printf("\tDryMass: %fkg\n", stagesiter->dryMass);
            printf("\tFuelMass: %fkg\n", stagesiter->fuelMass);
            printf("\tEngine %s:\n", stagesiter->engine.name);
            printf("\t\tMass: %fkg\n", stagesiter->engine.mass);
            printf("\t\tExhaustVelocity: %fm/s\n", stagesiter->engine.exhaustVelocity);
        }
    }

protected:
    std::vector<Stage> stages; /**< Vector of stages. */
    double mass;               /**< Total mass of the spaceship. */
    double deltaV;             /**< Total delta-V of the spaceship. */

    /**
     * @brief Generates the delta-V for the spaceship or a specific stage.
     * @param inpStage Pointer to the input stage (optional).
     *
     * If inpStage is specified, it calculates the delta-V only for stages before the new stage.
     * Otherwise, it calculates the delta-V for all stages.
     */
    void genDeltaV (Stage* inpStage = nullptr) {       // for addStage, this should implement only calcs on stages before new
        if (inpStage) {
            double remainingMass = getRemainingMass(inpStage);
            deltaV -= inpStage->deltaV;
            inpStage->deltaV = inpStage->engine.exhaustVelocity * log(remainingMass / (remainingMass - inpStage->fuelMass));
            deltaV += inpStage->deltaV;
        } else {
            double remainingMass = mass;
            deltaV = 0;
            for (auto &stage: stages) {
                stage.deltaV = stage.engine.exhaustVelocity * log(remainingMass / (remainingMass - stage.fuelMass));
                deltaV += stage.deltaV;
                remainingMass -= stage.totalMass;
            }
        }
    }

};

int main () {
    auto* ship = new SpaceShip();
    ship->addStage(100, 1, {100, 100, "S1"});
    ship->addStage(50, 20, {5, 1000, "S0"});
    ship->printStats();
    free(ship);
    return 1;
}