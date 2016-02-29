#ifndef SERVERCORE_H
#define SERVERCORE_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include "database.h"
#include "serverclient.h"
#include <QList>


class ServerClient;

class ServerCore : public QTcpServer
{
    Q_OBJECT
public:
    explicit ServerCore(QObject *parent = 0);
    ~ServerCore();

signals:
protected:
    void incomingConnection(qintptr handle);

public slots:
    void startServer();
    bool login(QString username, QString password);
    QString getHalls();
    QJsonObject getItemsHalls(int halid);
private:
    QList<ServerClient* > clientList;
    Database* db;

};

#endif // SERVERCORE_H
