#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("uClient by Alkor Shikyaro");
    this->setMinimumSize(840,480);
    mainWidget = new QWidget();
    this->setCentralWidget(mainWidget);

    QGridLayout* mainLayout = new QGridLayout(mainWidget);
    mainWidget->setLayout(mainLayout);

    hallTabs = new QTabWidget();
    mainLayout->addWidget(hallTabs,0,0,Qt::AlignTop & Qt::AlignLeft);

    hallTabs->setMinimumSize(600,400);
    //HallTabWidget* widg = new HallTabWidget(1);
    //hallTabs->addTab(widg,"123");

}

MainWindow::~MainWindow()
{

}

void MainWindow::addHall(uint nid, QString nname, QString naddr, int nroomn, QString ncomm)
{
    HallTabWidget* widg = new HallTabWidget(nid, nname, naddr, nroomn, ncomm);
    hallTabs->addTab(widg, nname);
}

void MainWindow::addItem(int nId, QString nName, QString nInv, int nGr, QString nCom, int nCou)
{
    HallTabWidget *curTab = static_cast<HallTabWidget*>(hallTabs->widget(hallTabs->currentIndex()));
    qDebug() << "additm";
    curTab->addRow(nId, nName, nInv, nGr, nCom, nCou);

}

int MainWindow::getCurrHallId()
{
    HallTabWidget* curTab = static_cast<HallTabWidget*>(hallTabs->widget(hallTabs->currentIndex()));
    return curTab->getHallId();
}
