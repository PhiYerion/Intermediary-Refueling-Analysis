//
// Created by user on 5/25/23.
//

#include "Stage.h"
#include "Engine.h"
#include "iostream"
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
    // engine is handled by an engine handler, not this class.
}

// Copy operations:
Stage& Stage::operator=(const Stage& other) {
    // This before just for more chances to catch this error.
    if (other.engine == nullptr) {
        std::cerr << "[Stage::operator=] Move invalid. Check engine handler." << std::endl;
        throw std::runtime_error("Null pointer exception");
    }
    if (this == &other) {                                               // Handle self-assignment
        return *this;
    }

    mpfr_set(deltaV, other.deltaV, MPFR_RNDN);                          // Make sure that the underlying mpfr_t values
    mpfr_set(dryMass, other.dryMass, MPFR_RNDN);                        // are copied instead of just the pointers
    mpfr_set(fuelMass, other.fuelMass, MPFR_RNDN);
    mpfr_set(totalMass, other.totalMass, MPFR_RNDN);

    engine = other.engine;                                              // Engine handler's job to keep this valid

    return *this;
}
Stage::Stage(const Stage& other) {
    if (other.engine == nullptr) {
        std::cerr << "[Stage::operator=] Move invalid. Check engine handler." << std::endl;
        throw std::runtime_error("Null pointer exception");
    }
    mpfr_init(deltaV);                                                  // Make sure that the underlying mpfr_t values
    mpfr_init(dryMass);                                                 // are copied instead of just the pointers.
    mpfr_init(fuelMass);                                                // This is effectively the same as the
    mpfr_init(totalMass);                                               // move operator above, but also initializes
    mpfr_set(deltaV, other.deltaV, MPFR_RNDN);                          // the mpfr_t values.
    mpfr_set(dryMass, other.dryMass, MPFR_RNDN);
    mpfr_set(fuelMass, other.fuelMass, MPFR_RNDN);
    mpfr_set(totalMass, other.totalMass, MPFR_RNDN);

    engine = other.engine;                                              // Engine handler's job to keep this valid
}

// Move operations:
Stage& Stage::operator=(Stage&& other) noexcept {
    // should only be in debugging, so exception here should be removed after.
    if (other.engine == nullptr) {
        std::cerr << "[Stage::operator=] Move invalid. Check engine handler." << std::endl;
    }

    if (this == &other) {
        return *this;                                                   // Handle self-assignment
    }

    deltaV[0] = other.deltaV[0];                                        // Point the new mpfr_t pointers to the data
    dryMass[0] = other.dryMass[0];                                      // of the original object.
    fuelMass[0] = other.fuelMass[0];
    totalMass[0] = other.totalMass[0];
    engine = other.engine;                                              // Engine handler's job to keep this valid

    return *this;
}
Stage::Stage(Stage&& other) noexcept {
    // should only be in debugging, so exception here should be removed after.
    if (other.engine == nullptr) {
        std::cerr << "[Stage::operator=] Move invalid. Check engine handler." << std::endl;
    }
    deltaV[0] = other.deltaV[0];
    dryMass[0] = other.dryMass[0];
    fuelMass[0] = other.fuelMass[0];
    totalMass[0] = other.totalMass[0];
    engine = other.engine;                                              // Engine handler's job to keep this valid
}