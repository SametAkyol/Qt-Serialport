#include "widget.h"

#include <QApplication>
#include <QSerialPort>
#include <QTextBrowser>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
