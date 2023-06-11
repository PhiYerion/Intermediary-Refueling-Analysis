#ifndef IRA_STAGECREATOR_H
#define IRA_STAGECREATOR_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include "SpaceShipHandler.h"
#include "lib.h"

class StageCreator : public QWidget {
    Q_OBJECT

public:
    explicit StageCreator(std::string shipName, int stage, QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout* layout = new QVBoxLayout(this);

        // Header
        HeaderWidget* headerWidget = new HeaderWidget(QString::fromStdString(shipName + " Stage " + std::to_string(stage)), this);

        // Create a grid for the form
        QGridLayout *grid = new QGridLayout();

        // Create the input boxes and labels
        QLabel *label1 = new QLabel("Fuel Mass:", this);
        QLabel *label2 = new QLabel("Dry Mass:", this);
        QLabel *label3 = new QLabel("Engine:", this);

        input1_ = new QLineEdit(this);
        input2_ = new QLineEdit(this);
        input3_ = new QComboBox(this);

        // Create the "Enter" button
        QPushButton *enterButton = new QPushButton("Enter", this);

        // Connect the button's clicked signal to a slot
        QObject::connect(enterButton, &QPushButton::clicked, this, &StageCreator::handleEnterClicked);

        // add header
        layout->addWidget(headerWidget);

        // Add the labels, input boxes, and button to the grid
        layout->addLayout(grid);

        grid->addWidget(label1, 0, 0);
        grid->addWidget(input1_, 0, 1);
        grid->addWidget(label2, 1, 0);
        grid->addWidget(input2_, 1, 1);
        grid->addWidget(label3, 2, 0);
        grid->addWidget(input3_, 2, 1);
        grid->addWidget(enterButton, 3, 0, 1, 2);
    }

    signals:
            void stageFormSubmitted(double a, double b, std::string c);

private slots:
    void handleEnterClicked();

private:
    QLineEdit* input1_;
    QLineEdit* input2_;
    QComboBox* input3_;
};


#endif //IRA_STAGECREATOR_H
