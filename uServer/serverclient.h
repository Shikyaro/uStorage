#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#define SERVER_PORT quint16(6969)

#include <QObject>
#include <QtNetwork>
#include <QString>
#include <QDebug>
#include "servercore.h"

class ServerCore;

class ServerClient : public QObject
{
    Q_OBJECT
public:
    explicit ServerClient(qintptr socketDescriptor, ServerCore *server, QObject *parent = 0);
    ~ServerClient();

    static const quint8 testComm = 1;
    static const quint8 loginComm = 2;
    static const quint8 succLoginComm = 3;
    static const quint8 unSuccLoginComm = 4;
    static const quint8 getHallsComm = 5;
    static const quint8 getHallItems = 6;
    static const quint8 getItemGroups = 7;
    static const quint8 editItemGroup = 9;
    static const quint8 editItem = 10;
    static const quint8 editHall = 11;
    static const quint8 succEditItem = 12;
    static const quint8 addGroup = 13;
    static const quint8 succAddGroup = 14;
    static const quint8 delGroup = 15;
    static const quint8 succDelGroup = 16;
    static const quint8 addItem = 17;
    static const quint8 succAddItem = 18;
    static const quint8 modItem = 19;
    static const quint8 succModItem = 20;
    static const quint8 delItem = 21;
    static const quint8 succDelItem = 22;
    static const quint8 modHall = 23;
    static const quint8 succModHall = 24;
    static const quint8 addHall = 25;
    static const quint8 succAddHall = 26;
    static const quint8 delHall = 27;
    static const quint8 succDelHall = 28;
    static const quint8 addUsr = 29;
    static const quint8 delUsr = 30;
    static const quint8 modUsr = 31;
    static const quint8 succAddUsr = 32;
    static const quint8 succDelUsr = 33;
    static const quint8 succModUsr = 34;
    static const quint8 getUsr = 35;
    static const quint8 permRestrict = 69;
    static const quint8 err = 36;

signals:

public slots:

private:
    QTcpSocket* mainSocket;
    quint16 blockSize = 0;
    ServerCore *servPtr;
    bool isLoggedIn = false;
    int uId = 0;
private slots:
    void onReadyRead();
    void onDisconnect();

    void sendBlock(quint8 command, QByteArray* data);
};

#endif // SERVERCLIENT_H
