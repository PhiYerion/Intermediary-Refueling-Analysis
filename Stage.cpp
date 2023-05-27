//
// Created by user on 5/25/23.
//

#include "Stage.h"
#include "Engine.h"
#include <cstdio>
#include <mpfr.h>



Stage::Stage() {
    mpfr_init(deltaV);
    mpfr_init(dryMass);
    mpfr_init(fuelMass);
    mpfr_init(totalMass);
}

Stage::~Stage() {
    mpfr_clear(deltaV);
    mpfr_clear(dryMass);
    mpfr_clear(fuelMass);
    mpfr_clear(totalMass);
    delete engine;
}

// Copy operations:
Stage& Stage::operator=(const Stage& other) {
    if (this == &other) {                                               // Handle self-assignment
        return *this;
    }
    mpfr_set(deltaV, other.deltaV, MPFR_RNDN);                          // Make sure that the underlying mpfr_t values
    mpfr_set(dryMass, other.dryMass, MPFR_RNDN);                        // are copied instead of just the pointers
    mpfr_set(fuelMass, other.fuelMass, MPFR_RNDN);
    mpfr_set(totalMass, other.totalMass, MPFR_RNDN);
    *engine = *other.engine;                                              // This is a copy operation

    return *this;
}
Stage::Stage(const Stage& other) {
    mpfr_init(deltaV);                                                  // Make sure that the underlying mpfr_t values
    mpfr_init(dryMass);                                                 // are copied instead of just the pointers.
    mpfr_init(fuelMass);                                                // This is effectively the same as the
    mpfr_init(totalMass);                                               // move operator above, but also initializes
    mpfr_set(deltaV, other.deltaV, MPFR_RNDN);                          // the mpfr_t values.
    mpfr_set(dryMass, other.dryMass, MPFR_RNDN);
    mpfr_set(fuelMass, other.fuelMass, MPFR_RNDN);
    mpfr_set(totalMass, other.totalMass, MPFR_RNDN);

    *engine = *other.engine;                                              // This is a copy operation
}

// Move operations:
Stage& Stage::operator=(Stage&& other)  noexcept {
    if (this == &other) {
        return *this;                                                   // Handle self-assignment
    }

    deltaV[0] = other.deltaV[0];                                        // Point the new mpfr_t pointers to the data
    dryMass[0] = other.dryMass[0];                                      // of the original object.
    fuelMass[0] = other.fuelMass[0];
    totalMass[0] = other.totalMass[0];
    engine = other.engine;

    return *this;
}
Stage::Stage(Stage&& other) noexcept {
    deltaV[0] = other.deltaV[0];
    dryMass[0] = other.dryMass[0];
    fuelMass[0] = other.fuelMass[0];
    totalMass[0] = other.totalMass[0];
    engine = other.engine;
}

/* Consider removing    // Getters:
long double getDeltaV() {
    return mpfr_get_ld(deltaV, MPFR_RNDN);
}
long double getDryMass() {
    return mpfr_get_ld(dryMass, MPFR_RNDN);
}
long double getFuelMass() {
    return mpfr_get_ld(fuelMass, MPFR_RNDN);
}
long double getTotalMass() {
    return mpfr_get_ld(totalMass, MPFR_RNDN);
}
long double getEngineMass() {
    return mpfr_get_ld(engine.mass, MPFR_RNDN);
}
long double getExhaustVelocity() {
    return mpfr_get_ld(engine.exhaustVelocity, MPFR_RNDN);
}*/
