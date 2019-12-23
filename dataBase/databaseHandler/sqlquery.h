#ifndef SQLQUERY_H
#define SQLQUERY_H

#include <mqueue.h>
#include <string>
#include <string.h>
#include <vector>
#include <mqueue.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <vector>

#define MAX_MSG_LEN 10000
using namespace std;



struct userQuery
{
    string NickName;
    string PinCode;
    string Email;
    string PhoneNumber;
    string EmailPassword;
};

struct messageInfoQuery
{
    string UserMessage;
    string TargetPlatform;
    string dateToStart;
    int JumpTime;
};

struct faceQuery
{
    string PathDataset;
    int NumberOfImages;
};

struct fingerPrintQuery
{
    int FingerprintID;
    string FingerprintName;
};


struct fullUser
{
    userQuery user;
    messageInfoQuery messageInfo;
    faceQuery faceInfo;
    fingerPrintQuery fingerInfo;

};

class SqlQuery
{
public:
    SqlQuery();
    ~SqlQuery();
    bool insertUser(fullUser muser);
    bool insertUser(const userQuery user);
    bool insertFace(const string Nickname,const faceQuery faceInfo);
    bool insertFingerPrint(const string Nickname,const fingerPrintQuery fingerInfo);
    bool insertMessageInfo(const string Nickname,const messageInfoQuery messageInfo);
    bool deleteUser(const string Nickname);
    bool deleteFullUser(const string Nickname);
    bool deleteFace(const string Nickname);
    bool deleteFingerprint(const string Nickname);
    bool deleteMessage(const string Nickname);
    bool updateNumberOfImage(const string Nickname, const int newNumber);
    bool updateUserMessage(const string Nickname, const string newMessage);
    bool updateJumpTime(const string Nickname,const int newJumpTime);
    bool updatePlatformToRelease(const string Nickname,const string  newPlatform);
    bool updateDateToStart(const string Nickname, const string newDate);
    bool updateEmail(const string Nickname, const string newEmail);
    bool updateEmailPassword(const string Nickname,const string newPassword);
    bool updatePhoneNumber(const string Nickname,const string newPhoneNumber);
    bool getUser(string Nickname,fullUser *output);
    bool getUser(string Nickname,userQuery * user);
    bool getFace( string Nickname,faceQuery *faceInfo);
    bool getFingerprint( string Nickname, fingerPrintQuery *fingerInfo);
    bool getMessage( string Nickname, messageInfoQuery * messageInfo);
    vector<fullUser> getAllUsers();




private:
    bool selectQuery(const char*, const char*, const char*, const char*);
    bool selectQuery(const char*, const char*, const char*, int cond);
    bool selectQuery(const char*, const char*);
    bool selectQuery(const char*);
    bool deleteQuery(string,string,string);
    bool openQueryQueue();
    bool openCallbackQueue();
    bool closeQueryQueue();
    bool closeCallbackQueue();
    bool sendQuery(string);
    bool receiveQuery();

    void writeToLog(string Message);

    string getLastQueryResult();
    string selectQueryGetResponse(const char*, const char*, const char*, const char*);
    string selectQueryGetResponse(const char*, const char*, const char*, int);
    string selectQueryGetResponse(const char*, const char*);
    string messageLog;

    char query[MAX_MSG_LEN];
    char result[MAX_MSG_LEN];
    const char * msgq_query;
    const char * msgq_callback;
    mqd_t msgq_id_query;
    mqd_t msgq_id_callback;


    int fd;
};

#endif // SQLQUERY_H
