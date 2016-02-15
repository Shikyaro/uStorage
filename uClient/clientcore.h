#ifndef CLIENTCORE_H
#define CLIENTCORE_H


#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QCryptographicHash>
#include "../uServer/serverclient.h"

class ClientCore : public QObject
{
    Q_OBJECT
public:
    explicit ClientCore(QObject *parent = 0);

signals:
    void successConnect();
    void unSuccessConnect();

    void successLogin();
    void unSuccessLogin();

    void addHall(uint, QString , QString, int, QString);

public slots:
    bool connectToServer(QString ip, quint16 port);
    void login(QString ipAddr, QString login, QString password);
private:
    QTcpSocket* mainSocket;
    bool isConnected = false;
    void sendBlock(quint8 command, QByteArray *data);
    quint16 blockSize = 0;
private slots:
    void onReadyRead();
};

#endif // CLIENTCORE_H
