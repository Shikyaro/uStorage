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

    db.open();

    testGetId();

    //return db.open();
    return true;


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
    QJsonArray itemsArray;
    QJsonObject itemsArrayContainer;

    query.prepare(QString("SELECT "
                          "    items.itemId, "
                          "    items.itemName, "
                          "    items.itemInventoryNum, "
                          "    items.itemGroup, "
                          "    itemgroups.itemGroupName, "
                          "    itemshalls.itemCount "
                          "FROM "
                          "    items "
                          "    INNER JOIN itemgroups ON items.itemGroup = itemgroups.itemGroupId "
                          "INNER JOIN itemshalls ON items.itemId = itemshalls.itemId "
                          "WHERE itemshalls.hallId = %1;").arg(i));
    if (query.exec()){
        while (query.next()) {
            QJsonObject item;
            item.insert("itemId", query.value(0).toInt());
            item.insert("itemName", query.value(1).toString());
            item.insert("itemInventoryNum", query.value(2).toString());
            item.insert("itemGroup", query.value(3).toInt());
            item.insert("itemGroupName", query.value(4).toString());
            item.insert("itemCount", query.value(5).toInt());

            itemsArray.append(item);
        }
        itemsArrayContainer["itemsArray"] = itemsArray;

        QJsonDocument itemsDoc(itemsArrayContainer);

        return itemsDoc.toJson(QJsonDocument::Compact);

    } else {
        qDebug() << query.lastError().databaseText();
        return "NULL";
    }
}

QString Database::getItemTypes()
{
    QSqlQuery query;
    QJsonArray groupsArray;
    QJsonObject groupsArrayContainer;

    query.prepare(QString("SELECT *"
                          "FROM itemgroups"));
    if(query.exec()){
        while(query.next()){
            QJsonObject group;
            group.insert("groupId", query.value(0).toInt());
            group.insert("groupName", query.value(1).toString());
            group.insert("groupComment", query.value(2).toString());

            groupsArray.append(group);
        }
        groupsArrayContainer["groupsArray"] = groupsArray;

        QJsonDocument groupsDoc(groupsArrayContainer);

        return groupsDoc.toJson(QJsonDocument::Compact);

    } else {
        qDebug() << query.lastError().databaseText();
        return "NULL";
    }
}

void Database::testGetId()
{
    QSqlQuery query;
    if(query.exec("CALL testGetId(@out)")){
        query.exec("SELECT @out");
        query.next();
            qDebug() <<"id: " << query.value(0).toInt();

    }else{
        qDebug() << query.lastError().databaseText();
    }

}
