//
// Created by user on 6/12/23.
//

#include "ShipList.h"

void ShipList::update(double a, double b, std::string c) {
    layout()->removeWidget(scrollArea);

    scrollArea = new QScrollArea();
    QWidget *scrollContent = new QWidget();
    QGridLayout* list = new QGridLayout();

    for (auto shipPair : *handler->getShipList()) {
        auto shipName = shipPair.first;
        auto ship = shipPair.second;
        list->addWidget(new QLabel(shipName.c_str()));
        list->addWidget(new QLabel((
                " - Mass: " + std::to_string(ship->getMass()) + "\n"
            ).c_str()));

        for (int i = 0; i < ship->getStages()->size(); i++) {
            // Find Engine:
            const Engine* engine = ship->getStageEngine(i);

            list->addWidget(new QLabel((" - Stage " + std::to_string(i)).c_str()));
            list->addWidget(new QLabel((
                " - - Remaining Mass: " + std::to_string(ship->getRemainingMass(i)) + "\n" +
                " - - Total Mass: " + std::to_string(ship->getStageTotalMass(i)) + "\n" +
                " - - Dry Mass: " + std::to_string(ship->getStageDryMass(i)) + "\n" +
                " - - Fuel Mass: " + std::to_string(ship->getStageFuelMass(i)) + "\n" +
                " - - Delta V: " + std::to_string(ship->getStageDeltaV(i)) + "\n" +
                " - - Engine: " + engine->name + "\n" +
                " - - - Mass: " + std::to_string(handler->getEngineMass(engine->name)) + "\n" +
                " - - - Exhaust Velocity: " + std::to_string(handler->getEngineExhaustVelocity(engine->name)) + "\n"
            ).c_str()));
        }
    }
    scrollContent->setLayout(list);
    scrollArea->setWidget(scrollContent);
    layout()->addWidget(scrollArea);
}