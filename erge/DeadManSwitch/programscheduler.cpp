#include "programscheduler.h"
#include <sstream>

#define LOGIN_SUCCESS 1
#define USER_NOT_FOUND 2
#define USER_BLOCKED  3
#define BAD_CREDENTIALS  4
using namespace  std;
ProgramScheduler::ProgramScheduler():log("ProgramScheduler: ")
{
  usersScheduler.clear();
  nextScheduler.tm_year = 0;
  loadData(mQuery.getAllUsers());
}

void ProgramScheduler::loadData(vector<fullUser> users)
{
    for(fullUser nUser: users)
    {
        time_t raw_time;
        struct tm *ptr_ts;
        tm nextScheduler;
        time ( &raw_time );
        ptr_ts = gmtime ( &raw_time );
        ptr_ts->tm_min++;
        usersScheduler.push_back(UserScheduler(*ptr_ts,nextScheduler,nUser));
        //usersScheduler.push_back(UserScheduler(*ptr_ts,nextScheduler,newUser.nickName,newUser.pinCode,newUser.phoneNumber,newUser.email,newUser.emailPassword,idfinger,newUser.messageToRelease,newUser.platformToRelease,true));



    }
}
bool ProgramScheduler::userParsingToFulluser(fullUser input, user_parsing *output)
{
    output->email=input.user.Email;
    output->pinCode=input.user.PinCode;
    output->nickName=input.user.NickName;
    output->repeatTime=input.messageInfo.JumpTime;
    output->dateToStart=input.messageInfo.dateToStart;
    output->phoneNumber=input.user.PhoneNumber;
    output->emailPassword=input.user.EmailPassword;
    output->messageToRelease=input.messageInfo.UserMessage;
    output->platformToRelease=input.messageInfo.TargetPlatform;
    return true;
}

bool ProgramScheduler::userParsingToFulluser(user_parsing input, fullUser *output)
{
    output->user.NickName=input.nickName;
    output->user.PinCode=input.pinCode;
    output->user.Email=input.email;
    output->user.EmailPassword=input.emailPassword;
    output->user.PhoneNumber=input.phoneNumber;
    output->messageInfo.JumpTime=input.repeatTime;
    output->messageInfo.UserMessage=input.messageToRelease;
    output->messageInfo.dateToStart=input.dateToStart;
    output->messageInfo.TargetPlatform=input.dateToStart;
    return true;
}

void ProgramScheduler::verifyReleaseTime( void )
{
  time_t raw_time;
  struct tm *ptr_ts;

  time ( &raw_time );
  ptr_ts = gmtime ( &raw_time );
  bool aux = false;
while(!aux)
  for ( uint8_t cont = 0; cont < usersScheduler.size(); cont ++)
    if(usersScheduler.at(cont).compareTimeRelease (*ptr_ts))
      { /*send signal */
        printf("okooko");
        aux = true;
      }
    else
    {
       time ( &raw_time );
       ptr_ts = gmtime ( &raw_time );
    }
}

void ProgramScheduler::addUser(user_parsing newUser)
{
  time_t raw_time;
  struct tm *ptr_ts;
  tm nextScheduler;
  time ( &raw_time );
  ptr_ts = gmtime ( &raw_time );
  ptr_ts->tm_min++;
  int idfinger=0;

  fullUser NewUser;
  userParsingToFulluser(newUser,&NewUser);

  NewUser.fingerInfo.FingerprintID=1;     //TODO getFingerprint here
  NewUser.fingerInfo.FingerprintName="FingerOne";
  //TODO getfaces here!!
  NewUser.faceInfo.PathDataset=newUser.nickName;
  NewUser.faceInfo.NumberOfImages=15;



  // TODO mudar para full user
  usersScheduler.push_back(UserScheduler(*ptr_ts,nextScheduler,newUser.nickName,newUser.pinCode,newUser.phoneNumber,newUser.email,newUser.emailPassword,idfinger,newUser.messageToRelease,newUser.platformToRelease,true));
  mQuery.insertUser(NewUser);
}

void ProgramScheduler::deleteUser( string nickName )
{
  for ( uint8_t count = 0; count < usersScheduler.size(); count++ )
    {
      if ( usersScheduler[count].compareUserNickName ( nickName ))
        {

          usersScheduler.erase(usersScheduler.begin()+count);
          stringstream message;
          message<<"Deleted User: "<<nickName;
          log.writeToLog(message.str());
          return;
        }
    }
}


UserScheduler* ProgramScheduler::finduser(string Nickname)
{
    for(int i=0; i<usersScheduler.size();i++)
    {
        stringstream ss;
        ss<<"log size: "<<usersScheduler.size();
        log.writeToLog(ss.str());
        if(usersScheduler[i].compareUserNickName(Nickname))
        {
            log.writeToLog("User fouded!");
            return &usersScheduler[i];

        }
    }

    return  nullptr;
}

int ProgramScheduler::login(string nickname, string pincode)
{
    UserScheduler* userLog = finduser(nickname);
    if(userLog != nullptr)
    {
        if(userLog->login(pincode))
            return LOGIN_SUCCESS;
        return BAD_CREDENTIALS;

    }return USER_NOT_FOUND;
}

void ProgramScheduler::updateNextScheduler ( void )
{
    struct tm timeAux;
    unsigned long int aux;
    unsigned long int aux2;
    if ( usersScheduler.size() )
    {

        nextScheduler = usersScheduler[0].getNextScheduler ();
        aux = nextScheduler.tm_min;
        aux += nextScheduler.tm_hour * 61;
        aux += nextScheduler.tm_yday * 61 * 25 ;
        aux += nextScheduler.tm_year * 61 * 25 * 367 ;

        for ( unsigned int count = 1; count < usersScheduler.size(); count ++ )
        {
            timeAux = usersScheduler[count].getNextScheduler ();
            aux2 = timeAux.tm_min;
            aux2 += timeAux.tm_hour * 61;
            aux2 += timeAux.tm_yday * 61 * 25 ;
            aux2 += timeAux.tm_year * 61 * 25 * 367 ;

            if ( aux2 < aux )
            {
                aux = aux2;
                nextScheduler = timeAux;
            }
        }
    }
}
