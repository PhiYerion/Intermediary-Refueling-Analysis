//
// Created by user on 5/25/23.
//

#ifndef IRA_ENGINE_H
#define IRA_ENGINE_H

#include <vector>
#include <mpfr.h>
#include <string>

class Engine {
public:
    mpfr_t mass,                                /**< Mass of the engine. */
    exhaustVelocity;                            /**< Exhaust velocity of the engine. */
    std::string name;                           /**< Name of the engine. */

    Engine();
    ~Engine();

    Engine& operator=(const Engine& other);
    Engine(const Engine& other);
    Engine& operator=(Engine&& other) noexcept;
    Engine(Engine&& other) noexcept;

};


#endif //IRA_ENGINE_H
