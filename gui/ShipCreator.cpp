#include "ShipCreator.h"

void ShipInitForm::handleEnterClicked() {
    // Convert the text to numbers
    bool ok1;
    auto name = input1_->text().toStdString();
    auto stages = input2_->text().toDouble(&ok1);

    // Check if the conversion was successful
    if (ok1) {
        // Emit the numbersEntered signal
        emit formSubmitted(name, stages);
    } else {
        // Handle invalid input
        qDebug() << "Invalid input. Please enter numbers.";
    }
}

void ShipDisplay::setNumbers(std::string name, double stages) {
    auto handler = SpaceShipHandler(1024);
    auto ship = handler.addShip(name);
    handler.createEngine("newEngine", 100, 100);

    for (int i = 0; i < stages; i++) {
        StageCreator* stageCreator = new StageCreator(name, i, this);
        layout->addWidget(stageCreator);
    }
}
