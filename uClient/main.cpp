#include <maincontroller.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");
    paths.append("sqldrivers");
    QCoreApplication::setLibraryPaths(paths);

    QApplication a(argc, argv);

    MainController *mainCont = new MainController();

    mainCont->showInterface();




    return a.exec();
}
