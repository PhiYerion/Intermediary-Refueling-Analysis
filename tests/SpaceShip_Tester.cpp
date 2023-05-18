#include <random>
#include <vector>
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include "../SpaceShip.h"

void doubleTest(long double a, long double b, char* name) {
    printf("%20s variance: %15e\n", name, (a - b) / a);
    CHECK_THAT(a, Catch::Matchers::WithinRel(b, .00001));
};

TEST_CASE("getRemainingMass") {

}

TEST_CASE("SpaceShip") {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<long double> valRange(1, 100'000.0);
    std::uniform_int_distribution<uint> stageRange(10, 30);

    struct stage {
        long double dryMass, fuelMass, engineMass, exhaustVelocity;

        stage(long double dryMass, long double fuelMass, long double engineMass, long double exhaustVelocity) :
                dryMass(dryMass), fuelMass(fuelMass), engineMass(engineMass), exhaustVelocity(exhaustVelocity) {}
    };
    std::vector<stage> stageList;

    auto* ship = new SpaceShip();
    const uint stageCount = stageRange(gen);
    for (uint i = 0; i < stageCount; i++) {
        stageList.push_back(*(new stage (valRange(gen), valRange(gen), valRange(gen), valRange(gen))));
                                                                     stageList.at(i).exhaustVelocity, i;
        ship->addStage(stageList.at(i).dryMass, stageList.at(i).fuelMass, {stageList.at(i).engineMass,
                                                                           stageList.at(i).exhaustVelocity, ("S" + std::to_string(i)).c_str()});
    }



    auto fullTest = [&stageList, &ship, &stageCount](char* name) {
        long double rollingMass = 0;
        auto localStage = --stageList.end();
        auto stageIter = --ship->getStages().end();
        printf("--------------------# %s --------------------\n", name);
        for (uint i = stageList.size() - 1 ; i != 0; i--) {
            printf("--------------------## stage %d--------------------\n", i);
            doubleTest(stageIter->dryMass, localStage->dryMass, (char *) "Dry Mass");
            doubleTest(stageIter->fuelMass, localStage->fuelMass, (char *) "Fuel Mass");
            doubleTest(stageIter->engine.mass, localStage->engineMass, (char *) "Engine Mass");
            doubleTest(stageIter->engine.exhaustVelocity, localStage->exhaustVelocity, (char *) "Exhaust Velocity");
            doubleTest(stageIter->totalMass, localStage->dryMass + localStage->fuelMass + localStage->engineMass,(char *) "Total Mass");

            rollingMass += stageIter->totalMass;
            doubleTest(ship->getRemainingMass(i), rollingMass, (char *) "Remaining Mass");

            printf("DBG-1 %f * log(%f / (%f - %f))\n", localStage->exhaustVelocity, rollingMass, rollingMass, localStage->fuelMass);
            printf("DBG-2 %f\n", stageIter->deltaV);
            doubleTest(stageIter->deltaV,
                       localStage->exhaustVelocity * log(rollingMass / (rollingMass - localStage->fuelMass)),
                       (char *) "Delta V");
            stageIter--;
            localStage--;
        }
    };

    SECTION ("No Changes to Stages") {
        fullTest((char*)"No Changes to Stages");
    }

    SECTION("Changes to Stages") {
        std::uniform_int_distribution<int> modifyValChanceRange(1, 256);
        std::uniform_int_distribution<int> modifyValChance(-modifyValChanceRange(gen), modifyValChanceRange(gen));
        auto localStage = --stageList.end();
        auto stageIter = --ship->getStages().end();
        for (uint i = stageList.size() - 1; i != 0; i--) {
            if (0 < modifyValChance(gen)) {
                localStage->dryMass = valRange(gen);
                ship->setStageDryMass(&(*stageIter), localStage->dryMass);
            }
            /*if (0 < modifyValChance(gen)) {
                localStage->fuelMass = valRange(gen);
                ship->setStageFuelMass(&(*stageIter), localStage->dryMass);
            }
            if (0 < modifyValChance(gen)) {
                localStage->engineMass = valRange(gen);
                localStage->exhaustVelocity = valRange(gen);
                ship->setStageEngine(&(*stageIter), {localStage->engineMass, localStage->exhaustVelocity, ("S" + std::to_string(i)).c_str()});
            }*/
            --localStage;
            --stageIter;
        }
        fullTest((char*)"Modified");
    }

}
