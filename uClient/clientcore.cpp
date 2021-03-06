#include "clientcore.h"

ClientCore::ClientCore(QObject *parent) : QObject(parent)
{
    mainSocket = new QTcpSocket();

    mCPointer = static_cast<MainController*>(parent);

    connect(mainSocket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));

}

void ClientCore::onReadyRead()
{
    QDataStream in(mainSocket);
    while (mainSocket->bytesAvailable() > 0){
        if (blockSize == 0){
            if (mainSocket->bytesAvailable() < sizeof(quint16)){
                return;
            }
            in >> blockSize;
        }
        if (mainSocket->bytesAvailable() < blockSize){
            return;
        } else {
            blockSize = 0;
        }

        quint8 command = 0;
        in >> command;

        switch (command) {
        case ServerClient::succLoginComm:
        {
            emit this->successLogin();
            break;
        }
        case ServerClient::unSuccLoginComm:
        {
            emit this->unSuccessLogin();
            break;
        }
        case ServerClient::getHallsComm:
        {
            QString halls;
            in >> halls;
            QStringList hallsList = halls.split("/|/");
            for (int i = 0; i < hallsList.length(); i++)
            {
                QString iss = hallsList.at(i);
                QStringList hall = iss.split("|");

                QString rnum = hall.at(3);
                QString hid = hall.at(0);

                emit this->addHall(hid.toUInt(), hall.at(1), hall.at(2), rnum.toInt(), hall.at(4));
                //qDebug() << "hall " << hall.at(1);
            }

            //getItemsFromHall(mCPointer->getCurrHallId());

            break;
        }
        case ServerClient::getHallItems:
        {
            QString itms;
            in >> itms;

            //qDebug() << itms;

            QJsonDocument itemsDoc = QJsonDocument::fromJson(itms.toUtf8());

            QJsonObject items2parse = itemsDoc.object();

            QJsonArray itemsFromObj = items2parse["itemsArray"].toArray();

            for (int i = 0; i < itemsFromObj.size(); i++){
                QJsonObject newItem = itemsFromObj.at(i).toObject();
                addNewItem(newItem["itemId"].toInt(),
                        newItem["itemName"].toString(),
                        newItem["itemInventoryNum"].toString(),
                        newItem["itemGroup"].toInt(),
                        newItem["itemComment"].toString(),
                        newItem["itemHallId"].toInt(),
                        newItem["itemCount"].toInt(),
                        newItem["itemGroupName"].toString());
            }

            for(int j = 0; j < itemsInCurrHall.size(); j++){
                qDebug() << itemsInCurrHall.at(j)->getId() << itemsInCurrHall.at(j)->getName();
            }

            itemsToTable();
            ifNeedForGroups();

            break;
        }
        case ServerClient::getItemGroups:
        {
            emit this->groupAd(false);
            QString groups;
            in >> groups;

            QJsonDocument groupsDoc = QJsonDocument::fromJson(groups.toUtf8());
            QJsonObject groups2parse = groupsDoc.object();

            QJsonArray groupsFromObj = groups2parse["groupsArray"].toArray();

            groupsOfItem.clear();

            for (int i = 0; i < groupsFromObj.size(); i++){
                QJsonObject newGroup = groupsFromObj.at(i).toObject();

                groupItem* newGrp = new groupItem;

                newGrp->id = newGroup["groupId"].toInt();
                newGrp->name = newGroup["groupName"].toString();
                newGrp->comment = newGroup["groupComment"].toString();

                groupsOfItem.append(newGrp);

            }

            qDebug() << groups;
            groupsToTable();

            emit this->groupAd(true);

            break;
        }
        case ServerClient::succEditItem:
        {
            /*emit this->groupClear();
            ifNeedForGroups();
            break;*/
        }
        case ServerClient::succAddGroup:
        {
            /*emit this->groupClear();
            ifNeedForGroups();
            break;*/
        }
        case ServerClient::succDelGroup:
        {
            emit this->groupClear();
            ifNeedForGroups();
            break;
        }
        case ServerClient::succAddItem:
        {
        }
        case ServerClient::succModItem:
        {
        }
        case ServerClient::succDelItem:
        {
            mCPointer->cItTab();
            getItemsFromHall(mCPointer->getCurrHallId());
            break;
        }
        case ServerClient::succModHall:
        {
            QString itm;

            in >> itm;

            QJsonDocument itmDoc = QJsonDocument::fromJson(itm.toUtf8());
            QJsonObject newIt = itmDoc.object();

            emit this->updateCurHall(newIt["hallId"].toInt(),
                                    newIt["hallName"].toString(),
                                    newIt["hallAddr"].toString(),
                                    newIt["hallRoom"].toInt());
            break;
        }
        case ServerClient::succAddHall:
        {
        }
        case ServerClient::succDelHall:
        {
            QMessageBox::information(0 ,"Успех", "Для того чтобы увидеть изменения, перезакгрузите программу");
            break;
        }
        case ServerClient::getUsr:
        {
            QString itm;

            in >> itm;
            qDebug() << itm;

            QJsonDocument itmDoc = QJsonDocument::fromJson(itm.toUtf8());
            QJsonObject newIt = itmDoc.object();

            QJsonArray itemsFromObj = newIt["usersArray"].toArray();

            for (int i = 0; i < itemsFromObj.size(); i++){
                QJsonObject newItem = itemsFromObj.at(i).toObject();
            emit this->adUsr(newItem["userId"].toInt(),
                             newItem["userLogin"].toString(),
                             newItem["userPassword"].toString(),
                             newItem["userGroup"].toInt(),
                             newItem["userName"].toString(),
                             newItem["userSurname"].toString(),
                             newItem["userFathername"].toString());
            }
            qDebug() << "sig";
            break;
        }
        case ServerClient::succAddUsr:
        {

        }
        case ServerClient::succModUsr:
        {

        }
        case ServerClient::succDelUsr:
        {
            mCPointer->clusrs();
            ifNeedForUsers();
            break;
        }
        case ServerClient::permRestrict:
        {
            mCPointer->closAll();
            QMessageBox::warning(0,"Нет доступа","У вас недостаточно прав для выполнения этой команды");
            break;
        }
        case ServerClient::err:
        {
            QMessageBox::warning(0,"Ошибка","Невозможно выполнить данное действие");
            break;
        }
        default:
            break;
        }
    }
}

