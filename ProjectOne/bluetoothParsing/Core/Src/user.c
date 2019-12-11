#include "user.h"
#include <string.h>

#define MAX_USERS 10

int  nextUser;
user users[MAX_USERS];




void initUser(void)
{

	int i;
	nextUser =0;
	for(i=0;i<MAX_USERS;i++)
	{
		
	  users[i].funcs.updatePinCode = updatePinCode;
		users[i].funcs.updateEmail= updateEmail;
		users[i].funcs.updateEmailPassowrd=updateEmailPassword;
		users[i].funcs.updatePhoneNumber = updatePhoneNumber;
		
		users[i].mmessage.updateDateToRelease=updateDateToRelease;
		users[i].mmessage.updateMessageToRelease= updateMessageToRelease;
		users[i].mmessage.updatePlatformToRelease= updatePlatformToRelease;
		users[i].mmessage.updateRepeatTime= updateRepeatTime;
		users[i].position=i;
	}
	
	
}

 user* getUser(char* nickName)
 {
	 int i=0;
	 for(i=0;i<MAX_USERS;i++)
	 {
		 if(strcmp(users[i].nickName,nickName))
		 {
			 return &users[i];
		 }
		 
	 }
	 return 0;
 }

 int createUser(user_parsing newUser)
 {
	 int i=0;
	 if(nextUser <= MAX_USERS)
	 {
   strcpy(users[nextUser].nickName,newUser.nickName);
	 strcpy(users[nextUser].pinCode,newUser.pinCode);
	 strcpy(users[nextUser].email,newUser.email);
	 strcpy(users[nextUser].emailPassword,newUser.emailPassword);
	 strcpy(users[nextUser].phoneNumber,newUser.phoneNumber);
	 strcpy(users[nextUser].mmessage.dateToStart,newUser.dateToStart);
	 strcpy(users[nextUser].mmessage.messageToRelease,newUser.messageToRelease);
	 strcpy(users[nextUser].mmessage.platformToRelease,newUser.platformToRelease);
	 users[nextUser].mmessage.repeatTime= newUser.repeatTime;
	 nextUser++;
	 return 1;	 
	 }
	 else return 0;
	 
 }

 int updatePinCode(char *mpinCode, user * mUser)
 {
	 return 0;
 }
 

 int updatePhoneNumber(char * mphoneNumber, user *mUser)
 {
	 return 0;
 }
 

 int updateEmail(char* mEmail,user* mUser)
 {
	 return 0;
 }

 int updateEmailPassword(char* mEmailPassword, user* mUser)
 {
	 return 0;
 }
 