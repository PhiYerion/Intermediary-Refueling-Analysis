//
// Created by user on 6/11/23.
//

#include "EngineCreator.h"

void EngineCreator::handleEnterClicked() {
    // Convert the text to numbers
    bool ok;
    auto name = input1_->text().toStdString();

    double mass, exhaustVelocity;
    mass = input2_->text().toDouble(&ok);
    ok ? exhaustVelocity = input3_->text().toDouble(&ok) : 0;


    // Check if the conversion was successful
    if (ok) {
        handler->createEngine(name, mass, exhaustVelocity);

        // Emit the numbersEntered signal
        emit engineFormSubmitted(name, mass, exhaustVelocity);
    } else {
        // Handle invalid input
        qDebug() << "Invalid input. Please enter numbers.";
    }
}