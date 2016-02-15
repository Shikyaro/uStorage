#include "loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    QGridLayout* mainLayout = new QGridLayout(this);
    this->setLayout(mainLayout);

    ipEdit = new QLineEdit();
    ipEdit->setInputMask("000.000.000.000;_");

    loginEdit = new QLineEdit();
    loginEdit->setMaxLength(25);

    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMaxLength(25);

    mainLayout->addWidget(new QLabel("IP адрес: "), 0, 0);
    mainLayout->addWidget(new QLabel("Логин: "), 1, 0);
    mainLayout->addWidget(new QLabel("Пароль: "), 2, 0);

    mainLayout->addWidget(ipEdit, 0, 1);
    mainLayout->addWidget(loginEdit, 1, 1);
    mainLayout->addWidget(passwordEdit, 2, 1);

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    mainLayout->addLayout(buttonLayout, 3, 1);

    goLogin = new QPushButton("Вход");
    goExit =  new QPushButton("Выход");

    buttonLayout->addWidget(goLogin);
    buttonLayout->addWidget(goExit);

    this->setWindowFlags(Qt::CustomizeWindowHint);
    this->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowTitleHint);

    QRegExpValidator* userNameValid = new QRegExpValidator();
    QRegExp userNameReg("[A-Za-z0-9_]+$");
    userNameValid->setRegExp(userNameReg);

    loginEdit->setValidator(userNameValid);

    connect(goLogin,SIGNAL(clicked(bool)),this,SLOT(onGoLogin()));
    connect(goExit,SIGNAL(clicked(bool)),this,SLOT(onGoExit()));
}

void LoginWidget::onGoLogin()
{
    if(loginEdit->text().length()>0 &&
            passwordEdit->text().length()>0 &&
            ipEdit->text() != "..."){
        emit this->login(ipEdit->text(),loginEdit->text(),passwordEdit->text());
    } else {
        QMessageBox::warning(this,"Ошибка","Заполните все поля");
    }
}

void LoginWidget::onGoExit()
{
    this->close();
}

void LoginWidget::onUnSuccessLogin()
{
    QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль!");
}
