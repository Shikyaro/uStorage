#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include "mainwindow.h"
#include "servercore.h"

class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = 0);
    ~MainController();
    void showInterface();

signals:

public slots:
    
private:
    MainWindow* mainWindow;
    ServerCore* server;

};

#endif // MAINCONTROLLER_H
