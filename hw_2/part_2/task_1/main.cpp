#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("Моделирование распространения заболевания");
    window.resize(950, 650);
    window.show();

    return app.exec();
}