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
    this->setMinimumWidth(640);

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

    QVBoxLayout* boxLayout = new QVBoxLayout();

    editButtonBox = new QGroupBox("Редактирование");
    boxLayout->addWidget(editButtonBox);


    editButtonLayout = new QVBoxLayout();
    editButtonBox->setLayout(editButtonLayout);

    addButton = new QPushButton("Добавить");
    deleteButton = new QPushButton("Удалить");
    editButton = new QPushButton("Изменить");

    editButtonLayout->addWidget(addButton);
    editButtonLayout->addWidget(deleteButton);
    editButtonLayout->addWidget(editButton);
    editButtonLayout->addStretch();

    QGroupBox* hallBox = new QGroupBox("Управление складом");
    boxLayout->addWidget(hallBox);

    QVBoxLayout* halLay = new QVBoxLayout;
    hallBox->setLayout(halLay);
    mainLayout->addLayout(boxLayout,0,1);

    hId = new QLineEdit();
    hName = new QLineEdit();
    hAddr = new QLineEdit();
    hRoom = new QLineEdit();

    hId->setReadOnly(true);

    reloadFields();

    adHal = new QPushButton("Добавить склад");
    modHal = new QPushButton("Изменить склад");
    delHal = new QPushButton("Удалить склад");

    halLay->addWidget(hId);
    halLay->addWidget(hName);
    halLay->addWidget(hAddr);
    halLay->addWidget(hRoom);

    halLay->addWidget(adHal);
    halLay->addWidget(modHal);
    halLay->addWidget(delHal);



    QHBoxLayout* editLay = new QHBoxLayout();
    mainLayout->addLayout(editLay,1,0);

    grBox = new QComboBox();
    grBox->setMinimumWidth(130);

    iId = new QLineEdit();
    iId->setReadOnly(true);

    iName = new QLineEdit();
    iInv = new QLineEdit();
    iCom = new QLineEdit();
    iCou = new QLineEdit();

    editLay->addWidget(iId);
    editLay->addWidget(iName);
    editLay->addWidget(iInv);
    editLay->addWidget(grBox);
    editLay->addWidget(iCom);
    editLay->addWidget(iCou);

    connect(itemTable, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(onSelect()));
    connect(editButton, SIGNAL(clicked(bool)), this, SLOT(onEdit()));
    connect(addButton, SIGNAL(clicked(bool)), this, SLOT(onAdd()));
    connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(onDelete()));

    connect(modHal, SIGNAL(clicked(bool)), this, SLOT(onModHal()));
    connect(adHal, SIGNAL(clicked(bool)), this, SLOT(onAddHal()));
    connect(delHal, SIGNAL(clicked(bool)), this, SLOT(onDelHal()));
}

void HallTabWidget::addRow(int nId, QString nName, QString nInv, QString nGr, QString nCom, int nCou)
{
    itemTable->insertRow(itemTable->rowCount());

    itemTable->setItem(itemTable->rowCount()-1,0,new QTableWidgetItem(tr("%1").arg(nId)));
    itemTable->setItem(itemTable->rowCount()-1,1,new QTableWidgetItem(tr("%1").arg(nName)));
    itemTable->setItem(itemTable->rowCount()-1,2,new QTableWidgetItem(tr("%1").arg(nInv)));
    itemTable->setItem(itemTable->rowCount()-1,3,new QTableWidgetItem(tr("%1").arg(nGr)));
    itemTable->setItem(itemTable->rowCount()-1,4,new QTableWidgetItem(tr("%1").arg(nCom)));
    itemTable->setItem(itemTable->rowCount()-1,5,new QTableWidgetItem(tr("%1").arg(nCou)));
    isDat = true;
}

void HallTabWidget::loadToBox(QList<groupItem *> *list)
{
    //qDebug() << "lod hwid";
    grBox->clear();
    for (int i = 0; i < list->size(); i++){
        grBox->addItem(list->at(i)->name);
    }
}

void HallTabWidget::onSelect()
{
    if(isDat){
        int cRow = itemTable->currentRow();
        iId->setText(itemTable->item(cRow, 0)->text());
        iName->setText(itemTable->item(cRow, 1)->text());
        iInv->setText(itemTable->item(cRow, 2)->text());
        iCom->setText(itemTable->item(cRow, 4)->text());
        iCou->setText(itemTable->item(cRow, 5)->text());
        //iInv->setText(itemTable->item(cRow, 5));
        grBox->setCurrentText(itemTable->item(cRow,3)->text());
    }
}

void HallTabWidget::clearTable()
{
    isDat = false;
    itemTable->setRowCount(0);
}

void HallTabWidget::onChanClicked()
{
    clearTable();
}

void HallTabWidget::setDat(bool dat)
{
    isDat = dat;
}

void HallTabWidget::onEdit()
{
    emit this->onEditS(iId->text().toInt(),
                       iName->text(),
                       iInv->text(),
                       grBox->currentText(),
                       iCom->text(),
                       iCou->text().toInt());
}

void HallTabWidget::onDelete()
{
    bool ok;
    QMessageBox mb;
    mb.setText("Внимание");
    mb.setInformativeText("Вы хотите удалить это?");
    mb.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);

    int ret = mb.exec();

    if(ret == QMessageBox::Ok){
        emit this->onDeleteS(iId->text().toInt());
    }
}

void HallTabWidget::onAdd()
{
    emit this->onAddS(iId->text().toInt(),
                       iName->text(),
                       iInv->text(),
                       grBox->currentText(),
                       iCom->text(),
                       iCou->text().toInt());

}

void HallTabWidget::reloadFields()
{
    hId->setText(QString("%1").arg(hallId));
    hRoom->setText(QString("%1").arg(hallRommNum));
    hName->setText(hallName);
    hAddr->setText(hallAddr);
}

void HallTabWidget::onModHal()
{
    emit this->onModHalS(hId->text().toInt(), hName->text(), hAddr->text(), hRoom->text().toInt());
}

void HallTabWidget::updHallData(int id, QString name, QString adr, int romNum)
{
    hallId = id;
    hallName = name;
    hallAddr = adr;
    hallRommNum = romNum;
}

void HallTabWidget::onAddHal()
{
    emit this->onAddHalS(hName->text(), hAddr->text(), hRoom->text().toInt());
}

void HallTabWidget::onDelHal()
{
    emit this->onDelHalS(hId->text().toInt());
}
