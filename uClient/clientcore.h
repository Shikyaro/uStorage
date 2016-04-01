#ifndef CLIENTCORE_H
#define CLIENTCORE_H



#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QCryptographicHash>
#include <QList>
#include <QHash>
#include <storeditem.h>
#include <maincontroller.h>
#include "../uServer/serverclient.h"
#include "xlsxdocument.h"
#include <QCryptographicHash>

class MainController;

/*struct groupItem{
    int id;
    QString name;
    QString comment;
};*/

class ClientCore : public QObject
{
    Q_OBJECT
public:
    explicit ClientCore(QObject *parent = 0);

signals:
    void successConnect();
    void unSuccessConnect();

    void successLogin();
    void unSuccessLogin();

    void addHall(uint, QString , QString, int, QString);
    void addItem(int, QString, QString, QString, QString, int);
    void addGroup(int, QString, QString);
    void groupAd(bool);
    void groupClear();

    void groupsToBox(QList<groupItem*> *);

    void updateCurHall(int, QString, QString, int);

public slots:
    bool connectToServer(QString ip, quint16 port);
    void login(QString ipAddr, QString login, QString password);
    void addNewItem(int id, QString name, QString invNum, int grpId, QString comment, int hallId, int itmCount, QString grNme);
    void itemsToTable();
    void getItemsFromHall(int id);
    void ifNeedForGroups();

    void editGroup(int id, QString name, QString comment);
    void groupsToTable();
    void insertGroup(QString name, QString comment);
    void deleteGroup(int id);
    //void groupsToBox();

    void insertItem(int id, QString name, QString inv, QString grp, QString com, int count);
    void editItem(int id, QString name, QString inv, QString grp, QString com, int count);
    void deleteItem(int id);

    void editHall(int id, QString name, QString addr, int room);
    void insertHall(QString name, QString addr, int room);
    void delHall(int id);

    void createReport();
private:
    QTcpSocket* mainSocket;
    bool isConnected = false;
    void sendBlock(quint8 command, QByteArray *data);
    quint16 blockSize = 0;

    QList<storedItem*> itemsInCurrHall;
    //QHash<int, storedItem*> itemsInHal;
    QList<groupItem*> groupsOfItem;
    MainController* mCPointer;

private slots:
    void onReadyRead();
    QString hashPW(QString msg);
};

#endif // CLIENTCORE_H
