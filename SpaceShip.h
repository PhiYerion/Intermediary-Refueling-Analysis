
#ifndef SRC_SPACESHIP_H
#define SRC_SPACESHIP_H

#include <mpfr.h>
#include <vector>
#include "Stage.h"
#include "Engine.h"

/**
 * @brief Spaceship class with full functionality.
 *
 * This class provides getters and setters for accessing and modifying the spaceship's attributes.
 * Directly setting variables will produce inaccurate measurements, so they are protected.
 */
class SpaceShip {
    friend class SpaceShipHandler;

protected:
    std::vector<Stage*> stages;  /**< Vector of stages. */
    mpfr_t mass,                 /**< Total mass of the spaceship. */
    deltaV;                      /**< Total delta-V of the spaceship. */

    /**
     * @brief Generates the delta-V for the spaceship or a specific stage.
     * @param inpStage Pointer to the input stage (optional).
     *
     * If inpStage is specified, it calculates the delta-V only for stages before the new stage.
     * Otherwise, it calculates the delta-V for all stages.
     */
    void genDeltaV ();

public:
    SpaceShip();

    ~SpaceShip();

    /**
      * @brief Returns the vector of stages.
      * @return Vector of stages.
      */
    std::vector<Stage*>* getStages ();

    void getRemainingMass (mpfr_t result, const Stage* inputStage);

    /**
     * @brief Sets the engine of a stage.
     * @param stage Pointer to the stage.
     * @param newEngine The new engine.
     */
    //void setStageEngine(Stage* stage, Engine newEngine);

    /**
     * @brief Adds a stage to the spaceship.
     * @param dryMass The dry mass of the stage.
     * @param fuelMass The fuel mass of the stage.
     * @param engine The engine used in the stage.
     * @param index The index at which to insert the stage (optional).
     */

    void addStage(mpfr_t dryMass, mpfr_t fuelMass, Engine engine, const int index = -1);

    /**
     * @brief Sets the dry mass of a stage.
     * @param stage Pointer to the stage.
     * @param newMass The new dry mass.
     */
    void setStageDryMass(Stage* stage, const mpfr_t newMass);

    /**
     * @brief Sets the fuel mass of a stage.
     * @param stage Pointer to the stage.
     * @param newMass The new fuel mass.
     */
    void setStageFuelMass(Stage* stage, const mpfr_t newMass);

    //void setStageEngine(Stage *stage, Engine newEngine);
    void setStageEngine(Stage* stage, Engine newEngine);
};

#endif //SRC_SPACESHIP_H