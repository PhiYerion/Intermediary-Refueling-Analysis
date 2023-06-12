
#ifndef IRA_WINDOW_H
#define IRA_WINDOW_H

#include <QWidget>
#include <QPainter>
#include "SpaceShipHandler.h"
#include "ShipCreator.h"
#include "EngineCreator.h"
#include "lib.h"

class CustomFrame : public QFrame {
public:
    explicit CustomFrame(QWidget* parent = nullptr) : QFrame(parent) {}

protected:
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::black);
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 10, 10);
    }
};

class ShipWidget : public QWidget {
public:
    ShipWidget(QWidget *parent = nullptr) : QWidget(parent) {
        SpaceShipHandler* handler = new SpaceShipHandler(1024);

        // Create the header widget
        HeaderWidget* headerWidget = new HeaderWidget("Creator");

        // Create Engine input form

        CustomFrame* engineFrame = new CustomFrame;
        engineFrame->setFrameShape(QFrame::Box);
        engineFrame->setLineWidth(1);

        EngineCreator* engineForm = new EngineCreator(handler);
        QVBoxLayout* frameLayout = new QVBoxLayout(engineFrame);
        frameLayout->addWidget(engineForm);


        // Create the input form
        ShipInitForm* inputForm = new ShipInitForm();

        // Create the number display widget
        ShipDisplay* displayWidget = new ShipDisplay(handler);

        // Connect the numbersEntered signal to the setNumbers slot
        QObject::connect(inputForm, &ShipInitForm::formSubmitted, displayWidget, &ShipDisplay::addStageForms);

        // Set up the layout for the main window
        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(headerWidget);
        layout->addWidget(engineFrame);
        layout->addWidget(inputForm);
        layout->addWidget(displayWidget);

        // Set the layout for the widget
        setLayout(layout);
    }
};

#endif //IRA_WINDOW_H