bool ClientCore::connectToServer(QString ip, quint16 port)
{
    if (!isConnected){
        mainSocket->connectToHost(QHostAddress(ip),SERVER_PORT);
        isConnected = true;

    }
    return isConnected;
}

void ClientCore::getItemsFromHall(int id)
{
    itemsInCurrHall.clear();
    QByteArray blck;
    QDataStream blOut(&blck, QIODevice::WriteOnly);

    blOut << id;

    this->sendBlock(ServerClient::getHallItems, &blck);
}

void ClientCore::login(QString ipAddr, QString login, QString password)
{
    if(connectToServer(ipAddr, SERVER_PORT)){
        QString blockPrep;
        blockPrep.append(login);
        blockPrep.append(" ");
        blockPrep.append(hashPW(password));

        QByteArray datablock;
        QDataStream in(&datablock, QIODevice::WriteOnly);

        in << blockPrep;

        this->sendBlock(ServerClient::loginComm,&datablock);
    } else {
        qDebug() << "cant connect";
    }
}

void ClientCore::sendBlock(quint8 command, QByteArray *data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << quint16(0);
    out << command;
    if (data!=NULL){
        block.append(*data);
    }

    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));

    mainSocket->write(block);
}

void ClientCore::addNewItem(int id, QString name, QString invNum, int grpId, QString comment, int hallId, int itmCount, QString grNme)
{
    storedItem* itm = new storedItem(id, name, invNum, grpId, comment, hallId, itmCount, grNme);

    itemsInCurrHall.push_back(itm);

    //itemsInHal.insert(id, itm);
}

void ClientCore::itemsToTable()
{
    if (itemsInCurrHall.size()>0){
        for (int i = 0; i < itemsInCurrHall.size(); i++){
            storedItem* itm = itemsInCurrHall.at(i);
            emit this->addItem(itm->getId(),
                               itm->getName(),
                               itm->getInvNumber(),
                               itm->getGroupName(),
                               itm->getComment(),
                               itm->getItemCount());
        }
    }
}

