#include <vector>
#include <iostream>
#include <random>
#include <cstdlib>
#include "SpaceShipHandler.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QtWidgets>
#include <QtCore>
#include <QtGui>
#include "Window.cpp"

int main(int argc, char *argv[]) {
    SpaceShipHandler handler(1024);
    QApplication a(argc, argv);

    MyWindow w(&handler);
    w.show();

    return a.exec();
}