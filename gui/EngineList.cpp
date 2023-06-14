#include "EngineList.h"

void EngineList::update() {
    layout()->removeWidget(scrollArea);

    scrollArea = new QScrollArea();
    QWidget *scrollContent = new QWidget();
    QGridLayout* list = new QGridLayout();

    for (auto engine : *handler->getEngineList()) {
        list->addWidget(new QLabel(engine.first.c_str()));
        list->addWidget(new QLabel((
                " - Mass: " + std::to_string(handler->getEngineMass(engine.first)) + "\n" +
                " - Exhaust Velocity: " + std::to_string(handler->getEngineExhaustVelocity(engine.first)) + "\n"
                ).c_str()));
    }
    scrollContent->setLayout(list);
    scrollArea->setWidget(scrollContent);
    layout()->addWidget(scrollArea);
}