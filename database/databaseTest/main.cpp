#include <QCoreApplication>
#include "cquery.h"
#include <errno.h>
#include <string>
#include <sstream>
#include <iostream>
#include <signal.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CQuery test;


/*
    std::string sql = " CREATE TABLE IF NOT EXISTS MYUSER("
                          "Nickname TEXT PRIMARY KEY     NOT NULL, "
                          "pincode CHAR(4)    NOT NULL, "
                          "email TEXT    NOT NULL, "
                          "phone TEXT     NOT NULL, "
                          "pass TEXT      NOT NULL); "
                         ;
    cout<<"Result Was: "<<test.sendQuery(sql);
*/

    string query = "SELECT * FROM FACE;";
    cout << "STATE OF TABLE BEFORE INSERT" <<  test.sendQuery(query) << endl;
    //test.receiveQuery();
    cout<<"Reciebed: "<<test.getLastQueryResult();
     /* string sql("INSERT INTO MYUSER VALUES('Ola', '1234', 'olacoisa@gmail.com', '913753546', '12353232');"
                  "INSERT INTO FINGERPRINT VALUES(5, 'Dedo2', 'Ola');"
                  "INSERT INTO FACE VALUES(2, '/Datasets/test1', 'Ola');"
                  "INSERT INTO FACE VALUES(4, '/Datasets/test1', 'Ola');"); */
    string sql("INSERT INTO FACE VALUES(4, '/Datasets/test1', 'Ola');");

    test.sendQuery(sql);

    test.receiveQuery();
    test.sendQuery(query);
    test.receiveQuery();
    test.receiveQuery();

    cout<<"Result was: "<<test.getLastQueryResult()<<endl;






     //  ss << "insert into MYUSER values ('" << "Nome" << "','" << "1234" << "','" << "coisalinda@gmail.com" << "','" << "913753545"<<"','" << "12345" << ")";
      // if(test.sendQuery(ss.str()))
      // {
        //   cout<<"Worked!\n";
      // }else cout << "error";
    //return a.exec();

       return 0;
}
