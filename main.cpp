#include "mainwindow.h"
#include "cextglobals.h"

#include <QApplication>
#include <QStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CextGlobals::APP = &a;
    CextGlobals::SystemWStyle = a.style()->name();
    MainWindow w;
    w.show();
    int retCode = a.exec();
    delete CextGlobals::APP;
    return retCode;
}
