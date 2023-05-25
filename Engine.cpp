//
// Created by user on 5/25/23.
//

#include "SpaceShip.h"
#include "Engine.h"
#include <cstring>
#include <cstdio>
#include <mpfr.h>
#include <cmath>

Engine::Engine() {
    mpfr_init(mass);
    mpfr_init(exhaustVelocity);
}

Engine::~Engine() {
    if (mass[0]._mpfr_d != nullptr) {                                    // Errors pretaining to erroneous clearing
        free(name);                                                 // should be resolved, so the lack of hard
        mpfr_clear(mass);                                                // errors when nullptr is encountered is
        mpfr_clear(exhaustVelocity);                                     // acceptable and needed due to nullptrs
    }                                                                    // in the case that there is a move operation
}

// Copy operations:
Engine::Engine(const Engine& other) {
    mpfr_init(mass);
    mpfr_init(exhaustVelocity);
    mpfr_set(mass, other.mass, MPFR_RNDN);
    mpfr_set(exhaustVelocity, other.exhaustVelocity, MPFR_RNDN);

    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);
}
Engine& Engine::operator=(const Engine& other) {
    if (this == &other) {
        return *this; // Handle self-assignment
    }
    mpfr_set(mass, other.mass, MPFR_RNDN);
    mpfr_set(exhaustVelocity, other.exhaustVelocity, MPFR_RNDN);

    name = new char[strlen(other.name) + 1];
    strcpy(name, other.name);

    return *this;
}

// Move operations:
Engine::Engine(Engine&& other) noexcept {
    mass[0] = other.mass[0];
    other.mass[0]._mpfr_d = nullptr;

    exhaustVelocity[0] = other.exhaustVelocity[0];
    other.exhaustVelocity[0]._mpfr_d = nullptr;

    name = other.name;
    other.name = nullptr;
}

Engine& Engine::operator=(Engine&& other)  noexcept {
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