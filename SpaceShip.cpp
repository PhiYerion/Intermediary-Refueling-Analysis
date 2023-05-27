#include "SpaceShip.h"
#include "Stage.h"
#include <mpfr.h>

#define PRECISION 1024

/**
 * @brief Generates the delta-V for the spaceship or a specific stage.
 * @param inpStage Pointer to the input stage (optional).
 *
 * If inpStage is specified, it calculates the delta-V only for stages before the new stage.
 * Otherwise, it calculates the delta-V for all stages.
 */
void SpaceShip::genDeltaV () {       // for addStage, this should implement only calcs on stages before new

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
        mpfr_mul(stage->deltaV, stage->deltaV, stage->engine->exhaustVelocity, MPFR_RNDN);

        mpfr_add(deltaV, deltaV, stage->deltaV, MPFR_RNDN);
        mpfr_sub(remainingMass, remainingMass, stage->totalMass, MPFR_RNDN);
    }
    mpfr_clear(denominator);
    mpfr_clear(remainingMass);
}

SpaceShip::SpaceShip() {
    mpfr_set_default_prec(PRECISION);
    mpfr_init(mass);
    mpfr_init(deltaV);
    mpfr_set_zero(mass, 0);
    mpfr_set_zero(deltaV, 0);
}

SpaceShip::~SpaceShip() {
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
std::vector<Stage*>* SpaceShip::getStages () {
    return &stages;
}

void SpaceShip::getRemainingMass (mpfr_t result, const Stage* inputStage) {
    mpfr_set(result, mass, MPFR_RNDN);
    for (auto & stage : stages) {
        if (stage == inputStage) {
            return;
        }
        mpfr_sub(result, result, stage->totalMass, MPFR_RNDN);
    }
}

/*    void getRemainingMass (mpfr_t result, const int inputStageIndex) {
    mpfr_set(result, mass, MPFR_RNDN);
    uint i = 0;
    for (const auto& stage : stages) {
        if (i == inputStageIndex) {
            return;
        }
        mpfr_sub(result, result, stage->totalMass, MPFR_RNDN);
        i++;
    }
}*/

/**
 * @brief Sets the engine of a stage.
 * @param stage Pointer to the stage.
 * @param newEngine The new engine.
 */
void SpaceShip::setStageEngine(Stage* stage, Engine newEngine) {
    mpfr_sub(mass, mass, stage->engine->mass, MPFR_RNDN);
    mpfr_sub(stage->totalMass, stage->totalMass, stage->engine->mass, MPFR_RNDN);

    mpfr_add(mass, mass, newEngine.mass, MPFR_RNDN);
    mpfr_add(stage->totalMass, stage->totalMass, newEngine.mass, MPFR_RNDN);

    *stage->engine = std::move(newEngine);
    genDeltaV();
}

void SpaceShip::setStageEngine(Stage* stage, Engine* newEngine) {
    mpfr_sub(mass, mass, stage->engine->mass, MPFR_RNDN);
    mpfr_sub(stage->totalMass, stage->totalMass, stage->engine->mass, MPFR_RNDN);

    mpfr_add(mass, mass, newEngine->mass, MPFR_RNDN);
    mpfr_add(stage->totalMass, stage->totalMass, newEngine->mass, MPFR_RNDN);

    stage->engine = newEngine;
    genDeltaV();
}


/**
 * @brief Adds a stage to the spaceship.
 * @param dryMass The dry mass of the stage.
 * @param fuelMass The fuel mass of the stage.
 * @param engine The engine used in the stage.
 * @param index The index at which to insert the stage (optional).
 */

void SpaceShip::addStage(mpfr_t dryMass, mpfr_t fuelMass, Engine engine, const int index) {

    Stage* stage;
    if (index != -1) {
        stages.insert(stages.begin() + index, new Stage());
        stage = stages[index];
    } else {
        stages.push_back(new Stage());
        stage = stages.back();
        //std::cerr << "Warning: index not specified for addStage, appending to end of stages\n";
    }
    *stage->engine = std::move(engine);

    mpfr_set(stage->dryMass, dryMass, MPFR_RNDN);
    mpfr_set(stage->fuelMass, fuelMass, MPFR_RNDN);

    mpfr_add(stage->totalMass, stage->dryMass, stage->fuelMass, MPFR_RNDN);
    mpfr_add(stage->totalMass, stage->totalMass, stage->engine->mass, MPFR_RNDN);

    mpfr_add(mass, mass, stage->totalMass, MPFR_RNDN);
    genDeltaV();
}


void SpaceShip::addStage(mpfr_t dryMass, mpfr_t fuelMass, Engine* engine, const int index) {

    Stage* stage;
    if (index != -1) {
        stages.insert(stages.begin() + index, new Stage());
        stage = stages[index];
    } else {
        stages.push_back(new Stage());
        stage = stages.back();
        //std::cerr << "Warning: index not specified for addStage, appending to end of stages\n";
    }
    stage->engine = engine;

    mpfr_set(stage->dryMass, dryMass, MPFR_RNDN);
    mpfr_set(stage->fuelMass, fuelMass, MPFR_RNDN);

    mpfr_add(stage->totalMass, stage->dryMass, stage->fuelMass, MPFR_RNDN);
    mpfr_add(stage->totalMass, stage->totalMass, stage->engine->mass, MPFR_RNDN);

    mpfr_add(mass, mass, stage->totalMass, MPFR_RNDN);
    genDeltaV();
}

/**
* @brief Sets the dry mass of a stage.
* @param stage Pointer to the stage.
* @param newMass The new dry mass.
*/
void SpaceShip::setStageDryMass(Stage* stage, const mpfr_t newMass) {
    mpfr_sub(mass, mass, stage->dryMass, MPFR_RNDN);
    mpfr_sub(stage->totalMass, stage->totalMass, stage->dryMass, MPFR_RNDN);

    mpfr_add(mass, mass, newMass, MPFR_RNDN);
    mpfr_add(stage->totalMass, stage->totalMass, newMass, MPFR_RNDN);

    mpfr_set(stage->dryMass, newMass, MPFR_RNDN); // stage->dryMass = newMass;
    genDeltaV();
}

/**
 * @brief Sets the fuel mass of a stage.
 * @param stage Pointer to the stage.
 * @param newMass The new fuel mass.
 */
void SpaceShip::setStageFuelMass(Stage* stage, const mpfr_t newMass) {
    mpfr_sub(mass, mass, stage->fuelMass, MPFR_RNDN);
    mpfr_sub(stage->totalMass, stage->totalMass, stage->fuelMass, MPFR_RNDN);

    mpfr_add(mass, mass, newMass, MPFR_RNDN);
    mpfr_add(stage->totalMass, stage->totalMass, newMass, MPFR_RNDN);

    mpfr_set(stage->fuelMass, newMass, MPFR_RNDN); // stage->dryMass = newMass;
    genDeltaV();
}

