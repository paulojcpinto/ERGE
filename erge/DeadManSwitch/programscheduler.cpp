#include "programscheduler.h"

ProgramScheduler::ProgramScheduler()
{
  usersScheduler.clear();
  nextScheduler.tm_year = 0;
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

  usersScheduler.push_back(UserScheduler(*ptr_ts,nextScheduler,newUser.nickName,newUser.pinCode,newUser.phoneNumber,newUser.email,newUser.emailPassword,idfinger,newUser.messageToRelease,newUser.platformToRelease,true));
}

void ProgramScheduler::deleteUser( string nickName )
{
  for ( uint8_t count = 0; count < usersScheduler.size(); count++ )
    {
      if ( usersScheduler[count].compareUserNickName ( nickName ))
        {

          usersScheduler.erase(usersScheduler.begin()+count);
          return;
        }
    }
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
