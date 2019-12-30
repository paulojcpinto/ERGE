#ifndef PROGRAMSCHEDULER_H
#define PROGRAMSCHEDULER_H
#include "userscheduler.h"
#include "loghandler.h"
#include "SqlModule/sqlquery.h"
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

    LogHandler log;
    vector<UserScheduler> usersScheduler;
    UserScheduler* finduser(string Nickname);
    SqlQuery mQuery;
    void loadData(vector<fullUser> &users);
    tm nextScheduler;

  public:

    ProgramScheduler ( void );
    void verifyReleaseTime( void );
    void verifyLasWarningTime( void );
    void addUser (user_parsing newUser);
    void deleteUser ( string nickName );
    void updateNextScheduler ( void );
    int login(string nickname,string pincode);
    bool updateFullUser(user_parsing updDateinfo, string Nickname);
    static bool userParsingToFulluser(user_parsing input,fullUser *output);
    static bool userParsingToFulluser(fullUser input,user_parsing *output);

    ~ProgramScheduler ( void ) {}

};

#endif // PROGRAMSCHEDULER_H
