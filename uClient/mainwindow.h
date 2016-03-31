#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <halltabwidget.h>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int getCurrHallId();
public slots:
    void addHall(uint nid, QString nname, QString naddr, int nroomn, QString ncomm);
    void addItem(int nId, QString nName, QString nInv, QString nGr, QString nCom, int nCou);
    void load2box(QList<groupItem*> *lis);

    void addIt(int id, QString name, QString inv, QString grp, QString com, int count);
    void modIt(int id, QString name, QString inv, QString grp, QString com, int count);
    void delIt(int id);

    void clearItms();
signals:
    void openGroupMenu();

    void onEditS(int id, QString name, QString inv, QString grp, QString com, int count);
    void onAddS(int id, QString name, QString inv, QString grp, QString com, int count);
    void onDeleteS(int id);

private:
    QWidget* mainWidget;
    QTabWidget* hallTabs;

private slots:
    void on_openGroup();
};

#endif // MAINWINDOW_H
