#include "groupwidget.h"

GroupWidget::GroupWidget(QWidget *parent) : QWidget(parent)
{
    QGridLayout* mainLay = new QGridLayout();

    groupTable = new QTableWidget();
    groupTable->setColumnCount(3);
    QStringList grList = {"Id", "Название", "Комментарий"};
    groupTable->setHorizontalHeaderLabels(grList);

    mainLay->addItem(grList, 0, 0);

    addGr = new QPushButton("Добавить");
    modGr = new QPushButton("Изменить");
    delGr = new QPushButton("Удалить");

    QVBoxLayout *butlay = new QVBoxLayout();
    butlay->addItem(addGr);
    butlay->addItem(modGr);
    butlay->addItem(delGr);

}

