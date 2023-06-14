//
// Created by user on 6/11/23.
//

#ifndef IRA_ENGINELIST_H
#define IRA_ENGINELIST_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QScrollArea>
#include "SpaceShipHandler.h"
#include "lib.h"

class EngineList : public QWidget {
Q_OBJECT

public:
    explicit EngineList(SpaceShipHandler* handler, QWidget *parent = nullptr) : QWidget(parent) {
        this->handler = handler;

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(new HeaderWidget("Engine List", this));

    }

public slots:
    void update();

private:
    SpaceShipHandler* handler;
    QScrollArea *scrollArea;
};


#endif //IRA_ENGINELIST_H
