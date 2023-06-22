#include <vector>
#include <iostream>
#include <random>
#include <cstdlib>
#include "Handler.h"
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
    { // TEST SPACESHIP 1 - Setting values:
        Handler handler(1024);
        auto ship = handler.addShip("Ship 1");
        handler.createEngine("1.1", "100000.456153", "456123.15687498453");
        ship->addStage("1561.1654893512", "4561312.8564854312", handler.getEngine("1.1"));

        ship->setStageDryMass(0, "46381.56737479119504996560863219201565");
        ship->setStageFuelMass(0, "3552.14007770645868733438987874251325");
        handler.createEngine("1.2", "64958.37813684586060647419003544200678", "3617.40857668100880828454535276250681");
        ship->setStageEngine(0, handler.getEngine("1.2"));

        mpfr_t deltaV;
        ship->getRawDeltaV(deltaV);
        // Result generated by keisan high precision calculator. Most calculators are around this same value.
        // e-15 deviation is due to conversions.
        printf("Ship 1 DeltaV: %Lf\n", mpfr_get_ld(deltaV, MPFR_RNDN));
        printf("Ship 1 Dry Mass: %Lf\n", ship->getStageDryMass(0));
        doubleTest(deltaV, 113.6054696961617280180871812622258240703610626419, "Ship 1 DeltaV");
        mpfr_clear(deltaV);
    }

    { // TEST SPACESHIP 2 - Random:
        Handler handler(1024);
        auto ship = handler.addShip("Ship 2");
        handler.createEngine("2.1", "52851.89721221082680102654194342903793", "44949670902.83464924991130828857421875000000");
        ship->addStage("52967195.75027480287462822161614894866943", "976209.93331809725617631556815467774868", handler.getEngine("2.1"));

        mpfr_t deltaV;
        ship->getRawDeltaV(deltaV);
        // Result generated by known good IRA configuration, but also checked with calculator.
        doubleTest(deltaV, 820090589.4936481447016862631570112981054490080322, "Ship 2");
        mpfr_clear(deltaV);
    }
}

TEST_CASE("Runtime Errors") {
    Handler handler(1024);
    std::vector<int> stages;

    for (uint i = 0; i < 100; i++) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<long double> valRange(1, 1'000'000'000.0);
        std::uniform_int_distribution<uint> stageRange(10, 30);

        auto* ship = handler.addShip("Ship " + std::to_string(i));
        const uint stageCount = stageRange(gen);
        stages.push_back(stageCount);

        for (uint j = 0; j < stageCount; j++) {
            Engine newEngine;
            mpfr_set_str(newEngine.mass, std::to_string(valRange(gen)).c_str(), 10, MPFR_RNDN);
            mpfr_set_str(newEngine.exhaustVelocity, std::to_string(valRange(gen)).c_str(), 10, MPFR_RNDN);

            std::string name = "S" + std::to_string(i) + "." + std::to_string(j);

            handler.createEngine(name, std::to_string(valRange(gen)), std::to_string(valRange(gen)));
        }

        for (uint j = 0; j < stageCount; j++) {
            Engine newEngine;
            mpfr_set_str(newEngine.mass, std::to_string(valRange(gen)).c_str(), 10, MPFR_RNDN);
            mpfr_set_str(newEngine.exhaustVelocity, std::to_string(valRange(gen)).c_str(), 10, MPFR_RNDN);
            std::string name = "S-infunc" + std::to_string(i) + "."  + std::to_string(j);
            handler.createEngine(name, std::to_string(valRange(gen)), std::to_string(valRange(gen)));

            auto random = stageRange(gen);
            if (random > 23) {
                ship->addStage(std::to_string(valRange(gen)), std::to_string(valRange(gen)), handler.getEngine(name));
            } else if (random > 16) {
                ship->addStage(std::to_string(valRange(gen)), std::to_string(valRange(gen)), handler.getEngine("S" + std::to_string(i) + "." + std::to_string(j)));
            } else {
                std::uniform_int_distribution<uint> iterationMax(0, i);
                auto rndIter = iterationMax(gen);
                std::uniform_int_distribution<uint> stageMax(0, stages[rndIter] - 1);
                auto rndStage = stageMax(gen);
                ship->addStage(std::to_string(valRange(gen)), std::to_string(valRange(gen)),
                               handler.getEngine("S" + std::to_string(rndIter) + "." + std::to_string(rndStage)));

            }
        }

        std::uniform_int_distribution<int> modifyValChanceRange(1, 256);
        std::uniform_int_distribution<int> modifyValChance(-modifyValChanceRange(gen), modifyValChanceRange(gen));
        for (uint j = 0; j < stageCount; j++) {
            if (0 < modifyValChance(gen)) {
                ship->setStageDryMass(j, std::to_string(valRange(gen)));
            }
            if (0 < modifyValChance(gen)) {
                ship->setStageFuelMass(j, std::to_string(valRange(gen)));
            }
            if (0 < modifyValChance(gen)) {
                // this will intentionally occasionally conflict with other engines
                auto name = "S-new" + std::to_string(i) + std::to_string(j);

                handler.createEngine(name, std::to_string(valRange(gen)), std::to_string(valRange(gen)));
                ship->setStageEngine(j, handler.getEngine(name));

            }
        }


        std::uniform_int_distribution<uint> insertStageAmt(1, 100);
        for (uint j = 0; j < insertStageAmt(gen); j++) {
            std::uniform_int_distribution<uint> insertStagePos(0, ship->getStages()->size() - 1);

            std::uniform_int_distribution<uint> iterationMax(0, i);
            auto rndIter = iterationMax(gen);
            std::uniform_int_distribution<uint> stageMax(0, stages[rndIter] - 1);
            auto rndStage = stageMax(gen);
            ship->addStage(std::to_string(valRange(gen)), std::to_string(valRange(gen)),
                              handler.getEngine("S" + std::to_string(rndIter) + "." + std::to_string(rndStage)),
                           insertStagePos(gen));
        }
    }

    for (const auto& shipPair : *handler.getShipList()) {
        shipPair.second->printStats();
    }

}

