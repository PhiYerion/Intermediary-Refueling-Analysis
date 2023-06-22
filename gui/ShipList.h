//
// Created by user on 6/12/23.
//

#ifndef IRA_SHIPLIST_H
#define IRA_SHIPLIST_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QScrollArea>
#include "Handler.h"
#include "lib.h"

class ShipList : public QWidget {
Q_OBJECT

public:
    explicit ShipList(Handler* handler, QWidget *parent = nullptr) : QWidget(parent) {
        this->handler = handler;

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(new HeaderWidget("Ship List", this));

    }

public slots:
    void update();

private:
    Handler* handler;
    QScrollArea *scrollArea;
};

#endif //IRA_SHIPLIST_H
