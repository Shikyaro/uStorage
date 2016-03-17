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

class MainController;

struct groupItem{
    int id;
    QString name;
    QString comment;
};

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
    void addItem(int, QString, QString, int, QString, int);

public slots:
    bool connectToServer(QString ip, quint16 port);
    void login(QString ipAddr, QString login, QString password);
    void addNewItem(int id, QString name, QString invNum, int grpId, QString comment, int hallId, int itmCount);
    void itemsToTable();
    void getItemsFromHall(int id);

    void editGroup(int id, QString name, QString comment);
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
};

#endif // CLIENTCORE_H
