#include "StageCreator.h"

void StageCreator::handleEnterClicked() {
    std::string engine = input3_->currentText().toStdString();

    const auto& fuelMass = input1_->text().trimmed().toStdString();
    const auto& dryMass = input2_->text().trimmed().toStdString();

    try {
        size_t pos;
        std::stod(fuelMass + dryMass, &pos);
        if (pos < fuelMass.length() + dryMass.length()) {
            throw std::invalid_argument("Invalid input. Please enter numbers.");
        }
    } catch (std::invalid_argument& e) {
        qDebug() << "Invalid input. Please enter numbers.";
        return;
    }

    // Check if the conversion was successful
    handler->getShip(shipName)->setStage(dryMass, fuelMass, handler->getEngine(engine), stage);

    emit stageFormSubmitted();
}