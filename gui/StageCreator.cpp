#include "StageCreator.h"

void StageCreator::handleEnterClicked() {
    std::string engine = input3_->currentText().toStdString();

    bool ok;
    double fuelMass, dryMass;
    fuelMass = input1_->text().toDouble(&ok);
    ok ? dryMass = input2_->text().toDouble(&ok) : 0;

    // Check if the conversion was successful
    if (ok) {
        // Emit the numbersEntered signal
        emit stageFormSubmitted(fuelMass, dryMass, engine);
    } else {
        // Handle invalid input
        qDebug() << "Invalid input. Please enter numbers.";
    }
}