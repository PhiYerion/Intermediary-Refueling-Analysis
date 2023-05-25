//
// Created by user on 5/25/23.
//

#ifndef IRA_STAGE_H
#define IRA_STAGE_H

#include "Engine.h"

/**
 * @brief Represents a stage of a spaceship.
 *
 * Any loss in non-fuel mass mid-stage will not be accounted for.
 */
class Stage {
public:
    Engine engine;                /**< Engine used in the stage. */
    mpfr_t deltaV,                /**< Delta-V of the stage. */
    dryMass,                      /**< Dry mass of the stage (excluding engine mass). */
    fuelMass,                     /**< Fuel mass of the stage. */
    totalMass;                    /**< Total mass of the stage (including engine mass). */

    Stage();
    ~Stage();

    Stage& operator=(const Stage& other);
    Stage(const Stage& other);

    Stage& operator=(Stage&& other) noexcept;
    Stage(Stage&& other) noexcept;
};


#endif //IRA_STAGE_H
