#ifndef PROGRAMSCHEDULER_H
#define PROGRAMSCHEDULER_H
#include "userscheduler.h"
#include "loghandler.h"
#include "SqlModule/sqlquery.h"
#include<vector>

class ProgramScheduler
{

  private:


    LogHandler log;
    vector<UserScheduler> usersScheduler;
    UserScheduler* finduser(string Nickname);
    SqlQuery mQuery;
    user_parsing newUserInfo;
    void loadData(vector<fullUser> users);
    tm nextScheduler;
    MCamera mCamera;

  public:

    ProgramScheduler ( void );
    void verifyReleaseTime( void );
    void verifyLasWarningTime( void );
    void createUser(int *imagesTaked,bool *endedDataSet, bool *endedFingerPrint);
    void deleteUser ( string nickName );
    void updateNextScheduler ( void );
    int login(string nickname,string pincode);
    int addUser (user_parsing newUser);
    int getNumberofImages(String nickname);
    int getPrensenceCheckStatus(String nickname);
    bool updateFullUser(user_parsing updDateinfo, string Nickname);
    bool getUserForUpdate(String Nickname, user_parsing *output);
    bool getUserRecognizer(string Nickname,FRecognizer** output);
    bool doPresenceCheck(string Nickname);
    unsigned int getNewFingerID ( void );
    bool appendImageDataset(String nickname,int Amount,int *imagesTaked,bool *ended);
    static bool userParsingToFulluser(user_parsing input,fullUser *output);
    static bool userParsingToFulluser(fullUser input,user_parsing *output);
    string getNicknameByFinger ( unsigned int );
    MCamera* getCameraInstance(){return &mCamera;}
    ~ProgramScheduler ( void ) {}

};

#endif // PROGRAMSCHEDULER_H
