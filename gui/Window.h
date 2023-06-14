
#ifndef IRA_WINDOW_H
#define IRA_WINDOW_H

#include <QWidget>
#include <QPainter>
#include "SpaceShipHandler.h"
#include "ShipCreator.h"
#include "EngineCreator.h"
#include "EngineList.h"
#include "lib.h"
#include "ShipList.h"

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


CustomFrame* customBox(QWidget* content, QWidget* parent = nullptr) {
    CustomFrame* frame = new CustomFrame;
    frame->setFrameShape(QFrame::Box);
    frame->setLineWidth(1);

    QVBoxLayout* frameLayout = new QVBoxLayout(frame);
    frameLayout->addWidget(content);
    return frame;
}

class ShipWidget : public QWidget {
public:
    ShipWidget(QWidget *parent = nullptr) : QWidget(parent) {
        SpaceShipHandler* handler = new SpaceShipHandler(1024);

        QHBoxLayout* hLayout = new QHBoxLayout(this);

        // Create SpaceShip list
        ShipList* shipList = new ShipList(handler);
        hLayout->addWidget(shipList);

        // Create the header widget
        HeaderWidget* headerWidget = new HeaderWidget("Creator");

        // Create Engine input form
        EngineCreator* engineForm = new EngineCreator(handler);
        CustomFrame* engineFrame = customBox(engineForm, this);


        // Create the input form
        ShipInitForm* inputForm = new ShipInitForm(shipList);
        CustomFrame* inputFrame = customBox(inputForm, this);

        // Create the number display widget
        ShipDisplay* displayWidget = new ShipDisplay(handler, shipList);
        CustomFrame* displayFrame = customBox(displayWidget, this);

        // Connect the numbersEntered signal to the setNumbers slot
        QObject::connect(inputForm, &ShipInitForm::formSubmitted, displayWidget, &ShipDisplay::addStageForms);

        // Set up the layout for the main window
        QVBoxLayout *vLayout = new QVBoxLayout();
        vLayout->addWidget(headerWidget);
        vLayout->addWidget(engineFrame);
        vLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred));
        vLayout->addWidget(inputFrame);
        vLayout->addWidget(displayFrame);

        EngineList* list = new EngineList(handler);
        QObject::connect(engineForm, &EngineCreator::engineFormSubmitted, list, &EngineList::update);
        hLayout->addLayout(vLayout);
        hLayout->addWidget(list);


        // Set the layout for the widget
        setLayout(hLayout);
    }
};

#endif //IRA_WINDOW_H