void ClientCore::editGroup(int id, QString name, QString comment)
{
    QJsonObject group2edit;

    group2edit["groupId"] = id;
    group2edit["groupName"] = name;
    group2edit["groupComment"] = comment;

    QJsonDocument groupDoc(group2edit);

    QString groupStr = groupDoc.toJson(QJsonDocument::Compact);

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);

    in << groupStr;

    this->sendBlock(ServerClient::editItemGroup, &block);

}

void ClientCore::ifNeedForGroups()
{
    this->sendBlock(ServerClient::getItemGroups, NULL);
    qDebug() << "nfg";
}

void ClientCore::groupsToTable()
{
    if(groupsOfItem.size()>0){
        qDebug() << "not gr null";
        for(int i = 0; i < groupsOfItem.size(); i++){
            groupItem* gr = groupsOfItem.at(i);
            emit this->addGroup(gr->id, gr->name, gr->comment);
        }
    }
    emit this->groupsToBox(&groupsOfItem);
}

void ClientCore::insertGroup(QString name, QString comment)
{
    QJsonObject insGr;

    insGr["groupName"] = name;
    insGr["groupComment"] = comment;

    QJsonDocument groupDoc(insGr);

    QString groupStr = groupDoc.toJson(QJsonDocument::Compact);

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);

    in << groupStr;

    this->sendBlock(ServerClient::addGroup, &block);
}

void ClientCore::deleteGroup(int id)
{
    QJsonObject gr2del;

    gr2del["groupId"] = id;

    QJsonDocument groupDoc(gr2del);

    QString groupStr = groupDoc.toJson(QJsonDocument::Compact);

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);

    in << groupStr;

    this->sendBlock(ServerClient::delGroup, &block);
}

void ClientCore::insertItem(int id, QString name, QString inv, QString grp, QString com, int count)
{
    int grNum = 0;
    for (int i = 0; i < groupsOfItem.size(); i++){
        if(groupsOfItem.at(i)->name == grp){
            grNum = groupsOfItem.at(i)->id;
            break;
        }
    }
    qDebug() << "insert item" << grNum;
    QJsonObject it2add;


    it2add["itemName"] = name;
    it2add["itemInventoryNum"] = inv;
    it2add["itemGroup"] = grNum;
    it2add["itemCount"] = count;
    it2add["itemComment"] = com;
    it2add["itemHallId"] = mCPointer->getCurrHallId();

    QJsonDocument itDoc(it2add);

    QString itStr = itDoc.toJson(QJsonDocument::Compact);

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);

    in << itStr;

    this->sendBlock(ServerClient::addItem, &block);
}

void ClientCore::editItem(int id, QString name, QString inv, QString grp, QString com, int count)
{
    int grNum = 0;
    for (int i = 0; i < groupsOfItem.size(); i++){
        if(groupsOfItem.at(i)->name == grp){
            grNum = groupsOfItem.at(i)->id;
            break;
        }
    }
    qDebug() << "edit item" << grNum;
    QJsonObject it2add;

    it2add["itemId"] = id;
    it2add["itemName"] = name;
    it2add["itemInventoryNum"] = inv;
    it2add["itemGroup"] = grNum;
    it2add["itemCount"] = count;
    it2add["itemComment"] = com;
    it2add["itemHallId"] = mCPointer->getCurrHallId();

    QJsonDocument itDoc(it2add);

    QString itStr = itDoc.toJson(QJsonDocument::Compact);

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);

    in << itStr;

    this->sendBlock(ServerClient::modItem, &block);
}

void ClientCore::deleteItem(int id)
{
    QJsonObject gr2del;

    gr2del["itemId"] = id;
    gr2del["hallId"] = mCPointer->getCurrHallId();

    QJsonDocument groupDoc(gr2del);

    QString groupStr = groupDoc.toJson(QJsonDocument::Compact);

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);

    in << groupStr;

    this->sendBlock(ServerClient::delItem, &block);
}

void ClientCore::editHall(int id, QString name, QString addr, int room)
{
    QJsonObject gr2del;

    gr2del["hallId"] = id;
    gr2del["hallName"] = name;
    gr2del["hallAddr"] = addr;
    gr2del["hallRoom"] = room;

    QJsonDocument groupDoc(gr2del);

    QString groupStr = groupDoc.toJson(QJsonDocument::Compact);

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);

    in << groupStr;

    this->sendBlock(ServerClient::modHall, &block);
}

