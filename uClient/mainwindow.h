#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <halltabwidget.h>
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void addHall(uint nid, QString nname, QString naddr, int nroomn, QString ncomm);
    void addItem(int nId, QString nName, QString nInv, int nGr, QString nCom, int nCou);
private:
    QWidget* mainWidget;
    QTabWidget* hallTabs;
};

#endif // MAINWINDOW_H
