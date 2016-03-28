#include "maincontroller.h"

MainController::MainController(QObject *parent) : QObject(parent)
{
    mainWindow = new MainWindow();
    loginWidget = new LoginWidget();
    groups = new GroupWidget();

    client = new ClientCore(this);

    connect(loginWidget, SIGNAL(login(QString,QString,QString)), client, SLOT(login(QString,QString,QString)));

    connect(client, SIGNAL(successLogin()), this, SLOT(onSuccessLogin()));
    connect(client, SIGNAL(unSuccessLogin()), loginWidget, SLOT(onUnSuccessLogin()));

    connect(client, SIGNAL(addHall(uint,QString,QString,int,QString)), mainWindow, SLOT(addHall(uint,QString,QString,int,QString)));
    connect(client, SIGNAL(addItem(int,QString,QString,int,QString,int)), mainWindow, SLOT(addItem(int,QString,QString,int,QString,int)));

    connect(mainWindow, SIGNAL(openGroupMenu()), this, SLOT(openGroupMenu()));

    connect(this, SIGNAL(needGroups()), client, SLOT(ifNeedForGroups()));
    connect(client, SIGNAL(addGroup(int,QString,QString)), groups, SLOT(addGroup(int,QString,QString)));

    connect(client, SIGNAL(groupAd(bool)), groups, SLOT(dataAD(bool)));

    connect(groups, SIGNAL(onModClicked(int,QString,QString)), client, SLOT(editGroup(int,QString,QString)));
    connect(client, SIGNAL(groupClear()),groups, SLOT(clearGroups()));

    connect(groups, SIGNAL(onAddClicked(QString,QString)), client, SLOT(insertGroup(QString,QString)));

    connect(groups, SIGNAL(onDelClicked(int)), client, SLOT(deleteGroup(int)));

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

void MainController::openGroupMenu()
{
    //mainWindow->setEnabled(false);
    groups->close();
    groups->dataAD(false);
    groups->clearGroups();
    groups->show();
    emit this->needGroups();
}