void ClientCore::insertHall(QString name, QString addr, int room)
{
    QJsonObject gr2del;

    gr2del["hallName"] = name;
    gr2del["hallAddr"] = addr;
    gr2del["hallRoom"] = room;

    QJsonDocument groupDoc(gr2del);

    QString groupStr = groupDoc.toJson(QJsonDocument::Compact);

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);

    in << groupStr;

    this->sendBlock(ServerClient::addHall, &block);

}

void ClientCore::delHall(int id)
{
    QJsonObject gr2del;

    gr2del["hallId"] = id;

    QJsonDocument groupDoc(gr2del);

    QString groupStr = groupDoc.toJson(QJsonDocument::Compact);

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);

    in << groupStr;

    this->sendBlock(ServerClient::delHall, &block);
}

void ClientCore::createReport()
{
    QXlsx::Document xlsx;
    xlsx.write("A1","Склад");
    xlsx.write("B1",QString("%1").arg(mCPointer->getCurrHallName()));
    xlsx.write("A2", "Название");
    xlsx.write("B2", "Инв. номер");
    xlsx.write("C2", "Категория");
    xlsx.write("D2", "Комментарий");
    xlsx.write("E2", "Количество");

    for(int i = 0; i < itemsInCurrHall.size(); i++){
        xlsx.write(QString("A%1").arg(i+3), itemsInCurrHall.at(i)->getName());
        xlsx.write(QString("B%1").arg(i+3), itemsInCurrHall.at(i)->getInvNumber());
        xlsx.write(QString("C%1").arg(i+3), itemsInCurrHall.at(i)->getGroupName());
        xlsx.write(QString("D%1").arg(i+3), itemsInCurrHall.at(i)->getComment());
        xlsx.write(QString("E%1").arg(i+3), itemsInCurrHall.at(i)->itemCount);
    }

    xlsx.saveAs(mCPointer->getCurrHallName()+".xlsx");
}

QString ClientCore::hashPW(QString msg)
{
    QByteArray *spw = new QByteArray();
    QString ret;
    QDataStream out(spw,QIODevice::WriteOnly);
    out << msg;
    ret = QCryptographicHash::hash(*spw,QCryptographicHash::Sha3_256).toHex();

    return ret;
}

void ClientCore::ifNeedForUsers()
{
    qDebug() << "_________________==========NFU  NFU================__________________";
    this->sendBlock(ServerClient::getUsr, NULL);
}

void ClientCore::creAcc(QString log, QString pass, int grId, QString nam, QString sur, QString mid)
{
    QJsonObject gr2del;

    gr2del["userLogin"] = log;
    gr2del["userPassword"] = hashPW(pass);
    gr2del["userGroup"] = grId;
    gr2del["userName"] = nam;
    gr2del["userSurname"] = sur;
    gr2del["userFathername"] = mid;

    QJsonDocument groupDoc(gr2del);

    QString groupStr = groupDoc.toJson(QJsonDocument::Compact);

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);

    in << groupStr;

    this->sendBlock(ServerClient::addUsr, &block);
}

void ClientCore::modAcc(int id, QString log, QString pass, int grId, QString nam, QString sur, QString mid)
{
    QJsonObject gr2del;
    qDebug() << pass;

    gr2del["userId"] = id;
    gr2del["userLogin"] = log;
    if(pass!=""){
        gr2del["userPassword"] = hashPW(pass);
    } else {
        gr2del["userPassword"] = "";
    }
    gr2del["userGroup"] = grId;
    gr2del["userName"] = nam;
    gr2del["userSurname"] = sur;
    gr2del["userFathername"] = mid;

    QJsonDocument groupDoc(gr2del);

    QString groupStr = groupDoc.toJson(QJsonDocument::Compact);

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);

    in << groupStr;

    this->sendBlock(ServerClient::modUsr, &block);
}

void ClientCore::delAcc(int id)
{
    QJsonObject gr2del;

    gr2del["userId"] = id;

    QJsonDocument groupDoc(gr2del);

    QString groupStr = groupDoc.toJson(QJsonDocument::Compact);

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);

    in << groupStr;

    this->sendBlock(ServerClient::delUsr, &block);
}
