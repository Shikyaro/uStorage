#include "database.h"

Database::Database(QObject *parent) : QObject(parent)
{

}

bool Database::initialize(QString hostname, QString dbName, QString username, QString password)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostname);
    db.setPort(3306);
    db.setDatabaseName(dbName);
    db.setUserName(username);
    db.setPassword(password);

    return db.open();
}

bool Database::login(QString username, QString password)
{
    if (username != "" && password != ""){
        QSqlQuery query;
        query.prepare(QString("SELECT userPassword "
                      "FROM users "
                      "WHERE userName = \"%1\"").arg(username));
        if (query.exec()){
            query.last();
            return query.value(0)==password;
        } else {
            qDebug() << query.lastError().databaseText();
            return false;
        }
    } else {
        return false;
    }
}

QString Database::getHalls()
{
    QSqlQuery query;
    QString querys;
    query.prepare("SELECT * FROM storageHalls");
    if (query.exec()){
        while (query.next()) {
            querys.append(query.value(0).toString()).append("|").append(query.value(1).toString()).append("|");
            querys.append(query.value(2).toString()).append("|").append(query.value(3).toString()).append("|");
            querys.append(query.value(4).toString()).append("/|/");
        }
        querys.remove(querys.length()-3,3);
        return querys;
    } else {
        qDebug() << query.lastError().databaseText();
        return "NULL";
    }
}

QString Database::getItemsHalls(int i)
{
    QSqlQuery query;
    QString querys;
    query.prepare(QString("SELECT"
                  "    items.itemId, "
                  "    items.itemName, "
                  "    items.itemInventoryNum, "
                  "    itemgroups.itemGroupName, "
                  "    items.itemComment "
                  "    FROM"
                  "    ("
                  "        items "
                  "        INNER JOIN itemshalls ON items.itemId = itemshalls.itemId "
                  "    ) "
                  "INNER JOIN itemgroups ON items.itemGroup = itemgroups.itemGroupId "
                  "WHERE "
                  "    itemshalls.hallId = %1").arg(i));
    if (query.exec()){
        while (query.next()) {

        }
    }
}
