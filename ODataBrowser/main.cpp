#include "MainWindow.h"
#include <QApplication>
#include <QLibrary>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(OakViewResources);

    QApplication a(argc, argv);
    QFont font = a.font();
    font.setPointSize(10);
    a.setFont(font);
    MainWindow w;
    w.show();

    return a.exec();
}
