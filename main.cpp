#include <vector>
#include <iostream>
#include <valarray>

struct Engine {
    double mass, exhaustVelocity;
    const char* name;
};
struct Stage {
    double dryMass,                                 // does not include engine mass
    fuelMass;
    Engine engine;
    double deltaV;
};

class SpaceShip {
public:
    SpaceShip() {
        mass = 0;
        deltaV = 0;
        stages = {};
    }

    std::vector<Stage> getStages () {
        return stages;
    }
    Engine getEngine (Stage* stage) {
        stage -> engine;
    }
    double getStageDryMass (Stage* stage) {
        return stage -> dryMass;
    }
    double getStageFuelMass (Stage* stage) {
        return stage -> fuelMass;
    }
    double getStageDeltaV (Stage* stage) {
        return stage -> deltaV;
    }
    double getMass () {
        return mass;
    }
    double getDeltaV () {
        return deltaV;
    }
    void setStageDryMass (Stage* stage, const double newMass) {
        mass += newMass - stage -> dryMass;
        stage -> dryMass = newMass;
        genDeltaV(stage);
    }
    void setStageFuelMass (Stage* stage, const double newMass) {
        mass += newMass - stage -> fuelMass;
        stage -> fuelMass = newMass;
        genDeltaV(stage);
    }
    void setStageEngine (Stage* stage, const Engine newEngine) {
        mass += newEngine.mass - stage -> engine.mass;
        stage -> engine = newEngine;
        genDeltaV(stage);
    }

    void addStage (const double dryMass, const double fuelMass, const Engine engine, const int index = -1) {
        if (index != -1) {
            stages.insert(stages.begin() + index, {dryMass, fuelMass, engine, 0});
        } else {
            stages.push_back({dryMass, fuelMass, engine, 0});
        }
        mass += dryMass + fuelMass + engine.mass;
        genDeltaV();
    }

protected:
    std::vector<Stage> stages;
    double mass;
    double deltaV;

    double getRemainingMass (const Stage& inputStage) {
        double remainingMass = mass;
        for (const auto& stage : stages) {
            if (&stage == &inputStage) {
                return remainingMass;
            }
            remainingMass -= stage.dryMass + stage.engine.mass + stage.fuelMass;
        }
        return remainingMass;
    }

    void genDeltaV (Stage* inpStage = nullptr) {       // for addStage, this should implement only calcs on stages before new
        if (inpStage) {
            double remainingMass = getRemainingMass(*inpStage);
            inpStage->deltaV = inpStage->engine.exhaustVelocity * log(remainingMass / (remainingMass - inpStage->fuelMass));
        } else {
            double remainingMass = mass;
            deltaV = 0;
            for (auto &stage: stages) {
                stage.deltaV = stage.engine.exhaustVelocity * log(mass / (remainingMass - stage.fuelMass));
                deltaV += stage.deltaV;
                remainingMass -= stage.dryMass + stage.fuelMass + stage.engine.mass;
            }
        }
    }


};

class Tanker : SpaceShip {
public:
    Tanker () {
        mass = 0;
        deltaV = 0;
        stages = {};
    }
};


int main () {
    auto* ship = new SpaceShip();
    ship->addStage(100, 1, {100, 100, "test"});
    printf("DeltaV: %f\n", ship->getDeltaV());
    printf("Mass: %f\n", ship->getMass());
    uint i = 0;
    for (const auto &stage : ship->getStages()) {
        printf("Stage %u:\n", i);
        printf("\tDryMass: %fkg\n", stage.dryMass);
        printf("\tFuelMass: %fkg\n", stage.fuelMass);
        printf("\tEngine %s:\n", stage.engine.name);
        printf("\t\tMass: %fkg\n", stage.engine.mass);
        printf("\t\tExhaustVelocity: %fm/s\n", stage.engine.exhaustVelocity);
        printf("\tDeltaV: %fm/s\n", stage.deltaV);
    }
    free(ship);
    return 1;
}