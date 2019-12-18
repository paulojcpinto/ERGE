#include <QCoreApplication>
#include "bluetooth_module.h"
#include <QtWidgets/qapplication.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cout<<"Hello";
    bluetooth_module b;
    //cout<<"Hello";

    a.exec();

    return 0;
}
