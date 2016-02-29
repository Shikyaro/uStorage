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
            QJsonObject* itemsInHalls = servPtr->getItemsHalls(hallid);
            if (itemsInHalls!=NULL){
                QByteArray block;
                QDataStream out(&block, QIODevice::WriteOnly);
                out << itemsInHalls;
                this->sendBlock(getHallItems, &block);
            }else{
                //eRRoR
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
