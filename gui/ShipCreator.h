#ifndef IRA_SHIPCREATOR_H
#define IRA_SHIPCREATOR_H


#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include "lib.h"
#include "StageCreator.h"
#include "ShipList.h"

class ShipInitForm : public QWidget {
Q_OBJECT

public:
    explicit ShipInitForm(ShipList* shipList, QWidget *parent = nullptr) : QWidget(parent) {
        this->shipList = shipList;

        // Apply stylesheets to the widget
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


        // Add BoxLayout for Engine Creator
        QVBoxLayout* layout = new QVBoxLayout(this);

        // Create a grid for the form
        QGridLayout *grid = new QGridLayout();

        // Header
        HeaderWidget* headerWidget = new HeaderWidget("New Ship");
        layout->addWidget(headerWidget);

        // Create the input boxes and labels
        QLabel *label1 = new QLabel("Name:");
        QLabel *label2 = new QLabel("Stages:");

        input1_ = new QLineEdit();
        input2_ = new QLineEdit();

        // Create the "Enter" button
        QPushButton *enterButton = new QPushButton("Enter");

        // Connect the button's clicked signal to a slot
        QObject::connect(enterButton, &QPushButton::clicked, this, &ShipInitForm::handleEnterClicked);

        // Add the labels, input boxes, and button to the grid
        grid->addWidget(label1, 0, 0);
        grid->addWidget(input1_, 0, 1);
        grid->addWidget(label2, 1, 0);
        grid->addWidget(input2_, 1, 1);
        grid->addWidget(enterButton, 2, 0, 1, 2);

        QWidget* gridWidget = new QWidget();
        gridWidget->setLayout(grid);
        layout->addWidget(gridWidget);
    }

signals:
    void formSubmitted(std::string a, double b, ShipList* shipList);

private slots:
    void handleEnterClicked();

private:
    QLineEdit *input1_;
    QLineEdit *input2_;
    ShipList* shipList;
};

class ShipDisplay : public QWidget {
public:
    ShipDisplay(Handler* handler, ShipList* shipList, QWidget *parent = nullptr) : QWidget(parent) {
        this->handler = handler;

        // Create a label to display the numbers
        label_ = new QLabel(this);
        label_->setText("Stages:");

        // Set up the layout
        layout = new QVBoxLayout(this);
        layout->addWidget(label_);
    }

    void addStageForms(const std::string& name, double stages, ShipList* shipList);

private:
    Handler* handler;
    QLabel *label_;
    QVBoxLayout *layout;
};

#endif //IRA_SHIPCREATOR_H
