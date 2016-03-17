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

signals:

public slots:

private:
    QTcpSocket* mainSocket;
    quint16 blockSize = 0;
    ServerCore *servPtr;
    bool isLoggedIn = false;
private slots:
    void onReadyRead();
    void onDisconnect();

    void sendBlock(quint8 command, QByteArray* data);
};

#endif // SERVERCLIENT_H
