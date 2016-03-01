#ifndef STOREDITEM_H
#define STOREDITEM_H

#include <QObject>

class storedItem : public QObject
{
    Q_OBJECT
public:
    explicit storedItem(QObject *parent = 0);



signals:

public slots:

private:
    int itemId;
    QString itemName;
    QString invNumber;
    int itemGroupId;
    QString itemComment;
    int itemHallId;

};

#endif // STOREDITEM_H
