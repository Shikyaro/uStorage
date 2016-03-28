#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include "mainwindow.h"
#include "loginwidget.h"
#include "clientcore.h"
#include "groupwidget.h"

class ClientCore;
class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = 0);
    ~MainController();
    void showInterface();
    int getCurrHallId(){return mainWindow->getCurrHallId();}

signals:
    void needGroups();

public slots:
    void onSuccessLogin();
    void openGroupMenu();

private:
    MainWindow* mainWindow;
    LoginWidget* loginWidget;
    ClientCore* client;
    GroupWidget* groups;

};

#endif // MAINCONTROLLER_H
