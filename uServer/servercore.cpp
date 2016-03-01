#include "servercore.h"

ServerCore::ServerCore(QObject *parent) : QTcpServer(parent)
{
    db = new Database();
    startServer();

}

ServerCore::~ServerCore()
{

}

void ServerCore::incomingConnection(qintptr handle)
{
    ServerClient* newClient = new ServerClient(handle, this, this);

    clientList.append(newClient);
    qDebug() << "new client";
}

void ServerCore::startServer()
{
    if (!this->isListening()){
        if (db->initialize("127.0.0.1","ustorage","root","alcpvc")){
            this->listen(QHostAddress::Any, SERVER_PORT);
            qDebug() << "server started";
        }
        else
            qDebug() << "cant connect to db";
    }

}

bool ServerCore::login(QString username, QString password)
{
    return db->login(username,password);
}

QString ServerCore::getHalls()
{
    return db->getHalls();
}

QString ServerCore::getItemsHalls(int halid)
{
    return db->getItemsHalls(halid);
}
