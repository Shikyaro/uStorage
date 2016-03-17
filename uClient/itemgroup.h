#ifndef ITEMGROUP_H
#define ITEMGROUP_H

#include <QObject>

class itemGroup : public QObject
{
    Q_OBJECT
public:
    explicit itemGroup(QObject *parent = 0);

    //void setId(int id);
    void setName(QString name);
    void setComment(QString comment);

    int getId(){return groupId;}
    QString getName(){return groupName;}
    QString getComment(){return groupComment;}



signals:

    void newId(int, int);
    void newName(int, QString);
    void newComment(int, QString);

public slots:

private:
    int groupId;
    QString groupName;
    QString groupComment;
};

#endif // ITEMGROUP_H
