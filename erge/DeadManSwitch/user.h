#ifndef USER_H
#define USER_H

#include <string>
#include "finger.h"
#include "face.h"
#include "datainfo.h"
#include "SqlModule/sqlquery.h"
#include "loghandler.h"
using namespace std;



struct user_parsing
{
    string nickName;
    string pinCode;
    string phoneNumber;
    string email;
    string emailPassword;
    string messageToRelease;
    int repeatTime;
    string dateToStart;
    string platformToRelease;

};


class User
{

    private:

       string nickName;
       string phoneNumber;
       string pinCode;
       string email;
       string password;
       Finger finger;
       Face face;
       DataInfo dataInfo;
       LogHandler log;



    public:
        User( string nickName,string pinCode, string phoneNumber, string email, string password, unsigned int IDFinger, string message, string platform, bool deleteAfterRelease = true  );
        User(MCamera* cameraPointer,fullUser Data);
        void updatePhoneNumeber ( string phoneNumber );
        void updateEMail ( string email );
        void updatePassword ( string password );
        bool releaseInformation ( void );
        bool compareNickName ( string nickName );
        bool compareFinger ( unsigned int id );
        bool createDataset(int *imagesTaked,bool *ended);
        bool login(string pincode);
        bool updateInfo(user_parsing newInfo);
        user_parsing getInfoToUpdate();
        ~User(){}

};



#endif // USER_H
