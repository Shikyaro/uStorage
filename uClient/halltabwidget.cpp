#include "halltabwidget.h"

HallTabWidget::HallTabWidget(uint nHallId,
                             QString nHallName,
                             QString nHallAddr,
                             int nHallRommNum,
                             QString nHallComment,
                             QWidget *parent) : QWidget(parent)
{
    hallId = nHallId;
    hallName = nHallName;
    hallAddr = nHallAddr;
    hallRommNum = nHallRommNum;
    hallComment = nHallComment;

    mainLayout = new QGridLayout(this);
    this->setLayout(mainLayout);

    itemTable = new QTableWidget(this);
    mainLayout->addWidget(itemTable,0,0);
    itemTable->setColumnCount(6);
    QStringList columnHeaders;
    columnHeaders << tr("Номер") <<tr("Название") << tr("Инвентарный номер") << tr("Категория") << tr("Комментарий") << tr("Количество");
    itemTable->setHorizontalHeaderLabels(columnHeaders);
    itemTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    itemTable->setSelectionMode(QAbstractItemView::SingleSelection);
    itemTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //itemTable->insertRow(0);
    //itemTable->insertRow(1);
    //itemTable->insertRow(itemTable->rowCount()+1);

    editButtonBox = new QGroupBox("Редактирование");
    mainLayout->addWidget(editButtonBox,0,1);

    editButtonLayout = new QVBoxLayout();
    editButtonBox->setLayout(editButtonLayout);

    addButton = new QPushButton("Добавить");
    deleteButton = new QPushButton("Удалить");
    editButton = new QPushButton("Изменить");

    editButtonLayout->addWidget(addButton);
    editButtonLayout->addWidget(deleteButton);
    editButtonLayout->addWidget(editButton);
    editButtonLayout->addStretch();
}

void HallTabWidget::addRow(int nId, QString nName, QString nInv, int nGr, QString nCom, int nCou)
{
    itemTable->insertRow(itemTable->rowCount());

    itemTable->setItem(itemTable->rowCount()-1,0,new QTableWidgetItem(tr("%1").arg(nId)));
    itemTable->setItem(itemTable->rowCount()-1,1,new QTableWidgetItem(tr("%1").arg(nName)));
    itemTable->setItem(itemTable->rowCount()-1,2,new QTableWidgetItem(tr("%1").arg(nInv)));
    itemTable->setItem(itemTable->rowCount()-1,3,new QTableWidgetItem(tr("%1").arg(nGr)));
    itemTable->setItem(itemTable->rowCount()-1,4,new QTableWidgetItem(tr("%1").arg(nCom)));
    itemTable->setItem(itemTable->rowCount()-1,5,new QTableWidgetItem(tr("%1").arg(nCou)));
}

