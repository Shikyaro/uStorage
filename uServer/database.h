#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QDataStream>
#include <QCryptographicHash>
#include "permissions.h"


class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = 0);

signals:

public slots:
    bool initialize(QString hostname, QString dbName, QString username, QString password);
    bool login(QString username, QString password);
    QString getHalls();
    QString getItemsHalls(int i);
    QString getItemTypes();
    void testGetId();

    bool updateGroup(int id, QString name, QString comment);
    bool insertGroup(QString name, QString comment);
    bool deleteGroup(int id);

    bool insertItem(QString name, QString inv, int grp, int count, QString comment, int hallId);
    bool updateItem(int id, QString name, QString inv, int grp, int count, QString comment, int hallId);
    bool deleteItem(int id, int hallId);

    bool updateHall(int id, QString name, QString address, int room);
    bool insertHall(QString name, QString address, int room);
    bool delHall(int id);


    perms getPerms(int id);
    QString getUsers();
    bool createAcc(QString login, QString pass, int group, QString name, QString surname, QString middle);
    bool modUser(int id, QString login, QString pass, int group, QString name, QString surname, QString middle);
    bool delUser(int id);
    int getId(QString login);

private:
    QSqlDatabase db;
    QString hashPW(QString msg);
};

#endif // DATABASE_H
