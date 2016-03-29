#ifndef HALLTABWIDGET_H
#define HALLTABWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <groupitem.h>

class HallTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HallTabWidget(uint nHallId,
                           QString nHallName,
                           QString nHallAddr,
                           int nHallRommNum,
                           QString nHallComment,
                           QWidget *parent = 0);

    int getHallId(){return hallId;}
    QTableWidget* getItemTable(){return itemTable;}
    void addRow(int nId, QString nName, QString nInv, QString nGr, QString nCom, int nCou);

signals:

public slots:
    void loadToBox(QList<groupItem*> *list);


private:
    uint hallId;
    QString hallName;
    QString hallAddr;
    int hallRommNum;
    QString hallComment;

    QTableWidget* itemTable;
    QGridLayout* mainLayout;
    QGroupBox* editButtonBox;

    QPushButton* addButton;
    QPushButton* deleteButton;
    QPushButton* editButton;

    QVBoxLayout* editButtonLayout;

    QLineEdit* iId, *iName, *iInv, *iCom, *iCou;
    QComboBox* grBox;
};

#endif // HALLTABWIDGET_H
