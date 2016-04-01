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

    return db.open();
    //return true;


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
                          "    itemshalls.itemCount,"
                          "    items.itemComment "
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
            item.insert("itemComment", query.value(6).toString());

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

bool Database::updateGroup(int id, QString name, QString comment)
{
    QSqlQuery query;

    query.prepare(QString("UPDATE itemgroups "
                          "SET itemGroupName = '%2', "
                          "itemGroupComment = '%3' "
                          "WHERE itemGroupId = %1").arg(id).arg(name).arg(comment));
    if(query.exec()){
        return true;
    }else{
        qDebug() << query.lastError().databaseText();
        return false;
    }
}

bool Database::insertGroup(QString name, QString comment)
{
    QSqlQuery query;

    query.prepare(QString("INSERT into itemgroups(itemGroupName, itemGroupComment) "
                          "VALUES (\"%1\", \"%2\")").arg(name, comment));
    if(query.exec()){
        return true;
    }else{
        qDebug() << query.lastError().databaseText();
        return false;
    }
}

bool Database::deleteGroup(int id)
{
    QSqlQuery query;

    query.prepare(QString("DELETE FROM itemgroups "
                          "WHERE itemGroupId = %1").arg(id));
    if(query.exec()){
        return true;
    }else{
        qDebug() << query.lastError().databaseText();
        return false;
    }
}

bool Database::insertItem(QString name, QString inv, int grp, int count, QString comment, int hallId)
{
    //qDebug() << "iniT";
    QSqlQuery query;

    QString z1 = QString("INSERT INTO items(items.itemName, items.itemInventoryNum, items.itemGroup, items.itemComment) "
                         "VALUES('%1', '%2', %3, '%4');").arg(name, inv).arg(grp).arg(comment);
    QString z2 = QString("INSERT INTO itemshalls(itemshalls.itemId, itemshalls.hallId, itemshalls.itemCount) "
                         "VALUES ((SELECT items.itemId FROM items WHERE items.itemInventoryNum = '%1'), %2, %3 );").arg(inv).arg(hallId).arg(count);

    query.prepare(z1+z2);
    if(query.exec()){
        return true;
    }else{
        qDebug() << query.lastError().databaseText();
        return false;
    }
}

bool Database::updateItem(int id, QString name, QString inv, int grp, int count, QString comment, int hallId)
{
    QSqlQuery query;

    QString z1 = QString("UPDATE items "
                         "SET itemName = '%1', itemInventoryNum = '%2', itemGroup = %3, itemComment = '%4' "
                         "WHERE itemId = %5; ").arg(name).arg(inv).arg(grp).arg(comment).arg(id);
    QString z2 = QString("UPDATE itemsHalls "
                         "SET itemCount = %2 "
                         "WHERE hallId = %1 AND itemId = %3;").arg(hallId).arg(count).arg(id);

    query.prepare(z1+z2);
    if(query.exec()){
        return true;
    }else{
        qDebug() << query.lastError().databaseText();
        return false;
    }

}

bool Database::deleteItem(int id, int hallId)
{
    QSqlQuery query;
    query.prepare(QString("START TRANSACTION; "
                          "DELETE FROM itemsHalls "
                          "WHERE hallId = %2 AND itemId = %1; "
                          "DELETE FROM items "
                          "WHERE itemId = %1; "
                          "COMMIT;").arg(id).arg(hallId));
    if(query.exec()){
        return true;
    }else{
        qDebug() << query.lastError().databaseText();
        return false;
    }
}

bool Database::updateHall(int id, QString name, QString address, int room)
{
    QSqlQuery query;
    query.prepare(QString ("UPDATE storageHalls "
                           "SET hallName = '%1', hallAddress = '%2', hallRoomNum = %3 "
                           "WHERE hallId = %4").arg(name).arg(address).arg(room).arg(id));
    if(query.exec()){
        return true;
    }else{
        qDebug() << query.lastError().databaseText();
        return false;
    }
}

bool Database::insertHall(QString name, QString address, int room)
{
    QSqlQuery query;
    query.prepare(QString ("INSERT INTO storageHalls(hallName, hallAddress, hallRoomNum) "
                           "VALUES ('%1', '%2', %3)").arg(name).arg(address).arg(room));
    if(query.exec()){
        return true;
    }else{
        qDebug() << query.lastError().databaseText();
        return false;
    }
}

bool Database::delHall(int id)
{
    QSqlQuery query;
    query.prepare(QString ("DELETE FROM storageHalls WHERE hallId = %1").arg(id));
    if(query.exec()){
        return true;
    }else{
        qDebug() << query.lastError().databaseText();
        return false;
    }
}

bool Database::createAcc(QString login, QString pass, int group, QString name, QString surname, QString middle)
{
    QSqlQuery query;
    query.prepare(QString ("INSERT INTO `users` (`userLogin`, `userPassword`, `userGroup`, `userName`, `userSurname`, `userFathername`) "
                           "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '')").arg(login).arg(hashPW(pass)).arg(group).arg(name).arg(surname).arg(middle));
    if(query.exec()){
        return true;
    }else{
        qDebug() << query.lastError().databaseText();
        return false;
    }
}

QString Database::hashPW(QString msg)
{
    QByteArray *spw = new QByteArray();
    QString ret;
    QDataStream out(spw,QIODevice::WriteOnly);
    out << msg;
    ret = QCryptographicHash::hash(*spw,QCryptographicHash::Sha3_256).toHex();

    return ret;
}
