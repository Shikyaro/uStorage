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
    itemTable->insertRow(0);
    itemTable->insertRow(1);

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

