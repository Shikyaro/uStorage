#include "clientcore.h"

ClientCore::ClientCore(QObject *parent) : QObject(parent)
{
    mainSocket = new QTcpSocket();

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
                qDebug() << "hall " << hall.at(1);

            }
            QByteArray blck;
            QDataStream blOut(&blck, QIODevice::WriteOnly);

            blOut << (int)1;

            this->sendBlock(ServerClient::getHallItems, &blck);

            break;
        }
        case ServerClient::getHallItems:
        {
            QString itms;
            in >> itms;

            qDebug() << itms;

            QJsonDocument itemsDoc = QJsonDocument::fromJson(itms.toUtf8());


            QJsonObject items2parse = itemsDoc.object();


            QJsonArray itemsFromObj = items2parse["itemsArray"].toArray();

            for (int i = 0; i < itemsFromObj.size(); i++){
                QJsonObject newItem = itemsFromObj.at(i).toObject();

                //storedItem* storItem = new storedItem(newItem["itemId"].toInt(),newItem["itemName"].toString());
                addNewItem(newItem["itemId"].toInt(),
                        newItem["itemName"].toString(),
                        newItem["itemInventoryNum"].toString(),
                        newItem["itemGroup"].toInt(),
                        newItem["itemGroupName"].toString(),
                        newItem["itemHallId"].toInt(),
                        newItem["itemCount"].toInt());
            }

            for(int j = 0; j < itemsInCurrHall.size(); j++){
                qDebug() << itemsInCurrHall.at(j)->getId() << itemsInCurrHall.at(j)->getName();
            }

            itemsToTable();

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

void ClientCore::login(QString ipAddr, QString login, QString password)
{
    if(connectToServer(ipAddr, SERVER_PORT)){
        QString blockPrep;
        blockPrep.append(login);
        blockPrep.append(" ");
        blockPrep.append(password);

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
    //out << *data;
    if (data!=NULL){
        block.append(*data);
    }

    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));

    mainSocket->write(block);
}

void ClientCore::addNewItem(int id, QString name, QString invNum, int grpId, QString comment, int hallId, int itmCount)
{
    storedItem* itm = new storedItem(id, name, invNum, grpId, comment, hallId, itmCount);

    itemsInCurrHall.push_back(itm);

    itemsInHal.insert(id, itm);
}

void ClientCore::itemsToTable()
{
    if (itemsInCurrHall.size()>0){
        for (int i = 0; i < itemsInCurrHall.size(); i++){
            storedItem* itm = itemsInCurrHall.at(i);
            emit this->addItem(itm->getId(), itm->getName(), itm->getInvNumber(), itm->getGroupId(), itm->getComment(), itm->getItemCount());

        }
    }
}
