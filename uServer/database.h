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
private:
    QSqlDatabase db;
};

#endif // DATABASE_H
