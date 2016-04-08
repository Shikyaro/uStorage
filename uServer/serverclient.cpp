#include "serverclient.h"

ServerClient::ServerClient(qintptr socketDescriptor, ServerCore *server, QObject *parent) : QObject(parent)
{
    mainSocket = new QTcpSocket();

    mainSocket->setSocketDescriptor(socketDescriptor);

    connect(mainSocket,SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(mainSocket,SIGNAL(disconnected()), this, SLOT(onDisconnect()));

    servPtr = server;
}

ServerClient::~ServerClient()
{

}

void ServerClient::onReadyRead()
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
        case testComm:
        {
            qDebug() << "Success!";
            break;
        }
        case loginComm:
        {
            QString logPass;
            QStringList logp;
            QString halls;
            in >> logPass;
            logp = logPass.split(" ");

            if (servPtr->login(logp.at(0),logp.at(1))){
                isLoggedIn = true;
                uId = servPtr->getId(logp.at(0));
                qDebug() << "user " << logp.at(0) << "connected";
                this->sendBlock(succLoginComm, NULL);
                halls = servPtr->getHalls();
                if (halls!="NULL"){
                    QByteArray block;
                    QDataStream out(&block, QIODevice::WriteOnly);
                    out << halls;
                    this->sendBlock(getHallsComm, &block);
                }
            } else {
                this->sendBlock(unSuccLoginComm, NULL);
            }
            break;
        }
        case getHallItems:
        {
            int hallid;
            in >> hallid;
            QString itemsInHalls = servPtr->getItemsHalls(hallid);
            if (itemsInHalls!="NULL"){
                QByteArray block;
                QDataStream out(&block, QIODevice::WriteOnly);
                out << itemsInHalls;
                this->sendBlock(getHallItems, &block);
            }else{
                //eRRoR
                sendBlock(err, NULL);
            }
            break;
        }
        case getItemGroups:
        {
            QString itemGroups = servPtr->getItemGroups();
            if (itemGroups!="NULL"){
                QByteArray block;
                QDataStream out(&block, QIODevice::WriteOnly);
                out << itemGroups;
                this->sendBlock(getItemGroups, &block);
            }else{
                //eRRoR
                sendBlock(err, NULL);
            }

            break;
        }
        case editItemGroup:
        {
            QString itemGroup;

            in >> itemGroup;
            if(servPtr->getPerms(uId).items){


                QJsonDocument itemGroupDoc = QJsonDocument::fromJson(itemGroup.toUtf8());
                QJsonObject group2edit = itemGroupDoc.object();

                if(!servPtr->updateGroups(group2edit["groupId"].toInt(),
                                          group2edit["groupName"].toString(),
                                          group2edit["groupComment"].toString()))
                {
                    //eRRoR
                    sendBlock(err, NULL);
                }else{
                    this->sendBlock(succEditItem, NULL);
                }
            } else {
                sendBlock(permRestrict, NULL);
            }
            break;
        }
        case addGroup:
        {
            QString itemGroup;

            in >> itemGroup;
            if(servPtr->getPerms(uId).items){


                QJsonDocument itemGroupDoc = QJsonDocument::fromJson(itemGroup.toUtf8());
                QJsonObject newGr = itemGroupDoc.object();

                if(!servPtr->insertGroup(newGr["groupName"].toString(),
                                         newGr["groupComment"].toString()))
                {
                    //eRRoR
                    sendBlock(err, NULL);
                }else{
                    this->sendBlock(succAddGroup, NULL);
                }
            } else {
                sendBlock(permRestrict, NULL);
            }
            break;
        }
        case delGroup:
        {
            QString itemGroup;

            in >> itemGroup;
            if(servPtr->getPerms(uId).items){


                QJsonDocument itemGroupDoc = QJsonDocument::fromJson(itemGroup.toUtf8());
                QJsonObject newGr = itemGroupDoc.object();

                if(!servPtr->deleteGroup(newGr["groupId"].toInt()))
                {
                    //eRRoR
                    sendBlock(err, NULL);
                }else{
                    this->sendBlock(succDelGroup, NULL);
                }
            } else {
                sendBlock(permRestrict, NULL);
            }
            break;

        }
        case addItem:
        {
            QString itm;

            in >> itm;
            if(servPtr->getPerms(uId).items){
                //qDebug() << "addItem";

                qDebug() << itm;

                QJsonDocument itmDoc = QJsonDocument::fromJson(itm.toUtf8());
                QJsonObject newIt = itmDoc.object();

                if(!servPtr->insertItem(newIt["itemName"].toString(),
                                        newIt["itemInventoryNum"].toString(),
                                        newIt["itemGroup"].toInt(),
                                        newIt["itemCount"].toInt(),
                                        newIt["itemComment"].toString(),
                                        newIt["itemHallId"].toInt()))
                {
                    //eRRoR
                    sendBlock(err, NULL);
                }else{
                    this->sendBlock(succAddItem, NULL);
                }
            } else {
                sendBlock(permRestrict, NULL);
            }
            break;
        }
        case modItem:
        {
            QString itm;

            in >> itm;
            if(servPtr->getPerms(uId).items){

                qDebug() << itm;

                QJsonDocument itmDoc = QJsonDocument::fromJson(itm.toUtf8());
                QJsonObject newIt = itmDoc.object();

                if(!servPtr->updateItem(newIt["itemId"].toInt(),
                                        newIt["itemName"].toString(),
                                        newIt["itemInventoryNum"].toString(),
                                        newIt["itemGroup"].toInt(),
                                        newIt["itemCount"].toInt(),
                                        newIt["itemComment"].toString(),
                                        newIt["itemHallId"].toInt()))
                {
                    //eRRoR
                    sendBlock(err, NULL);
                }else{
                    this->sendBlock(succAddItem, NULL);
                }
            } else {
                sendBlock(permRestrict, NULL);
            }
            break;
        }
        case delItem:
        {
            QString itemGroup;

            in >> itemGroup;
            if(servPtr->getPerms(uId).items){


                QJsonDocument itemGroupDoc = QJsonDocument::fromJson(itemGroup.toUtf8());
                QJsonObject newGr = itemGroupDoc.object();

                if(!servPtr->deleteItem(newGr["itemId"].toInt(), newGr["hallId"].toInt()))
                {
                    //eRRoR
                    sendBlock(err, NULL);
                }else{
                    this->sendBlock(succDelItem, NULL);
                }
            } else {
                sendBlock(permRestrict, NULL);
            }
            break;
        }
        case modHall:
        {
            QString itm;

            in >> itm;
            if(servPtr->getPerms(uId).halls){

                qDebug() << itm;

                QJsonDocument itmDoc = QJsonDocument::fromJson(itm.toUtf8());
                QJsonObject newIt = itmDoc.object();

                if(!servPtr->updateHall(newIt["hallId"].toInt(),
                                        newIt["hallName"].toString(),
                                        newIt["hallAddr"].toString(),
                                        newIt["hallRoom"].toInt()))

                {
                    //eRRoR
                    sendBlock(err, NULL);
                }else{
                    QByteArray block;
                    QDataStream out(&block, QIODevice::WriteOnly);
                    out << itm;

                    this->sendBlock(succModHall, &block);
                }
            } else {
                sendBlock(permRestrict, NULL);
            }
            break;
        }
        case addHall:
        {
            QString itm;

            in >> itm;
            if(servPtr->getPerms(uId).halls){

                qDebug() << itm;

                QJsonDocument itmDoc = QJsonDocument::fromJson(itm.toUtf8());
                QJsonObject newIt = itmDoc.object();

                if(!servPtr->insertHall(newIt["hallName"].toString(),
                                        newIt["hallAddr"].toString(),
                                        newIt["hallRoom"].toInt()))

                {
                    //eRRoR
                    sendBlock(err, NULL);
                }else{
                    this->sendBlock(succAddHall, NULL);
                }
            } else {
                sendBlock(permRestrict, NULL);
            }
            break;
        }
        case delHall:
        {
            QString itm;

            in >> itm;
            if(servPtr->getPerms(uId).halls){

                qDebug() << itm;

                QJsonDocument itmDoc = QJsonDocument::fromJson(itm.toUtf8());
                QJsonObject newIt = itmDoc.object();

                if(!servPtr->delHall(newIt["hallId"].toInt()))

                {
                    //eRRoR
                    sendBlock(err, NULL);
                }else{
                    this->sendBlock(succDelHall, NULL);
                }
            } else {
                sendBlock(permRestrict, NULL);
            }
            break;
        }
        case getUsr:
        {
            qDebug() << "_______________________GUSR_____________________";
            if(servPtr->getPerms(uId).usr){
                QString itemGroups = servPtr->getUsers();
                if (itemGroups!="NULL"){
                    QByteArray block;
                    QDataStream out(&block, QIODevice::WriteOnly);
                    out << itemGroups;
                    this->sendBlock(getUsr, &block);
                }else{
                    //eRRoR
                    sendBlock(err, NULL);
                }
            } else {
                sendBlock(permRestrict, NULL);
            }
            break;
        }
        case addUsr:
        {
            QString itm;

            in >> itm;
            if(servPtr->getPerms(uId).usr){

                qDebug() << itm;

                QJsonDocument itmDoc = QJsonDocument::fromJson(itm.toUtf8());
                QJsonObject newIt = itmDoc.object();

                if(!servPtr->createAcc(newIt["userLogin"].toString(),
                                       newIt["userPassword"].toString(),
                                       newIt["userGroup"].toInt(),
                                       newIt["userName"].toString(),
                                       newIt["userSurname"].toString(),
                                       newIt["userFathername"].toString()))


                {
                    //eRRoR
                    sendBlock(err, NULL);
                }else{
                    this->sendBlock(succAddUsr, NULL);
                }
            } else {
                sendBlock(permRestrict, NULL);
            }
            break;
        }
        case modUsr:
        {
            QString itm;

            in >> itm;
            if(servPtr->getPerms(uId).usr){

                qDebug() << itm;

                QJsonDocument itmDoc = QJsonDocument::fromJson(itm.toUtf8());
                QJsonObject newIt = itmDoc.object();

                if(!servPtr->modUser(  newIt["userId"].toInt(),
                                       newIt["userLogin"].toString(),
                                       newIt["userPassword"].toString(),
                                       newIt["userGroup"].toInt(),
                                       newIt["userName"].toString(),
                                       newIt["userSurname"].toString(),
                                       newIt["userFathername"].toString()))


                {
                    //eRRoR
                    sendBlock(err, NULL);
                }else{
                    this->sendBlock(succModUsr, NULL);
                }
            } else {
                sendBlock(permRestrict, NULL);
            }
            break;
        }
        case delUsr:
        {
            QString itm;

            in >> itm;
            if(servPtr->getPerms(uId).usr){

                qDebug() << itm;

                QJsonDocument itmDoc = QJsonDocument::fromJson(itm.toUtf8());
                QJsonObject newIt = itmDoc.object();

                if(!servPtr->delUser(newIt["userId"].toInt()))


                {
                    //eRRoR
                    sendBlock(err, NULL);
                }else{
                    this->sendBlock(succDelUsr, NULL);
                }
            } else {
                sendBlock(permRestrict, NULL);
            }
            break;
        }
        default:
            break;
        }
    }
}

void ServerClient::sendBlock(quint8 command, QByteArray *data)
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

void ServerClient::onDisconnect()
{

}
