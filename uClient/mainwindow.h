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
    QString getCurrHallName();
public slots:
    void addHall(uint nid, QString nname, QString naddr, int nroomn, QString ncomm);
    void addItem(int nId, QString nName, QString nInv, QString nGr, QString nCom, int nCou);
    void load2box(QList<groupItem*> *lis);

    void addIt(int id, QString name, QString inv, QString grp, QString com, int count);
    void modIt(int id, QString name, QString inv, QString grp, QString com, int count);
    void delIt(int id);

    void clearItms();

    void tabChanged();

    void modHall(int id, QString name, QString addr, int room);
    void updHall(int id, QString name, QString addr, int room);

    void insHall(QString name, QString addr, int room);
    void delHall(int id);

    void newHall();
signals:
    void openGroupMenu();

    void onEditS(int id, QString name, QString inv, QString grp, QString com, int count);
    void onAddS(int id, QString name, QString inv, QString grp, QString com, int count);
    void onDeleteS(int id);

    void tabChSig(int id);

    void addHallSig(int, QString, QString, int);

    void modHallSig(int id, QString name, QString addr, int room);
    void insHallSig(QString name, QString addr, int room);
    void delHallSig(int id);

    void creReport();

    void openUsrMenu();

private:
    QWidget* mainWidget;
    QTabWidget* hallTabs;
    QPushButton* addHbut = new QPushButton("Добавить");
    QLineEdit* nameHal = new QLineEdit();


private slots:
    void on_openGroup();
    void onOpenUsr();
    //void onAdHal();
};

#endif // MAINWINDOW_H
