
#ifndef IRA_WINDOW_H
#define IRA_WINDOW_H

#include <QWidget>
#include "SpaceShipHandler.h"
#include "lib.h"
#include "ShipCreator.h"

class ShipWidget : public QWidget {
public:
    ShipWidget(QWidget *parent = nullptr) : QWidget(parent) {
        // Create the header widget
        HeaderWidget* headerWidget = new HeaderWidget("New Space Ship", this);

        // Create the input form
        ShipInitForm* inputForm = new ShipInitForm(this);

        // Create the number display widget
        ShipDisplay* displayWidget = new ShipDisplay(this);

        // Connect the numbersEntered signal to the setNumbers slot
        QObject::connect(inputForm, &ShipInitForm::formSubmitted, displayWidget, &ShipDisplay::setNumbers);

        // Set up the layout for the main window
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(headerWidget);
        layout->addWidget(inputForm);
        layout->addWidget(displayWidget);

        // Set the layout for the widget
        setLayout(layout);
    }
};

#endif //IRA_WINDOW_H