#ifndef PROGRAMSCHEDULER_H
#define PROGRAMSCHEDULER_H
#include "userscheduler.h"
#include<vector>

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

class ProgramScheduler
{

  private:

    vector<UserScheduler> usersScheduler;
    tm nextScheduler;

  public:

    ProgramScheduler ( void );
    void verifyReleaseTime( void );
    void verifyLasWarningTime( void );
    void addUser (user_parsing newUser);
    void deleteUser ( string nickName );
    void updateNextScheduler ( void );
    ~ProgramScheduler ( void ) {}

};

#endif // PROGRAMSCHEDULER_H
