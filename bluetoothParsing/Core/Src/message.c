#include "message.h"
#include <string.h>

int updateMessageToRelease(char* mMessageRelease, message* mMessage)
{
	memset(mMessage->messageToRelease,0,strlen(mMessage->messageToRelease));
	strcpy(mMessage->messageToRelease,mMessageRelease);
	return 1;

}

int updateRepeatTime(int mRepeatTime, message* mMessage)
{
	mMessage->repeatTime= mRepeatTime;
	return 1;
}

int updateDateToRelease(char* mDateRelease, message* mMessage)
{
	memset(mMessage->dateToStart,0,strlen(mMessage->dateToStart));
	strcpy(mMessage->dateToStart,mDateRelease);
	return 1;
}

int updatePlatformToRelease(char* mPlatform, message* mMessage)
{
	memset(mMessage->platformToRelease,0,strlen(mMessage->platformToRelease));
	strcpy(mMessage->platformToRelease,mPlatform);
	return 1;
}

