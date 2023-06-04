#include <vector>
#include <iostream>
#include <random>
#include <cstdlib>
#include "SpaceShipHandler.h"
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

void doubleTest(const mpfr_t mpfrA, const long double b, char* name) {
    long double a = mpfr_get_ld(mpfrA, MPFR_RNDN);
    printf("%20s variance: %30Le\n", name, 100 * (a - b) / a);
    CHECK_THAT(a, Catch::Matchers::WithinRel(b, .00001));
};

void doubleTest(const long double a, const long double b, char* name) {
    printf("%20s variance: %30Le\n", name, 100 * (a - b) / a);
    CHECK_THAT(a, Catch::Matchers::WithinAbs(b, .00001));
};

TEST_CASE("Accuracy") {
    { // TEST SPACESHIP 1 - Random:
        SpaceShipHandler handler(1024);
        auto ship = handler.addShip();
        handler.createEngine("1.1", 64958.37813684586060647419003544200678, 3617.40857668100880828454535276250681);
        ship->addStage(46381.56737479119504996560863219201565, 3552.14007770645868733438987874251325, handler.getEngine("1.1"));

        mpfr_t deltaV;
        ship->getRawDeltaV(deltaV);
        // Result generated by keisan high precision calculator. Most calculators are around this same value.
        doubleTest(deltaV, 203.42606155927194207166264721281638984133345653072, "Ship 1");
        mpfr_clear(deltaV);
    }

    { // TEST SPACESHIP 2 - Random:
        SpaceShipHandler handler(1024);
        auto ship = handler.addShip();
        handler.createEngine("2.1", 52851.89721221082680102654194342903793, 44949670902.83464924991130828857421875000000);
        ship->addStage(52967195.75027480287462822161614894866943, 976209.93331809725617631556815467774868, handler.getEngine("2.1"));

        mpfr_t deltaV;
        ship->getRawDeltaV(deltaV);
        // Result generated by known good IRA configuration, but also checked with calculator.
        doubleTest(deltaV, 820090589.49364814470754936337471008300781, "Ship 2");
        mpfr_clear(deltaV);
    }
}

TEST_CASE("Runtime Errors") {
    SpaceShipHandler handler(1024);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint> magnitude(1, 4);
    std::uniform_int_distribution<uint> chanceA(1, magnitude(gen));
    std::uniform_int_distribution<uint> chanceB(1, magnitude(gen));

    for (uint i = 0; i < 100; i++) {
        auto ship = handler.addShip();

    }

}

long double genDeltaV(long double remaningMass, long double fuelMass, long double exhaustVelocity) {
    //return log((remaningMass - fuelMass) / remaningMass) * exhaustVelocity;
    mpfr_prec_t precision = 1024;
    mpfr_set_default_prec(precision);

    mpfr_t result;
    mpfr_init2(result, 1024);
    mpfr_set_ld(result, remaningMass, MPFR_RNDN);

    mpfr_t denominator;
    mpfr_init2(denominator, 1024);
    mpfr_set_ld(denominator, remaningMass - fuelMass, MPFR_RNDN);

    mpfr_div(result, result, denominator, MPFR_RNDN);
    mpfr_log(result, result, MPFR_RNDN);
    mpfr_mul_d(result, result, exhaustVelocity, MPFR_RNDN);

    long double toReturn = mpfr_get_ld(result, MPFR_RNDN);
    mpfr_clear(result);
    mpfr_clear(denominator);
    return toReturn;
}

struct stage {
    long double dryMass, fuelMass, engineMass, exhaustVelocity;

    stage(long double dryMass, long double fuelMass, long double engineMass, long double exhaustVelocity) :
            dryMass(dryMass), fuelMass(fuelMass), engineMass(engineMass), exhaustVelocity(exhaustVelocity) {}
};

