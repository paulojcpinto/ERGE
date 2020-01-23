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
        usersScheduler.push_back(UserScheduler(*ptr_ts,nextScheduler,&mCamera,nUser));
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
    output->messageInfo.TargetPlatform=input.platformToRelease;
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

int ProgramScheduler::addUser(user_parsing newUser)
{
    if(finduser(newUser.nickName)==nullptr)
    {

        newUserInfo=newUser;
        return 1;
    }
    return 2;
}

void ProgramScheduler::createUser(int *imagesTaked, bool *endedDataSet, bool *endedFingerPrint)
{

    time_t raw_time;
    struct tm *ptr_ts;
    tm nextScheduler;
    time ( &raw_time );
    ptr_ts = gmtime ( &raw_time );
    ptr_ts->tm_min++;
    fullUser NewUser;
    userParsingToFulluser(newUserInfo,&NewUser);

    log.writeToLog("Deu ate aqui!");
    NewUser.fingerInfo.FingerprintID=2;     //TODO getFingerprint here
    NewUser.fingerInfo.FingerprintName="FingerOne";
    *endedFingerPrint=true;
    log.writeToLog("Deu ate aqui!2");
    //TODO getfaces here!!

    NewUser.faceInfo.PathDataset=newUserInfo.nickName;
    NewUser.faceInfo.NumberOfImages=15;
    usersScheduler.push_back(UserScheduler(*ptr_ts,nextScheduler,&mCamera,NewUser));

    finduser(NewUser.user.NickName)->createDataset(imagesTaked,endedDataSet);





    // TODO mudar para full user
    mQuery.insertUser(NewUser);
}



void ProgramScheduler::deleteUser( string nickName )
{
  for ( uint8_t count = 0; count < usersScheduler.size(); count++ )
    {
      if ( usersScheduler[count].compareUserNickName ( nickName ))
        {

          usersScheduler.erase(usersScheduler.begin()+count);
          mQuery.deleteUser(nickName);
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
        ss<<"Users number: "<<usersScheduler.size();
        log.writeToLog(ss.str());
        if(usersScheduler[i].compareUserNickName(Nickname))
        {
            ss.clear();
            ss<<"User Founded: "<<Nickname<<" at position: "<<i;
            log.writeToLog(ss.str());
            return &usersScheduler.at(i);

        }

    }

    stringstream ss;
    ss<<"User "<<Nickname<<" Not founded";
    log.writeToLog(ss.str());
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



bool ProgramScheduler::getUserForUpdate(String Nickname, user_parsing *output)
{
    user_parsing userToUpdate;
    UserScheduler *aux =  finduser(Nickname);
    if(aux != nullptr)
    {
    userToUpdate = aux->getInfoToUpdate();
    *output = userToUpdate;
    log.writeToLog("User information getted.");
    return true;
    }return false;

}
<<<<<<< Updated upstream
bool ProgramScheduler::updateFullUser(user_parsing updDateinfo, string Nickname)

{
    UserScheduler *aux = finduser(Nickname);
    if(aux != nullptr)
    {
        aux->updateUser(updDateinfo);
        mQuery.updateEmail(Nickname,updDateinfo.email);
        mQuery.updateJumpTime(Nickname,updDateinfo.repeatTime);
        mQuery.updatePhoneNumber(Nickname,updDateinfo.phoneNumber);
        mQuery.updateUserMessage(Nickname,updDateinfo.messageToRelease);
        mQuery.updateEmailPassword(Nickname,updDateinfo.emailPassword);
        mQuery.updatePlatformToRelease(Nickname,updDateinfo.platformToRelease);
        log.writeToLog("Update done With success");
        return true;
    }
    return false;
=======


unsigned int ProgramScheduler::getNewFingerID( void )
{
    bool FreeID = false;
    for (int count = 1; count < 100; count ++)
    {
        for ( UserScheduler ok : usersScheduler )
        {
            if ( ok.compareUserFinger ( count ))
                FreeID = true;
        }
        if (FreeID)
        {
            return count;
        }
    }
    return 0;
}

void ProgramScheduler::doPresenceCheck(unsigned int fingerID)
{
     for ( UserScheduler ok : usersScheduler )
     {
         if (ok.compareUserFinger (fingerID))
         {
             ok.setPresenceCheck (true);
             return ;
         }
     }
>>>>>>> Stashed changes

}

bool ProgramScheduler::appendImageDataset(String nickname,int Amount,int *imagesTaked,bool *ended)
{
   bool  aux= finduser(nickname)->appendImages(imagesTaked,ended,Amount);
   if(aux)
   {
       mQuery.updateNumberOfImage(nickname,*imagesTaked);
       log.writeToLog("Added Images to dataSet with success");
       return true;
   }
   return false;

}
int ProgramScheduler::getNumberofImages(String nickname)
{
    return finduser(nickname)->getNumberofImages();
}
