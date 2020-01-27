#ifndef USERSCHEDULER_H
#define USERSCHEDULER_H

#include <ctime>
#include "user.h"
#include <string>


using namespace std;


class UserScheduler
{
private:

  tm jumpScheduler;
  tm nextScheduler;
  bool preseceCheck;
  int repeatTime;
  User user;

public:
  UserScheduler();
  UserScheduler(tm jumpScheduler, tm nextScheduler, string nickName,string pinCode, string phoneNumber, string email, string password, unsigned int IDFinger, string message, string platform, bool deleteAfterRelease );
  UserScheduler(tm nextScheduler);
  UserScheduler(tm jumpScheduler, tm nextScheduler,MCamera* cameraPointer,fullUser userData);
  struct tm getNextScheduler ( void );
  bool compareTimeRelease( tm timeNow );
  void updateNextSchedulerTime ( void );
  bool compareUserFinger ( unsigned int id );
  bool compareUserNickName ( string nickName );
  bool createDataset(int *imagesTaked,bool *ended);
  bool login(string pincode);
  bool updateUser(user_parsing newInfo);
  void setPresenceCheck( bool );
  bool appendImages(int *imagesTaked,bool *ended,int Amount);
  int getNumberofImages(){return user.getNumberofImages();};
  user_parsing getInfoToUpdate();
  bool doRelease();
  void operator=(UserScheduler userSchedule);
  ~UserScheduler(){}
};


#endif // USERSCHEDULER_H