long double genDeltaV(long double remainingMass, long double fuelMass, long double exhaustVelocity) {
    //return log((remainingMass - fuelMass) / remainingMass) * exhaustVelocity;
    mpfr_prec_t precision = 1024;
    mpfr_set_default_prec(precision);

    mpfr_t result;
    mpfr_init2(result, 1024);
    mpfr_set_str(result, std::to_string(remainingMass).c_str(), 10, MPFR_RNDN);

    mpfr_t denominator;
    mpfr_init2(denominator, 1024);
    mpfr_set_str(denominator, std::to_string(remainingMass).c_str(), 10, MPFR_RNDN);
    mpfr_sub_d(denominator, denominator, fuelMass, MPFR_RNDN);

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

        Handler handler(1024);
        auto* ship = handler.addShip("Ship " + std::to_string(j));
        const uint stageCount = stageRange(gen);
        for (uint i = 0; i < stageCount; i++) {
            Engine newEngine;
            mpfr_set_str(newEngine.mass, std::to_string(valRange(gen)).c_str(), 10, MPFR_RNDN);
            mpfr_set_str(newEngine.exhaustVelocity, std::to_string(valRange(gen)).c_str(), 10, MPFR_RNDN);

            std::string name = "S" + std::to_string(i);

            stageList.push_back(new stage(valRange(gen), valRange(gen),
                                          mpfr_get_ld(newEngine.mass, MPFR_RNDN),mpfr_get_ld(newEngine.exhaustVelocity, MPFR_RNDN)));

            handler.createEngine(name, std::to_string(stageList.at(i)->engineMass), std::to_string(stageList.at(i)->exhaustVelocity));
            ship->addStage(std::to_string(stageList.at(i)->dryMass), std::to_string(stageList.at(i)->fuelMass), handler.getEngine(name));
        }

        std::uniform_int_distribution<int> modifyValChanceRange(1, 256);
        std::uniform_int_distribution<int> modifyValChance(-modifyValChanceRange(gen), modifyValChanceRange(gen));
        auto localStage = --stageList.end();
        auto stageIter = --ship->getStages()->end();
        for (int i = stageList.size() - 1; i >= 0; i--) {

            if (0 < modifyValChance(gen)) {
                (*localStage)->dryMass = valRange(gen);
                ship->setStageDryMass(i, std::to_string((*localStage)->dryMass));
            }
            if (0 < modifyValChance(gen)) {
                (*localStage)->fuelMass = valRange(gen);
                ship->setStageFuelMass(i, std::to_string((*localStage)->fuelMass));
            }
            if (0 < modifyValChance(gen)) {
                Engine newEngine;
                mpfr_set_str(newEngine.mass, std::to_string(valRange(gen)).c_str(), 10, MPFR_RNDN);
                mpfr_set_str(newEngine.exhaustVelocity, std::to_string(valRange(gen)).c_str(), 10, MPFR_RNDN);
                newEngine.name = "S-new." + std::to_string(i);

                (*localStage)->engineMass = mpfr_get_ld(newEngine.mass, MPFR_RNDN);
                (*localStage)->exhaustVelocity = mpfr_get_ld(newEngine.exhaustVelocity, MPFR_RNDN);

                handler.createEngine(newEngine.name, std::to_string((*localStage)->engineMass), std::to_string((*localStage)->exhaustVelocity));
                ship->setStageEngine(i, handler.getEngine(newEngine.name));
            }

            --localStage;
            --stageIter;
        }

        auto fullTest = [&stageList, &ship, &stageCount](char *name) {
            long double rollingMass = 0;
            auto localStage = --stageList.end();
            auto stageIter = --ship->getStages()->end();
            printf("--------------------# %s --------------------\n", name);
            for (uint i = stageList.size() - 1; i != 0; i--) {
                printf("--------------------## stage %d--------------------\n", i);
                printf("Verified: rM: %.20Lf, f: %.20Lf, e.m: %.20Lf, e.eV: %.20Lf, dM: %.20Lf\n",
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

        fullTest((char *) "Modified");

        for (auto stage: stageList) {
            delete (stage);
        }
//}
        mpfr_free_cache();
    }
}