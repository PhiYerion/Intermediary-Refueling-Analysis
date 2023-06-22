//
// Created by user on 6/11/23.
//

#ifndef IRA_ENGINECREATOR_H
#define IRA_ENGINECREATOR_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include "Handler.h"
#include "lib.h"

class EngineCreator : public QWidget {
    Q_OBJECT

public:
    explicit EngineCreator(Handler* handler, QWidget *parent = nullptr) : QWidget(parent) {
        this->handler = handler;

        // Add Widget for Engine Creator
        QVBoxLayout* layout = new QVBoxLayout(this);

        // Create a grid for the form
        QGridLayout *grid = new QGridLayout();

        // Header
        HeaderWidget* headerWidget = new HeaderWidget("New Engine", this);
        layout->addWidget(headerWidget);

        // Create the input boxes and labels
        QLabel *label1 = new QLabel("Name:");
        QLabel *label2 = new QLabel("Mass:");
        QLabel *label3 = new QLabel("Exhaust Velocity:");

        input1_ = new QLineEdit();
        input2_ = new QLineEdit();
        input3_ = new QLineEdit();

        // Create the "Enter" button
        QPushButton *enterButton = new QPushButton("Enter");

        // Connect the button's clicked signal to a slot
        QObject::connect(enterButton, &QPushButton::clicked, this, &EngineCreator::handleEnterClicked);

        // Add the labels, input boxes, and button to the grid
        grid->addWidget(label1, 0, 0);
        grid->addWidget(input1_, 0, 1);
        grid->addWidget(label2, 1, 0);
        grid->addWidget(input2_, 1, 1);
        grid->addWidget(label3, 2, 0);
        grid->addWidget(input3_, 2, 1);
        grid->addWidget(enterButton, 3, 0, 1, 2);
        QWidget* gridWidget = new QWidget();
        gridWidget->setLayout(grid);
        layout->addWidget(gridWidget);
    }

    signals:
            void engineFormSubmitted(std::string a, double b, double c);

private slots:
            void handleEnterClicked();

private:
    Handler* handler;

    QLineEdit *input1_;
    QLineEdit *input2_;
    QLineEdit *input3_;
};


#endif //IRA_ENGINECREATOR_H
