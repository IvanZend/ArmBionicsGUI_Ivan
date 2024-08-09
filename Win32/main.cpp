#include "emgwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/armbionics.icns"));
    EMGWidget w;
    w.show();
    return a.exec();
}
