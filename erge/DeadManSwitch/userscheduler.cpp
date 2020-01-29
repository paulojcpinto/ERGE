#include "userscheduler.h"
#include <QDebug>
#include <time.h>

#define numberDaysYear      366
#define numberHourDay       24
#define numberMinutesHour   60



UserScheduler::UserScheduler(tm jumpScheduler, tm nextScheduler, string nickName,string pinCode, string phoneNumber, string email, string password, unsigned int IDFinger, string message, string platform, bool deleteAfterRelease ) : user ( nickName,pinCode, phoneNumber, email, password, IDFinger, message, platform, deleteAfterRelease )
{
    this->jumpScheduler = jumpScheduler;
    this->nextScheduler = nextScheduler;

    this->preseceCheck = false;
}

UserScheduler::UserScheduler(tm jumpScheduler,tm nextScheduler,MCamera* cameraPointer,fullUser userData):user(cameraPointer,userData)
{
    this->jumpScheduler = jumpScheduler;
    this->nextScheduler = nextScheduler;
    this->repeatTime = userData.messageInfo.JumpTime;
    this->preseceCheck = false;
}



struct tm UserScheduler::getNextScheduler( void )
{
  return nextScheduler;
}

bool UserScheduler::login(string pincode)
{
    return  user.login(pincode);
}
/******************************************************************************************************
 *
 *
 * This Function will verify if it is time to do the release information
 * return true     ->   it is time to release the information
 * return false    ->   it is not time to release the information
 *
 *
 * ****************************************************************************************************/

bool UserScheduler::compareTimeRelease( tm timeNow )
{
 // printf ("\n %2d:%02d\n", timeNow.tm_min, this->nextScheduler.tm_min);8
        qDebug() <<"\n\nday "<<(nextScheduler.tm_mday)<<","<<timeNow.tm_mday<<"\n\nm "<<(nextScheduler.tm_min)<<","<<timeNow.tm_min<<"\n\n,o"<<(nextScheduler.tm_mon)<<","<<timeNow.tm_mon<<"\n\ny"<<(nextScheduler.tm_year)<<","<<timeNow.tm_year<<"\n\nh"<<(nextScheduler.tm_hour)<<","<<timeNow.tm_hour<<"\n\n";
    if (this->nextScheduler.tm_year < timeNow.tm_year)
        return true;
    else if (this->nextScheduler.tm_year == timeNow.tm_year+1900)
    {
        if ( this->nextScheduler.tm_mday < timeNow.tm_mday )
        {
            return true;
        }
        else if ( this->nextScheduler.tm_mday == timeNow.tm_mday )
        {
            if  ( this->nextScheduler.tm_hour < timeNow.tm_hour )
          {
              return true;
          }
          else  if  ( this->nextScheduler.tm_hour == timeNow.tm_hour )
          {
              if( this->nextScheduler.tm_min  <= timeNow.tm_min  )
              {
                  return true;
              }
          }
        }
    }
    return false;
}


/*******************************************************************************************************
 *
 *
 * This will update the next time that it is needed to verify if it is needed to share the information
 *
 *
 * *****************************************************************************************************/

void UserScheduler::updateNextSchedulerTime( void )
{
    int minutesIncrement = nextScheduler.tm_min + repeatTime;
    while(minutesIncrement)
    {
        if (minutesIncrement >= 60)
        {
            if (minutesIncrement + nextScheduler.tm_hour*60 >= 60*24)
            {
                nextScheduler.tm_mday ++;
                minutesIncrement -= 6*24;
            }
            else
            {
                nextScheduler.tm_hour ++;
                minutesIncrement -= 60;
            }
        }
        else
        {
            nextScheduler.tm_min = minutesIncrement;
            minutesIncrement -=60;
        }
    }
}


/****************************************************************************
 *
 *
 * This Compare the ID from a finger with the ID of the finger of the user
 * return true     ->   the finger ID's are the same
 * return false    ->   the finger ID's are not the same
 *
 *
 * *************************************************************************/

bool UserScheduler::compareUserFinger ( unsigned int id )
{
  return user.compareFinger ( id );
}


/****************************************************************************
 *
 *
 * This Compare the nick Name of the user with the one received
 * return true   ->  the nick Names match
 * return false  ->  the nick Names does not match
 *
 *
 * *************************************************************************/

bool UserScheduler::compareUserNickName ( string nickName )
{
  return user.compareNickName (nickName);
}

bool UserScheduler::createDataset(int *imagesTaked, bool *ended)
{
    return user.createDataset(imagesTaked,ended);
}

void UserScheduler::operator=(UserScheduler userSchedule)
{
  this->jumpScheduler = userSchedule.jumpScheduler;
  this->nextScheduler = userSchedule.nextScheduler;
  this->preseceCheck = userSchedule.preseceCheck;
  this->user = userSchedule.user;
}

user_parsing UserScheduler::getInfoToUpdate()
{
    user_parsing infoUpdate = user.getInfoToUpdate();
    infoUpdate.repeatTime = repeatTime;
    return infoUpdate;
}

bool UserScheduler::updateUser(user_parsing newInfo)
{
    repeatTime = newInfo.repeatTime;
    user.updateInfo(newInfo);
    return true;
}


void UserScheduler::setPresenceCheck( bool success )
{
    this->preseceCheck = success;

}
bool UserScheduler::appendImages(int *imagesTaked, bool *ended, int Amount)
{
    return user.appendImages(imagesTaked,ended,Amount);
}

bool UserScheduler::doRelease()
{
    if (!preseceCheck)
    {
        qDebug()<<"\n\n\n" <<  "release ok?"<<"\n\n\n";
       user.doRelease ();
        return true;
    }
    else
    {
       updateNextSchedulerTime ();
       return false;
    }
}

string UserScheduler::getNick()
{
    return user.getNick ();
}

bool UserScheduler::getpresenceCheck()
{
    return preseceCheck;
}
