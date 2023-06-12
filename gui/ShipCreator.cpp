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

#include <QScrollArea>
void ShipDisplay::addStageForms(std::string name, double stages) {
    handler->addShip(name);

    QScrollArea *scrollArea = new QScrollArea();
    QWidget *scrollContent = new QWidget();
    QGridLayout* stageFormsWidget = new QGridLayout();

    for (int i = 0; i < stages; i++) {
        StageCreator* stageCreator = new StageCreator(handler, name, i);
        stageFormsWidget->addWidget(stageCreator, i/3, i%3);
    }

    scrollContent->setLayout(stageFormsWidget);
    scrollArea->setWidget(scrollContent);
    layout->addWidget(scrollArea);
}
