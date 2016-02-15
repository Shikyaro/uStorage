#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <halltabwidget.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void addHall(uint nid, QString nname, QString naddr, int nroomn, QString ncomm);
private:
    QWidget* mainWidget;
    QTabWidget* hallTabs;
};

#endif // MAINWINDOW_H
