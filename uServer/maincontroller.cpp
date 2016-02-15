#include "maincontroller.h"

MainController::MainController(QObject *parent) : QObject(parent)
{
    mainWindow = new MainWindow();
    server = new ServerCore();
}
MainController::~MainController()
{
    delete mainWindow;
}

void MainController::showInterface()
{
    mainWindow->show();
}

