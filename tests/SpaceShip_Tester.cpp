#include <random>
#include <vector>
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include "../SpaceShip.h"
#include <cstdio>
#include <mpfr.h>

void doubleTest(mpfr_t mpfrA, long double b, char* name) {
    long double a = mpfr_get_ld(mpfrA, MPFR_RNDN);
    printf("%20s variance: %30Le\n", name, 100 * (a - b) / a);
    CHECK_THAT(a, Catch::Matchers::WithinRel(b, .00001));
};

struct stage {
    long double dryMass, fuelMass, engineMass, exhaustVelocity;

    stage(long double dryMass, long double fuelMass, long double engineMass, long double exhaustVelocity) :
            dryMass(dryMass), fuelMass(fuelMass), engineMass(engineMass), exhaustVelocity(exhaustVelocity) {}
};

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

TEST_CASE("SpaceShip") {
    for (int j = 0; j < 1000; j++) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<long double> valRange(1, 1'000'000'000.0);
        std::uniform_int_distribution<uint> stageRange(10, 30);


        std::vector<stage *> stageList;

        auto *ship = new SpaceShip();
        const uint stageCount = stageRange(gen);
        for (uint i = 0; i < stageCount; i++) {
            Engine newEngine;
            mpfr_set_ld(newEngine.mass, valRange(gen), MPFR_RNDN);
            mpfr_set_ld(newEngine.exhaustVelocity, valRange(gen), MPFR_RNDN);

            const char* name = ("S" + std::to_string(i)).c_str();
            newEngine.name = new char[strlen(name) + 1];
            std::strcpy(newEngine.name, name);

            stageList.push_back(new stage(valRange(gen), valRange(gen),
                                          mpfr_get_ld(newEngine.mass, MPFR_RNDN),mpfr_get_ld(newEngine.exhaustVelocity, MPFR_RNDN)));
            ship->addStage(stageList.at(i)->dryMass, stageList.at(i)->fuelMass, newEngine);
        }


        auto fullTest = [&stageList, &ship, &stageCount](char *name) {
            long double rollingMass = 0;
            auto localStage = --stageList.end();
            auto stageIter = --ship->getStages()->end();
            printf("--------------------# %s --------------------\n", name);
            for (uint i = stageList.size() - 1; i != 0; i--) {
                printf("--------------------## stage %d--------------------\n", i);
                doubleTest((*stageIter)->dryMass, (*localStage)->dryMass, (char *) "Dry Mass");
                doubleTest((*stageIter)->fuelMass, (*localStage)->fuelMass, (char *) "Fuel Mass");
                doubleTest((*stageIter)->engine.mass, (*localStage)->engineMass, (char *) "Engine Mass");
                doubleTest((*stageIter)->engine.exhaustVelocity, (*localStage)->exhaustVelocity,
                           (char *) "Exhaust Velocity");
                doubleTest((*stageIter)->totalMass,
                           (*localStage)->dryMass + (*localStage)->fuelMass + (*localStage)->engineMass,
                           (char *) "Total Mass");

                rollingMass += (*localStage)->dryMass + (*localStage)->fuelMass + (*localStage)->engineMass;

                mpfr_t remainingMass;
                mpfr_init2(remainingMass, 1024);
                ship->getRemainingMass(remainingMass, i);
                doubleTest(remainingMass, rollingMass, (char *) "Remaining Mass");
                mpfr_clear(remainingMass);

                printf("ln(%Lf/(%Lf - %Lf)) * %Lf | %Lf : %Lf \n", rollingMass, rollingMass, (*localStage)->fuelMass,
                       (*localStage)->exhaustVelocity, mpfr_get_ld((*stageIter)->deltaV, MPFR_RNDN),
                        genDeltaV(rollingMass, (*localStage)->fuelMass, (*localStage)->exhaustVelocity));
                doubleTest((*stageIter)->deltaV,
                           genDeltaV(rollingMass, (*localStage)->fuelMass, (*localStage)->exhaustVelocity),
                           (char *) "Delta V");
                stageIter--;
                localStage--;
            }
        };

        //SECTION ("No Changes to Stages") {
        //    fullTest((char *) "No Changes to Stages");
        //}

        //SECTION("Changes to Stages") {
            std::uniform_int_distribution<int> modifyValChanceRange(1, 256);
            std::uniform_int_distribution<int> modifyValChance(-modifyValChanceRange(gen), modifyValChanceRange(gen));
            auto localStage = --stageList.end();
            auto stageIter = --ship->getStages()->end();
            for (uint i = stageList.size() - 1; i != 0; i--) {

                if (0 < modifyValChance(gen)) {
                    (*localStage)->dryMass = valRange(gen);
                    ship->setStageDryMass(*stageIter, (*localStage)->dryMass);
                }
                if (0 < modifyValChance(gen)) {
                    (*localStage)->fuelMass = valRange(gen);
                    ship->setStageFuelMass(*stageIter, (*localStage)->fuelMass);
                }
                if (0 < modifyValChance(gen)) {
                    Engine newEngine;
                    mpfr_set_ld(newEngine.mass, valRange(gen), MPFR_RNDN);
                    mpfr_set_ld(newEngine.exhaustVelocity, valRange(gen), MPFR_RNDN);
                    const char* name = ("S" + std::to_string(i)).c_str();
                    newEngine.name = new char[strlen(name) + 1];
                    std::strcpy(newEngine.name, name);
                    ship->setStageEngine(*stageIter, newEngine);

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
            delete (ship);
        //}
    mpfr_free_cache();
    }
}