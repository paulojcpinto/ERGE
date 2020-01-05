#ifndef __message_H
#define __message_H

#include <time.h>

typedef struct Message message;


struct Message
{
	char messageToRelease[255];
	int repeatTime;
	char dateToStart[10];
	char platformToRelease[50];
	struct tm dateToRelease1;
	
  int   (*updateMessageToRelease)(char*,message*);
	int   (*updateRepeatTime)	(int ,message*);
	int   (*updateDateToRelease) (char*,message*);
	int   (*updatePlatformToRelease) (char*, message*);
	
};

extern int updateMessageToRelease(char* mMessageRelease, message* mMessage);

extern int updateRepeatTime(int mRepeatTime, message* mMessage);

extern int updateDateToRelease(char* mDateRelease, message* mMessage);

extern int updatePlatformToRelease(char* mPlatform, message* mMessage);


#endif
