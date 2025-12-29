#include "ipwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ipwindow w;
    w.show();
    return a.exec();
}
