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
  User user;

public:
  UserScheduler();
  UserScheduler(tm jumpScheduler, tm nextScheduler, string nickName, string phoneNumber, string email, string password, unsigned int IDFinger, string message, string platform, bool deleteAfterRelease );
  UserScheduler(tm nextScheduler);
  struct tm getNextScheduler ( void );
  bool compareTimeRelease( tm timeNow );
  void updateNextSchedulerTime ( void );
  bool compareUserFinger ( unsigned int id );
  bool compareUserNickName ( string nickName );
  void operator=(UserScheduler userSchedule);
  ~UserScheduler(){}
};


#endif // USERSCHEDULER_H