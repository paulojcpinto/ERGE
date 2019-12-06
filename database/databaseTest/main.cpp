#include <QCoreApplication>
#include "cquery.h"
#include <errno.h>
#include <string>
#include <sstream>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CQuery test;
    test.openQueryQueue();

    stringstream ss;
       ss << "insert into MYUSER values ('" << "Nome" << "','" << "1234" << "','" << "coisalinda@gmail.com" << "','" << "913753545"<<"','" << "12345" << ")";
       if(test.sendQuery(ss.str()))
       {
           cout << test.receiveQuery();
       }else cout << "error";
    return a.exec();
}
