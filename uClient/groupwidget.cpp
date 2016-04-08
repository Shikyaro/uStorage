#include "groupwidget.h"

GroupWidget::GroupWidget(QWidget *parent) : QWidget(parent)
{

    QGridLayout* mainLay = new QGridLayout();

    this->setLayout(mainLay);

    groupTable = new QTableWidget();
    groupTable->setColumnCount(3);
    QStringList grList = {"Id", "Название", "Комментарий"};
    groupTable->setHorizontalHeaderLabels(grList);

    groupTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    groupTable->setSelectionMode(QAbstractItemView::SingleSelection);
    groupTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainLay->addWidget(groupTable, 0, 0);

    addGr = new QPushButton("Добавить");
    modGr = new QPushButton("Изменить");
    delGr = new QPushButton("Удалить");

    QVBoxLayout *butlay = new QVBoxLayout();
    butlay->addWidget(addGr);
    butlay->addWidget(modGr);
    butlay->addWidget(delGr);

    mainLay->addLayout(butlay,0,1);

    grNum = new QLineEdit();
    grNum->setReadOnly(true);
    grName = new QLineEdit();
    grComm = new QLineEdit();

    QHBoxLayout* editlay = new QHBoxLayout();

    editlay->addWidget(grNum);
    editlay->addWidget(grName);
    editlay->addWidget(grComm);

    mainLay->addLayout(editlay,1,0);

    connect(groupTable, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(onSelect(int)));
    connect(addGr, SIGNAL(clicked(bool)), this, SLOT(onAdd()));
    connect(modGr, SIGNAL(clicked(bool)), this, SLOT(onMod()));
    connect(delGr, SIGNAL(clicked(bool)), this, SLOT(onDel()));
}

void GroupWidget::onSelect(int rowId)
{
    groupTable->selectionModel()->clearSelection();
    //groupTable->clearSelection();
    if(dataAdded){
        grNum->setText(groupTable->item(rowId, 0)->text());
        grName->setText(groupTable->item(rowId, 1)->text());
        grComm->setText(groupTable->item(rowId, 2)->text());
    }
}

void GroupWidget::onAdd()
{
    if(grName->text() != "" && grComm->text() != ""){
        emit this->onAddClicked(grName->text(), grComm->text());
    }
}

void GroupWidget::onMod()
{
    if(grNum->text()!= "" && grName->text() != "" && grComm->text() != ""){
        emit this->onModClicked(grNum->text().toInt(), grName->text(), grComm->text());
    }
}

void GroupWidget::onDel()
{
    if(grNum->text()!= ""){
        emit this->onDelClicked(grNum->text().toInt());
    }
}

void GroupWidget::addGroup(int id, QString name, QString com)
{
    qDebug() << "qwer";
    groupTable->insertRow(groupTable->rowCount());
    groupTable->setItem(groupTable->rowCount()-1, 0, new QTableWidgetItem(QString("%1").arg(id)));
    groupTable->setItem(groupTable->rowCount()-1, 1, new QTableWidgetItem(name));
    groupTable->setItem(groupTable->rowCount()-1, 2, new QTableWidgetItem(com));
    groupTable->selectRow(0);
}

void GroupWidget::clearGroups()
{
    qDebug() << "cler";
    //groupTable->clearContents();
    dataAdded = false;
    groupTable->setRowCount(0);
}

void GroupWidget::dataAD(bool dat)
{
    dataAdded = dat;
}
