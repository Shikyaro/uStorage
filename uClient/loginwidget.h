#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QRegExp>

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = 0);

signals:
    void login(QString ipAddr, QString login, QString password);

public slots:
    void onUnSuccessLogin();
private slots:
    void onGoLogin();
    void onGoExit();
private:
    QLineEdit* ipEdit;
    QLineEdit* loginEdit;
    QLineEdit* passwordEdit;

    QPushButton* goLogin;
    QPushButton* goExit;
};

#endif // LOGINWIDGET_H
