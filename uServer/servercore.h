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
    QString getItemsHalls(int halid);
    QString getItemGroups();
    bool updateGroups(int id, QString name, QString comment);
    bool insertGroup(QString name, QString comment);
    bool deleteGroup(int id);
    bool insertItem(QString name, QString inv, int grp, int count, QString comment, int hallId);
    bool updateItem(int id, QString name, QString inv, int grp, int count, QString comment, int hallId);
    bool deleteItem(int id, int hallId);
private:
    QList<ServerClient* > clientList;
    Database* db;

};

#endif // SERVERCORE_H
