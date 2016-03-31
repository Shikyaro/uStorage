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
            }

            break;
        }
        case editItemGroup:
        {
            QString itemGroup;

            in >> itemGroup;

            QJsonDocument itemGroupDoc = QJsonDocument::fromJson(itemGroup.toUtf8());
            QJsonObject group2edit = itemGroupDoc.object();

            if(!servPtr->updateGroups(group2edit["groupId"].toInt(),
                                     group2edit["groupName"].toString(),
                                     group2edit["groupComment"].toString()))
            {
                //eRRoR
            }else{
                this->sendBlock(succEditItem, NULL);
            }
            break;
        }
        case addGroup:
        {
            QString itemGroup;

            in >> itemGroup;

            QJsonDocument itemGroupDoc = QJsonDocument::fromJson(itemGroup.toUtf8());
            QJsonObject newGr = itemGroupDoc.object();

            if(!servPtr->insertGroup(newGr["groupName"].toString(),
                                     newGr["groupComment"].toString()))
            {
                //eRRoR
            }else{
                this->sendBlock(succAddGroup, NULL);
            }
            break;
        }
        case delGroup:
        {
            QString itemGroup;

            in >> itemGroup;

            QJsonDocument itemGroupDoc = QJsonDocument::fromJson(itemGroup.toUtf8());
            QJsonObject newGr = itemGroupDoc.object();

            if(!servPtr->deleteGroup(newGr["groupId"].toInt()))
            {
                //eRRoR
            }else{
                this->sendBlock(succDelGroup, NULL);
            }
            break;

        }
        case addItem:
        {
            //qDebug() << "addItem";
            QString itm;

            in >> itm;
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
            }else{
                this->sendBlock(succAddItem, NULL);
            }
            break;
        }
        case modItem:
        {
            QString itm;

            in >> itm;
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
            }else{
                this->sendBlock(succAddItem, NULL);
            }
            break;
        }
        case delItem:
        {
            QString itemGroup;

            in >> itemGroup;

            QJsonDocument itemGroupDoc = QJsonDocument::fromJson(itemGroup.toUtf8());
            QJsonObject newGr = itemGroupDoc.object();

            if(!servPtr->deleteItem(newGr["itemId"].toInt(), newGr["hallId"].toInt()))
            {
                //eRRoR
            }else{
                this->sendBlock(succDelItem, NULL);
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
