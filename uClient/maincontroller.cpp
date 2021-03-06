#include "maincontroller.h"

MainController::MainController(QObject *parent) : QObject(parent)
{
    mainWindow = new MainWindow();
    loginWidget = new LoginWidget();
    groups = new GroupWidget();
    client = new ClientCore(this);
    users = new UserWidget();

    connect(loginWidget, SIGNAL(login(QString,QString,QString)), client, SLOT(login(QString,QString,QString)));

    connect(client, SIGNAL(successLogin()), this, SLOT(onSuccessLogin()));
    connect(client, SIGNAL(unSuccessLogin()), loginWidget, SLOT(onUnSuccessLogin()));

    connect(client, SIGNAL(addHall(uint,QString,QString,int,QString)), mainWindow, SLOT(addHall(uint,QString,QString,int,QString)));
    connect(client, SIGNAL(addItem(int,QString,QString,QString,QString,int)), mainWindow, SLOT(addItem(int,QString,QString,QString,QString,int)));

    connect(mainWindow, SIGNAL(openGroupMenu()), this, SLOT(openGroupMenu()));

    connect(this, SIGNAL(needGroups()), client, SLOT(ifNeedForGroups()));
    connect(client, SIGNAL(addGroup(int,QString,QString)), groups, SLOT(addGroup(int,QString,QString)));

    connect(client, SIGNAL(groupAd(bool)), groups, SLOT(dataAD(bool)));

    connect(groups, SIGNAL(onModClicked(int,QString,QString)), client, SLOT(editGroup(int,QString,QString)));
    connect(client, SIGNAL(groupClear()),groups, SLOT(clearGroups()));

    connect(groups, SIGNAL(onAddClicked(QString,QString)), client, SLOT(insertGroup(QString,QString)));

    connect(groups, SIGNAL(onDelClicked(int)), client, SLOT(deleteGroup(int)));

    connect(client, SIGNAL(groupsToBox(QList<groupItem*>*)), mainWindow, SLOT(load2box(QList<groupItem*>*)));

    connect(mainWindow, SIGNAL(onAddS(int,QString,QString,QString,QString,int)), client, SLOT(insertItem(int,QString,QString,QString,QString,int)));
    connect(mainWindow, SIGNAL(onEditS(int,QString,QString,QString,QString,int)), client, SLOT(editItem(int,QString,QString,QString,QString,int)));
    connect(mainWindow, SIGNAL(onDeleteS(int)), client, SLOT(deleteItem(int)));

    connect(mainWindow, SIGNAL(tabChSig(int)), client, SLOT(getItemsFromHall(int)));

    connect(client, SIGNAL(updateCurHall(int,QString,QString,int)), mainWindow, SLOT(updHall(int,QString,QString,int)));

    connect(mainWindow, SIGNAL(modHallSig(int,QString,QString,int)), client, SLOT(editHall(int,QString,QString,int)));
    connect(mainWindow, SIGNAL(insHallSig(QString,QString,int)), client, SLOT(insertHall(QString,QString,int)));
    connect(mainWindow, SIGNAL(delHallSig(int)), client, SLOT(delHall(int)));

    connect(mainWindow, SIGNAL(creReport()), client, SLOT(createReport()));
    connect(this, SIGNAL(need4users()), client, SLOT(ifNeedForUsers()));

    connect(client, SIGNAL(adUsr(int,QString,QString,int,QString,QString,QString)), users, SLOT(addRow(int,QString,QString,int,QString,QString,QString)));
    connect(mainWindow, SIGNAL(openUsrMenu()), this, SLOT(openUserMenu()));

    connect(users, SIGNAL(adUsr(QString,QString,int,QString,QString,QString)), client, SLOT(creAcc(QString,QString,int,QString,QString,QString)));
    connect(users, SIGNAL(modUsr(int,QString,QString,int,QString,QString,QString)), client, SLOT(modAcc(int,QString,QString,int,QString,QString,QString)));
    connect(users, SIGNAL(delUsr(int)), client, SLOT(delAcc(int)));

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

void MainController::openUserMenu()
{
    users->close();
    users->dataAd(false);
    users->clear();
    users->show();
    emit this->need4users();
}

void MainController::clusrs()
{
    users->clear();
}

void MainController::closAll()
{
    users->close();
    groups->close();
}

