#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <mainwindow.h>
#include "loginwidget.h"
#include "clientcore.h"
#include "groupwidget.h"
#include "userwidget.h"

class ClientCore;
class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = 0);
    ~MainController();
    void showInterface();
    int getCurrHallId(){return mainWindow->getCurrHallId();}
    void cItTab(){mainWindow->clearItms();}
    QString getCurrHallName(){return mainWindow->getCurrHallName();}
    void clusrs();

    void closAll();

signals:
    void needGroups();
    void need4users();

public slots:
    void onSuccessLogin();
    void openGroupMenu();

    void openUserMenu();

private:
    MainWindow* mainWindow;
    LoginWidget* loginWidget;
    ClientCore* client;
    GroupWidget* groups;
    UserWidget* users;

};

#endif // MAINCONTROLLER_H
