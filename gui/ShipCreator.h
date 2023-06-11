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

class ShipInitForm : public QWidget {
Q_OBJECT

public:
    explicit ShipInitForm(QWidget *parent = nullptr) : QWidget(parent) {
        // Create a grid layout for the form
        QGridLayout *layout = new QGridLayout(this);

        // Create the input boxes and labels
        QLabel *label1 = new QLabel("Name:", this);
        QLabel *label2 = new QLabel("Stages:", this);

        input1_ = new QLineEdit(this);
        input2_ = new QLineEdit(this);

        // Create the "Enter" button
        QPushButton *enterButton = new QPushButton("Enter", this);

        // Connect the button's clicked signal to a slot
        QObject::connect(enterButton, &QPushButton::clicked, this, &ShipInitForm::handleEnterClicked);

        // Add the labels, input boxes, and button to the layout
        layout->addWidget(label1, 0, 0);
        layout->addWidget(input1_, 0, 1);
        layout->addWidget(label2, 1, 0);
        layout->addWidget(input2_, 1, 1);
        layout->addWidget(enterButton, 2, 0, 1, 2);
    }

signals:
    void formSubmitted(std::string a, double b);

private slots:
    void handleEnterClicked();

private:
    QLineEdit *input1_;
    QLineEdit *input2_;
};

class ShipDisplay : public QWidget {
public:
    ShipDisplay(QWidget *parent = nullptr) : QWidget(parent) {
        // Create a label to display the numbers
        label_ = new QLabel(this);
        label_->setText("Numbers will be displayed here.");

        // Set up the layout
        layout = new QVBoxLayout(this);
        layout->addWidget(label_);
    }

    void setNumbers(std::string name, double stages);

private:
    QLabel *label_;
    QVBoxLayout *layout;
};

#endif //IRA_SHIPCREATOR_H
