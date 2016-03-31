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
#include <QDebug>

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
    void clearTable();

signals:
    void onEditS(int id, QString name, QString inv, QString grp, QString com, int count);
    void onAddS(int id, QString name, QString inv, QString grp, QString com, int count);
    void onDeleteS(int id);

    void onModHalS(int, QString, QString, int);
    void onAddHalS(QString, QString, int);
    void onDelHalS(int);

public slots:
    void loadToBox(QList<groupItem*> *list);
    void onChanClicked();
    void setDat(bool dat);

    void onEdit();
    void onAdd();
    void onDelete();

    void reloadFields();

    void onModHal();
    void onAddHal();
    void onDelHal();

    void updHallData(int id, QString name, QString adr, int romNum);
private slots:
    void onSelect();


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

    QLineEdit* hId, *hName, *hAddr, *hRoom;
    QPushButton* adHal, *modHal, *delHal;

    bool isDat = false;
};

#endif // HALLTABWIDGET_H
