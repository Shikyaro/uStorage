#include "maincontroller.h"

MainController::MainController(QObject *parent) : QObject(parent)
{
    mainWindow = new MainWindow();
    loginWidget = new LoginWidget();

    client = new ClientCore();

    connect(loginWidget, SIGNAL(login(QString,QString,QString)), client, SLOT(login(QString,QString,QString)));

    connect(client, SIGNAL(successLogin()), this, SLOT(onSuccessLogin()));
    connect(client, SIGNAL(unSuccessLogin()), loginWidget, SLOT(onUnSuccessLogin()));

    connect(client, SIGNAL(addHall(uint,QString,QString,int,QString)), mainWindow, SLOT(addHall(uint,QString,QString,int,QString)));
    connect(client, SIGNAL(addItem(int,QString,QString,int,QString,int)), mainWindow, SLOT(addItem(int,QString,QString,int,QString,int)));
}
MainController::~MainController()
{
    delete mainWindow;
}

void MainController::showInterface()
{
    //mainWindow->show();
    loginWidget->show();
}

void MainController::onSuccessLogin()
{
    mainWindow->show();
    loginWidget->close();
}

