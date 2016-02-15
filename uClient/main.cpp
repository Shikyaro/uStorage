#include <maincontroller.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainController *mainCont = new MainController();

    mainCont->showInterface();


    return a.exec();
}
