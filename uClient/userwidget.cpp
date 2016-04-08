#include "userwidget.h"

UserWidget::UserWidget(QWidget *parent) : QWidget(parent)
{
    QGridLayout* mainlay = new QGridLayout();
    this->setLayout(mainlay);

    userTable = new QTableWidget();
    userTable->setColumnCount(7);
    userTable->setHorizontalHeaderLabels({"Id", "Логин", "Пароль", "Группа", "Имя", "Фамилия", "Отчество"});
    userTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    userTable->setSelectionMode(QAbstractItemView::SingleSelection);
    userTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainlay->addWidget(userTable,0,0);

    grBox = new QComboBox();

    grBox->addItems({"Админ", "Редактор", "Пользователь"});

    idL = new QLineEdit();
    idL->setReadOnly(true);
    logL = new QLineEdit();
    pwL = new QLineEdit();
    namL = new QLineEdit();
    surL = new QLineEdit();
    midL = new QLineEdit();

    QHBoxLayout* editLay = new QHBoxLayout();
    mainlay->addLayout(editLay, 1,0);

    editLay->addWidget(idL);
    editLay->addWidget(logL);
    editLay->addWidget(pwL);
    editLay->addWidget(grBox);
    editLay->addWidget(namL);
    editLay->addWidget(surL);
    editLay->addWidget(midL);

    addU = new QPushButton("Добавить");
    modU = new QPushButton("Изменить");
    delU = new QPushButton("Удалить");

    QVBoxLayout* butnLay = new QVBoxLayout();
    mainlay->addLayout(butnLay, 0,1);

    butnLay->addWidget(addU);
    butnLay->addWidget(modU);
    butnLay->addWidget(delU);

    connect(userTable, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(onSelectRow(int)));
    connect(addU, SIGNAL(clicked(bool)), this, SLOT(onAdd()));
    connect(modU, SIGNAL(clicked(bool)), this, SLOT(onMod()));
    connect(delU, SIGNAL(clicked(bool)), this, SLOT(onDel()));
}

void UserWidget::dataAd(bool d)
{
    datta = d;
}

void UserWidget::onSelectRow(int rowId)
{
    if(datta){
        idL->setText(userTable->item(rowId, 0)->text());
        logL->setText(userTable->item(rowId, 1)->text());
        grBox->setCurrentText(userTable->item(rowId, 3)->text());
        namL->setText(userTable->item(rowId, 4)->text());
        surL->setText(userTable->item(rowId, 5)->text());
        midL->setText(userTable->item(rowId, 6)->text());
    }
}

void UserWidget::addRow(int id, QString log, QString pass, int grId, QString nam, QString sur, QString mid)
{
    userTable->insertRow(userTable->rowCount());
    userTable->setItem(userTable->rowCount()-1, 0, new QTableWidgetItem(QString("%1").arg(id)));
    userTable->setItem(userTable->rowCount()-1, 1, new QTableWidgetItem(QString("%1").arg(log)));
    userTable->setItem(userTable->rowCount()-1, 2, new QTableWidgetItem(QString("%1").arg(pass)));
    userTable->setItem(userTable->rowCount()-1, 3, new QTableWidgetItem(QString("%1").arg(grBox->itemText(grId-1))));
    userTable->setItem(userTable->rowCount()-1, 4, new QTableWidgetItem(QString("%1").arg(nam)));
    userTable->setItem(userTable->rowCount()-1, 5, new QTableWidgetItem(QString("%1").arg(sur)));
    userTable->setItem(userTable->rowCount()-1, 6, new QTableWidgetItem(QString("%1").arg(mid)));
    datta = true;
}

void UserWidget::onAdd()
{
    emit this->adUsr(logL->text(), pwL->text(), grBox->currentIndex()+1, namL->text(), surL->text(), midL->text());
}

void UserWidget::onMod()
{
    emit this->modUsr(idL->text().toInt(), logL->text(), pwL->text(), grBox->currentIndex()+1, namL->text(), surL->text(), midL->text());
}

void UserWidget::onDel()
{
    emit this->delUsr(idL->text().toInt());
}

void UserWidget::clear()
{
    datta = false;
    userTable->setRowCount(0);
}
