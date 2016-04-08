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

    QMenuBar *mainMenuBar = new QMenuBar();

    QMenu *mainMenu = new QMenu("Главное");
    mainMenuBar->addMenu(mainMenu);
    mainLayout->setMenuBar(mainMenuBar);
    QAction *groupsMenu = new QAction("Работа с группами", mainMenu);
    mainMenu->addAction(groupsMenu);

    QMenu *hallMenu = new QMenu("Склады");
    mainMenuBar->addMenu(hallMenu);
    QAction* addHallMenu =new QAction("Создать отчет", hallMenu);
    hallMenu->addAction(addHallMenu);

    QMenu *adminMenu = new QMenu("Администрирование");
    mainMenuBar->addMenu(adminMenu);
    QAction* usersMenu = new QAction("Работа с пользователями", adminMenu);
    adminMenu->addAction(usersMenu);



    connect(groupsMenu, SIGNAL(triggered(bool)), this, SLOT(on_openGroup()));
    connect(hallTabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged()));
    connect(addHallMenu, SIGNAL(triggered(bool)), this, SLOT(newHall()));
    connect(usersMenu, SIGNAL(triggered(bool)), this, SLOT(onOpenUsr()));

}

MainWindow::~MainWindow()
{

}

void MainWindow::addHall(uint nid, QString nname, QString naddr, int nroomn, QString ncomm)
{
    HallTabWidget* widg = new HallTabWidget(nid, nname, naddr, nroomn, ncomm);
    hallTabs->addTab(widg, nname);

    connect(widg, SIGNAL(onAddS(int,QString,QString,QString,QString,int)), this, SLOT(addIt(int,QString,QString,QString,QString,int)));
    connect(widg, SIGNAL(onEditS(int,QString,QString,QString,QString,int)), this, SLOT(modIt(int,QString,QString,QString,QString,int)));
    connect(widg, SIGNAL(onDeleteS(int)), this, SLOT(delIt(int)));

    connect(widg, SIGNAL(onModHalS(int,QString,QString,int)), this, SLOT(modHall(int,QString,QString,int)));
    connect(widg, SIGNAL(onAddHalS(QString,QString,int)), this , SLOT(insHall(QString,QString,int)));
    connect(widg, SIGNAL(onDelHalS(int)), this, SLOT(delHall(int)));
}

void MainWindow::addItem(int nId, QString nName, QString nInv, QString nGr, QString nCom, int nCou)
{
    HallTabWidget *curTab = static_cast<HallTabWidget*>(hallTabs->widget(hallTabs->currentIndex()));
    qDebug() << "additm";
    curTab->addRow(nId, nName, nInv, nGr, nCom, nCou);
    curTab->setDat(true);
}

int MainWindow::getCurrHallId()
{
    HallTabWidget* curTab = static_cast<HallTabWidget*>(hallTabs->widget(hallTabs->currentIndex()));
    return curTab->getHallId();
}

void MainWindow::on_openGroup()
{
    emit this->openGroupMenu();
}

void MainWindow::load2box(QList<groupItem *> *lis)
{
    qDebug() << "lod mwid";
    static_cast<HallTabWidget*>(hallTabs->widget(hallTabs->currentIndex()))->loadToBox(lis);
}

void MainWindow::addIt(int id, QString name, QString inv, QString grp, QString com, int count)
{
    emit this->onAddS(id, name, inv, grp, com, count);

}

void MainWindow::modIt(int id, QString name, QString inv, QString grp, QString com, int count)
{
    emit this->onEditS(id, name, inv, grp, com, count);
}

void MainWindow::delIt(int id)
{
    emit this->onDeleteS(id);
}

void MainWindow::clearItms()
{
    static_cast<HallTabWidget*>(hallTabs->widget(hallTabs->currentIndex()))->clearTable();
}

void MainWindow::tabChanged()
{
    static_cast<HallTabWidget*>(hallTabs->widget(hallTabs->currentIndex()))->clearTable();
    emit this->tabChSig(static_cast<HallTabWidget*>(hallTabs->widget(hallTabs->currentIndex()))->getHallId());
    static_cast<HallTabWidget*>(hallTabs->widget(hallTabs->currentIndex()))->reloadFields();
}

void MainWindow::modHall(int id, QString name, QString addr, int room)
{
    emit this->modHallSig(id, name, addr, room);
}

void MainWindow::updHall(int id, QString name, QString addr, int room)
{
    static_cast<HallTabWidget*>(hallTabs->widget(hallTabs->currentIndex()))->updHallData(id, name, addr, room);
}

void MainWindow::insHall(QString name, QString addr, int room)
{
    emit this->insHallSig(name, addr, room);
}

void MainWindow::delHall(int id)
{
    emit this->delHallSig(id);
}

QString MainWindow::getCurrHallName()
{
     return static_cast<HallTabWidget*>(hallTabs->widget(hallTabs->currentIndex()))->hallName;
}

void MainWindow::newHall()
{
    emit this->creReport();
}

void MainWindow::onOpenUsr()
{
    emit this->openUsrMenu();
}
