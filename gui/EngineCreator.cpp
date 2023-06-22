//
// Created by user on 6/11/23.
//

#include "EngineCreator.h"
#include <cctype>

void EngineCreator::handleEnterClicked() {
    auto name = input1_->text().toStdString();
    const auto& mass = input2_->text().trimmed().toStdString();
    const auto& exhaustVelocity = input3_->text().trimmed().toStdString();

    try {
        size_t pos;
        std::stod(mass + exhaustVelocity, &pos);
        if (pos < mass.length() + exhaustVelocity.length()) {
            throw std::invalid_argument("Invalid input. Please enter numbers.");
        }
    } catch (std::invalid_argument& e) {
        qDebug() << "Invalid input. Please enter numbers.";
        return;
    }
    handler->createEngine(name, mass, exhaustVelocity);

    // Emit the numbersEntered signal
    emit engineFormSubmitted();
}