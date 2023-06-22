#ifndef IRA_LIB_H
#define IRA_LIB_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "Handler.h"

class HeaderWidget : public QWidget {
public:
    HeaderWidget(const QString& text, QWidget *parent = nullptr) : QWidget(parent) {
        // Create a QLabel to display the header text
        QLabel *headerLabel = new QLabel(text, this);
        headerLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

        // Set up the layout
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(headerLabel);
        layout->setContentsMargins(10, 10, 10, 10); // Set margins if needed
    }
};
#endif //IRA_LIB_H
