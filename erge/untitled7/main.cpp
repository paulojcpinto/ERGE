#include "cgsm.h"
#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   //Dialog w;
    CGSM P(9600,8963,1659 );
   // w.setWindowTitle("Temperature Sensor Reading");
   // w.setFixedSize(400,112);

 //   w.show();
    return a.exec();
}
