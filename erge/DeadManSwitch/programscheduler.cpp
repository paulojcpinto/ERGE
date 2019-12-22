#include "programscheduler.h"

ProgramScheduler::ProgramScheduler()
{
  userNumber = 0;
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
  for ( uint8_t cont = 0; cont < userNumber; cont ++)
    if(usersScheduler[cont].compareTimeRelease (*ptr_ts))
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

void ProgramScheduler::addUser( void )
{
  time_t raw_time;
  struct tm *ptr_ts;
  userNumber ++ ;
  time ( &raw_time );
  ptr_ts = gmtime ( &raw_time );
  ptr_ts->tm_min++;
  UserScheduler ok(*ptr_ts);
  usersScheduler[0] = ok;
}

void ProgramScheduler::deleteUser( string nickName )
{
  for ( uint8_t count = 0; count < userNumber; count++ )
    {
      if ( usersScheduler[count].compareUserNickName ( nickName ))
        {
          usersScheduler[count] = usersScheduler[userNumber - 1];
          userNumber --;
          return;
        }
    }
}


void ProgramScheduler::updateNextScheduler ( void )
{
    struct tm timeAux;
    unsigned long int aux;
    unsigned long int aux2;
    if ( userNumber )
    {

        nextScheduler = usersScheduler[0].getNextScheduler ();
        aux = nextScheduler.tm_min;
        aux += nextScheduler.tm_hour * 61;
        aux += nextScheduler.tm_yday * 61 * 25 ;
        aux += nextScheduler.tm_year * 61 * 25 * 367 ;

        for ( unsigned int count = 1; count < userNumber; count ++ )
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
