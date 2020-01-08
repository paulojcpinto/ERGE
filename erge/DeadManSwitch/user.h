#ifndef USER_H
#define USER_H

#include <string>
#include "finger.h"
#include "datainfo.h"

using namespace std;


class User
{

    private:

       string nickName;
       string phoneNumber;
       string pinCode;
       string email;
       string password;
       Finger finger;
       DataInfo dataInfo;     


    public:

        User();
        User( string nickName, string phoneNumber, string email, string password, unsigned int IDFinger, string message, string platform, bool deleteAfterRelease = true  );
        void updatePhoneNumeber ( string phoneNumber );
        void updateEMail ( string email );
        void updatePassword ( string password );
        bool releaseInformation ( void );
        bool compareNickName ( string nickName );
        bool compareFinger ( unsigned int id );
        ~User(){}

};



#endif // USER_H
