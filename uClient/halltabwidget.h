#ifndef HALLTABWIDGET_H
#define HALLTABWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

class HallTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HallTabWidget(uint nHallId,
                           QString nHallName,
                           QString nHallAddr,
                           int nHallRommNum,
                           QString nHallComment,
                           QWidget *parent = 0);

    uint getHallId(){return hallId;}

signals:

public slots:

private:
    uint hallId;
    QString hallName;
    QString hallAddr;
    int hallRommNum;
    QString hallComment;

    QTableWidget* itemTable;
    QGridLayout* mainLayout;
    QGroupBox* editButtonBox;

    QPushButton* addButton;
    QPushButton* deleteButton;
    QPushButton* editButton;

    QVBoxLayout* editButtonLayout;
};

#endif // HALLTABWIDGET_H
