#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDebug>

class GroupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupWidget(QWidget *parent = 0);

signals:
    void onAddClicked(QString, QString);
    void onModClicked(int, QString, QString);
    void onDelClicked(int);
public slots:
    void addGroup(int id, QString name, QString com);
    void clearGroups();
    void dataAD(bool);

private:
    QTableWidget* groupTable;
    QPushButton* addGr, *modGr, *delGr;
    QLineEdit* grNum, *grName, *grComm;
    bool dataAdded = false;
private slots:
    void onAdd();
    void onMod();
    void onDel();

    void onSelect(int);

};

#endif // GROUPWIDGET_H
