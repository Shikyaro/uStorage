#ifndef USERWIDGET_H
#define USERWIDGET_H

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
#include <QComboBox>
#include <QDebug>

class UserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserWidget(QWidget *parent = 0);

signals:
    void adUsr(QString log, QString pass, int grId, QString nam, QString sur, QString mid);
    void modUsr(int id, QString log, QString pass, int grId, QString nam, QString sur, QString mid);
    void delUsr(int id);

public slots:
    void dataAd(bool d);
    void addRow(int id, QString log, QString pass, int grId, QString nam, QString sur, QString mid);

    void clear();

private:
    QTableWidget* userTable;
    QComboBox* grBox;
    QLineEdit* idL,*logL, *pwL, *namL, *surL, *midL;
    QPushButton* addU, *modU, *delU;

    bool datta = false;
private slots:
    void onSelectRow(int rowId);
    void onAdd();
    void onMod();
    void onDel();
};

#endif // USERWIDGET_H
