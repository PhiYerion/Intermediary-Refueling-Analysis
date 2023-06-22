#ifndef IRA_STAGECREATOR_H
#define IRA_STAGECREATOR_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include "Handler.h"
#include "lib.h"
#include "ShipList.h"

class StageCreator : public QWidget {
    Q_OBJECT

public:
    explicit StageCreator(Handler* handler, std::string shipName, int stage, ShipList* shipList, QWidget *parent = nullptr) : QWidget(parent) {
        this->handler = handler;
        this->shipName = shipName;
        this->stage = stage;

        QVBoxLayout* layout = new QVBoxLayout(this);

        // Header
        HeaderWidget* headerWidget = new HeaderWidget(QString::fromStdString(shipName + " Stage " + std::to_string(stage)));
        layout->addWidget(headerWidget);

        // Create a grid for the form
        QGridLayout *grid = new QGridLayout();

        // Create the input boxes and labels
        QLabel *label1 = new QLabel("Fuel Mass:");
        QLabel *label2 = new QLabel("Dry Mass:");
        QLabel *label3 = new QLabel("Engine:");

        input1_ = new QLineEdit();
        input2_ = new QLineEdit();

        input3_ = new QComboBox();
        for (auto& engine : *handler->getEngineList()) {
            input3_->addItem(QString::fromStdString(engine.first));
        }

        // Create the "Enter" button
        QPushButton *enterButton = new QPushButton("Enter");

        // Connect the button's clicked signal to a slot
        QObject::connect(enterButton, &QPushButton::clicked, this, &StageCreator::handleEnterClicked);

        // Add the labels, input boxes, and button to the grid
        grid->addWidget(label1, 0, 0);
        grid->addWidget(input1_, 0, 1);
        grid->addWidget(label2, 1, 0);
        grid->addWidget(input2_, 1, 1);
        grid->addWidget(label3, 2, 0);
        grid->addWidget(input3_, 2, 1);
        grid->addWidget(enterButton, 3, 0, 1, 2);

        layout->addLayout(grid);

        QWidget* gridWidget = new QWidget();
        gridWidget->setLayout(grid);
        layout->addWidget(gridWidget);

        QObject::connect(this, &StageCreator::stageFormSubmitted, shipList, &ShipList::update);
    }

    signals:
        void stageFormSubmitted(double a, double b, std::string c);

private slots:
    void handleEnterClicked();

private:
    Handler* handler;
    std::string shipName;
    int stage;

    QLineEdit* input1_;
    QLineEdit* input2_;
    QComboBox* input3_;
};


#endif //IRA_STAGECREATOR_H