TEST_CASE("Crazy") {

    for (int j = 0; j < 1000; j++) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<long double> valRange(1, 1'000'000'000.0);
        std::uniform_int_distribution<uint> stageRange(10, 30);


        std::vector<stage*> stageList;

        SpaceShipHandler handler(1024);
        auto* ship = handler.addShip();
        const uint stageCount = stageRange(gen);
        for (uint i = 0; i < stageCount; i++) {
            Engine newEngine;
            mpfr_set_ld(newEngine.mass, valRange(gen), MPFR_RNDN);
            mpfr_set_ld(newEngine.exhaustVelocity, valRange(gen), MPFR_RNDN);

            std::string name = ("S" + std::to_string(j) + "." + std::to_string(i)).c_str();


            stageList.push_back(new stage(valRange(gen), valRange(gen),
                                          mpfr_get_ld(newEngine.mass, MPFR_RNDN),mpfr_get_ld(newEngine.exhaustVelocity, MPFR_RNDN)));

            handler.createEngine(name, stageList.at(i)->dryMass, stageList.at(i)->fuelMass);
            ship->addStage(stageList.at(i)->dryMass, stageList.at(i)->fuelMass, handler.getEngine(name));
        }


        auto fullTest = [&stageList, &ship, &stageCount](char *name) {
            long double rollingMass = 0;
            auto localStage = --stageList.end();
            auto stageIter = --ship->getStages()->end();
            printf("--------------------# %s --------------------\n", name);
            for (uint i = stageList.size() - 1; i != 0; i--) {
                printf("--------------------## stage %d--------------------\n", i);
                printf("Verified: rM: %Lf, f: %Lf, e.m: %Lf, e.eV: %Lf, dM: %Lf\n",
                       rollingMass, (*localStage)->fuelMass, (*localStage)->engineMass,
                       (*localStage)->exhaustVelocity, mpfr_get_ld((*stageIter)->dryMass, MPFR_RNDN));
                doubleTest((*stageIter)->dryMass, (*localStage)->dryMass, (char *) "Dry Mass");
                doubleTest((*stageIter)->fuelMass, (*localStage)->fuelMass, (char *) "Fuel Mass");
                doubleTest((*stageIter)->engine->mass, (*localStage)->engineMass, (char *) "Engine Mass");
                doubleTest((*stageIter)->engine->exhaustVelocity, (*localStage)->exhaustVelocity,
                           (char *) "Exhaust Velocity");
                doubleTest((*stageIter)->totalMass,
                           (*localStage)->dryMass + (*localStage)->fuelMass + (*localStage)->engineMass,
                           (char *) "Total Mass");

                rollingMass += (*localStage)->dryMass + (*localStage)->fuelMass + (*localStage)->engineMass;


                doubleTest(ship->getRemainingMass(i), rollingMass, (char *) "Remaining Mass");

                (*localStage)->exhaustVelocity, mpfr_get_ld((*stageIter)->deltaV, MPFR_RNDN),
                        genDeltaV(rollingMass, (*localStage)->fuelMass, (*localStage)->exhaustVelocity);
                doubleTest((*stageIter)->deltaV,
                           genDeltaV(rollingMass, (*localStage)->fuelMass, (*localStage)->exhaustVelocity),
                           (char *) "Delta V");
                stageIter--;
                localStage--;
            }
        };

        std::uniform_int_distribution<int> modifyValChanceRange(1, 256);
        std::uniform_int_distribution<int> modifyValChance(-modifyValChanceRange(gen), modifyValChanceRange(gen));
        auto localStage = --stageList.end();
        auto stageIter = --ship->getStages()->end();
        for (uint i = stageList.size() - 1; i != 0; i--) {

            if (0 < modifyValChance(gen)) {
                (*localStage)->dryMass = valRange(gen);
                ship->setStageDryMass(i, (*localStage)->dryMass);
            }
            if (0 < modifyValChance(gen)) {
                (*localStage)->fuelMass = valRange(gen);
                ship->setStageFuelMass(i, (*localStage)->fuelMass);
            }
            if (0 < modifyValChance(gen)) {
                Engine newEngine;
                mpfr_set_ld(newEngine.mass, valRange(gen), MPFR_RNDN);
                mpfr_set_ld(newEngine.exhaustVelocity, valRange(gen), MPFR_RNDN);
                newEngine.name = ("S" + std::to_string(i)).c_str();

                (*localStage)->engineMass = mpfr_get_ld(newEngine.mass, MPFR_RNDN);
                (*localStage)->exhaustVelocity = mpfr_get_ld(newEngine.exhaustVelocity, MPFR_RNDN);

            }

            --localStage;
            --stageIter;
        }
        fullTest((char *) "Modified");
        for (auto stage: stageList) {
            delete (stage);
        }
//}
        mpfr_free_cache();
    }
}