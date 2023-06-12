#include <QApplication>
#include "gui/Window.h"
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the main window
    QWidget window;

    window.setGeometry(100, 100, 800, 600);

    // Create the form widget
    ShipWidget *formWidget = new ShipWidget(&window);

    // Set up the layout for the main window
    QVBoxLayout *layout = new QVBoxLayout(&window);
    layout->addWidget(formWidget);

    // Show the main window
    window.show();

    return app.exec();
}
