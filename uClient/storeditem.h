#ifndef STOREDITEM_H
#define STOREDITEM_H

#include <QObject>
#include <QJsonObject>

class storedItem : public QObject
{
    Q_OBJECT
public:
    explicit storedItem(int nId,
                        QString nNme,
                        QString nInNum,
                        int nGrId,
                        QString nCom,
                        int nHaId,
                        int nCount,
                        QString nGname,
                        QObject *parent = 0);

    int getId(){return itemId;}
    QString getName(){return itemName;}
    QString getInvNumber(){return invNumber;}
    int getGroupId(){return itemGroupId;}
    QString getComment(){return itemComment;}
    int getHallId(){return itemHallId;}
    QString getGroupName(){return groupName;}
    int getItemCount(){return itemCount;}
    int itemCount;

    //static QJsonObject serializeToJson(storedItem* item);
    //static storedItem deserializeFromJson(QJsonObject* object);


    //void setId(int newId);
    void setName(QString newName);
    void setInvNum(QString newInvNum);
    //void setGroupId(int newGroupId);
    //void setComment(QString newComment);
    //void setHallId(int newHallId);

signals:
    void setNewName(int id, QString newName);
    void setNewInvNum(int id, QString newInvNum);

public slots:

private:
    int itemId;
    QString itemName;
    QString invNumber;
    int itemGroupId;
    QString itemComment;
    int itemHallId;

    QString groupName;

};

#endif // STOREDITEM_H
