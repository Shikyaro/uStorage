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

bool ServerCore::updateHall(int id, QString name, QString address, int room)
{
    return db->updateHall(id, name, address, room);
}

bool ServerCore::insertHall(QString name, QString address, int room)
{
    return db->insertHall(name, address, room);
}

bool ServerCore::delHall(int id)
{
    return db->delHall(id);
}

bool ServerCore::createAcc(QString login, QString pass, int group, QString name, QString surname, QString middle)
{
    return db->createAcc(login, pass, group, name, surname, middle);
}

bool ServerCore::modUser(int id, QString login, QString pass, int group, QString name, QString surname, QString middle)
{
    return db->modUser(id, login, pass, group, name, surname, middle);
}

bool ServerCore::delUser(int id)
{
    return db->delUser(id);
}

QString ServerCore::getUsers()
{
    return db->getUsers();
}

int ServerCore::getId(QString login)
{
    return db->getId(login);
}

perms ServerCore::getPerms(int id)
{
    return db->getPerms(id);
}
