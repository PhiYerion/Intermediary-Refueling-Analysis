//
// Created by user on 5/22/23.
//

#ifndef IRA_MPFR_INTERFACE_H
#define IRA_MPFR_INTERFACE_H

#include "lib/mpfr.h"
#include <cstdio>
#include <type_traits>
#include <stdexcept>

/*
 * Functions for MPFR for this project. Largely wraps the MPFR library.
 */
class mpfr_interface {
private:
    const static unsigned int precision = 1024;
public:
    static void DeltaV (mpfr_t* result, mpfr_t mass, mpfr_t fuelMass, mpfr_t exhaustVelocity) {
        mpfr_prec_t mpfrPrecision = precision;
        mpfr_set_default_prec(mpfrPrecision);

        mpfr_set(*result, mass, MPFR_RNDN);

        mpfr_t denominator;
        mpfr_init2(denominator, precision);
        mpfr_set_ld(denominator, mass - fuelMass, MPFR_RNDN);

        mpfr_div(*result, mass, denominator, MPFR_RNDN);
        mpfr_log(*result, *result, MPFR_RNDN);
        mpfr_mul(*result, *result, exhaustVelocity, MPFR_RNDN);

        mpfr_clear(denominator);
    }

    template <typename T>
    void convertToMpfr(mpfr_t result, const T& value)
    {
        static_assert(std::is_arithmetic<T>::value, "Only arithmetic types are supported.");

        // Use type traits to select the appropriate conversion function
        if (std::is_floating_point<T>::value)
        {
            mpfr_set_d(result, static_cast<double>(value), MPFR_RNDN);
        }
        else if (std::is_integral<T>::value)
        {
            mpfr_set_si(result, static_cast<long>(value), MPFR_RNDN);
        }
        else
        {
            throw std::invalid_argument("Unsupported type.");
        }
    }
};

#endif //IRA_MPFR_INTERFACE_H
