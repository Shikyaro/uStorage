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

QString ServerCore::getItemGroups()
{
    return db->getItemTypes();
}

bool ServerCore::updateGroups(int id, QString name, QString comment)
{
    return db->updateGroup(id, name, comment);
}

bool ServerCore::insertGroup(QString name, QString comment)
{
    return db->insertGroup(name, comment);
}

bool ServerCore::deleteGroup(int id)
{
    return db->deleteGroup(id);
}

bool ServerCore::insertItem(QString name, QString inv, int grp, int count, QString comment, int hallId)
{
    return db->insertItem(name, inv, grp, count, comment, hallId);
}

bool ServerCore::updateItem(int id, QString name, QString inv, int grp, int count, QString comment, int hallId)
{
    return db->updateItem(id, name, inv, grp, count, comment, hallId);
}

bool ServerCore::deleteItem(int id, int hallId)
{
    return db->deleteItem(id, hallId);
}
