#ifndef PROGRAMSCHEDULER_H
#define PROGRAMSCHEDULER_H

#include "userscheduler.h"

#define maxUsers 5

class ProgramScheduler
{

  private:

    UserScheduler usersScheduler[maxUsers];
    unsigned int userNumber;
    tm nextScheduler;

  public:

    ProgramScheduler ( void );
    void verifyReleaseTime( void );
    void verifyLasWarningTime( void );
    void addUser (  );
    void deleteUser ( string nickName );
    void updateNextScheduler ( void );
    ~ProgramScheduler ( void ) {}

};

#endif // PROGRAMSCHEDULER_H
