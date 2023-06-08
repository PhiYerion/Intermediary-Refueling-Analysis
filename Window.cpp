#include <QMainWindow>
#include <QStyle>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "SpaceShipHandler.h"

class MyWindow : public QMainWindow
{
public:
    MyWindow (SpaceShipHandler* handler, QWidget *parent = nullptr) {
        // Set window title
        setWindowTitle("My Window");

        // Set window size
        resize(800, 600);

        // Center the window on the screen
        setGeometry(
                QStyle::alignedRect(
                        Qt::LeftToRight,
                        Qt::AlignCenter,
                        size(),
                        qApp->desktop()->availableGeometry()
                )
        );

        QTableWidget* tableWidget = new QTableWidget(this);

        int rowCount = 3;
        int columnCount = 2;
        tableWidget->setRowCount(rowCount);
        tableWidget->setColumnCount(columnCount);

        // Set the table headers
/*        QStringList headers;
        tableWidget->setHorizontalHeaderLabels(headers);*/

        // Populate the table with data
        for (int row = 0; row < rowCount; ++row) {
            for (int col = 0; col < columnCount; ++col) {
                QTableWidgetItem *item = new QTableWidgetItem(QString("Cell %1-%2").arg(row + 1).arg(col + 1));
                tableWidget->setItem(row, col, item);
            }
        }

        setCentralWidget(tableWidget);

    }
    ~MyWindow() {

    }
};
