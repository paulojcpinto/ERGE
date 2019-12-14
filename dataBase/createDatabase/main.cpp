#include <QCoreApplication>
#include <sys/syslog.h>
#include <sys/types.h>
#include <signal.h>
#include <mqueue.h>
#include "querymodule.h"
#define  PROGRAM_NAME "DatabaseCreation: "

using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QueryModule q;

    string tableMYUSER = " CREATE TABLE IF NOT EXISTS MYUSER("
                          "Nickname TEXT PRIMARY KEY     NOT NULL, "
                          "pincode CHAR(4)    NOT NULL, "
                          "email TEXT    NOT NULL, "
                          "phone TEXT     NOT NULL, "
                          "pass TEXT      NOT NULL); ";

    string tableFingerprint= "CREATE TABLE IF NOT EXISTS FINGERPRINT("
                              "fingerprintID INTEGER PRIMARY KEY NOT NULL,"
                              "nameFingerprint TEXT NOT NULL,"
                              "Nickname TEXT,"
                              " FOREIGN KEY (Nickname) references MYUSER(Nickname) on update cascade on delete set null);";

    string tableFace= "CREATE TABLE IF NOT EXISTS FACE("
                      "faceID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "DataSetPath TEXT NOT NULL,"
                      "Nickname TEXT,"
                      "FOREIGN KEY (Nickname) references MYUSER(Nickname) on update cascade on delete set null);";

    string tableMessageInfo = "CREATE TABLE IF NOT EXISTS MESSAGEINFO("
                         "messageInfoID INTEGER PRIMARY KEY AUTOINCREMENT"
                         "userMessage TEXT,"
                         "targetPlatform TEXT,"
                         "targetPlatform TEXT,"
                         "jumpTime INTEGER,"
                         "Nickname TEXT,"
                         "FOREIGN KEY (Nickname) references MYUSER(Nickname) on update cascade on delete set null);";


    q.sendQuery(tableMYUSER);
    q.sendQuery(tableFingerprint);
    q.sendQuery(tableFace);
    q.sendQuery(tableMessageInfo);


    return 0;
}
