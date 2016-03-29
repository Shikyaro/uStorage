#include "storeditem.h"

storedItem::storedItem(int nId,
                       QString nNme,
                       QString nInNum,
                       int nGrId,
                       QString nCom,
                       int nHaId,
                       int nCount,
                       QString nGname,
                       QObject *parent) : QObject(parent)
{
    itemId = nId;
    itemName = nNme;
    itemGroupId = nGrId;
    invNumber = nInNum;
    itemComment = nCom;
    itemHallId = nHaId;
    itemCount = nCount;
    groupName = nGname;
}

void storedItem::setName(QString newName)
{
    itemName = newName;
    emit this->setNewName(this->itemId, itemName);
}

void storedItem::setInvNum(QString newInvNum)
{
    invNumber = newInvNum;
    emit this->setNewInvNum(this->itemId, invNumber);
}

