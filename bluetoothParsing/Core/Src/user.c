#include "user.h"
#include <string.h>
#include <localtime.h>
#include "init.h"

#define MAX_USERS 10

int  nextUser = 0;
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
	  users[i].unlocked=0;

	}
	
	
}

 user* getUser(char* nickName)
 {
	 int i=0;
	 for(i=0;i<MAX_USERS;i++)
	 {
		 if(!strcmp(users[i].nickName,nickName))
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
	 users[nextUser].unlocked=1;
	 nextUser++;
	 return 1;	 
	 }
	 else return 0;
	 
 }
 
 int login(char* nickName, char* pinCode)
 {
	 user* userAux;
	 userAux=getUser(nickName);
	 if(!strcmp(userAux->nickName,nickName))
	 {
		 if(!strcmp(userAux->pinCode,pinCode))
		 {
			 if(userAux->unlocked)
			 {
				 return LOGIN_SUCCESS;    //login success
			 }
			 else return USER_BLOCKED;   //user blocked
		 }
		 else return BAD_CREDENTIALS;   //wrong pincode
	 }
	 else return USER_NOT_FOUND;  //return user not found
 }
 
 
 

 int updatePinCode(char *mpinCode, user * mUser)
 {
	 return 1;
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
 
 
 void verify_release_time ( void )
 {
	 nextUser =1;
	 for ( int count = 0; count < nextUser; count ++)
	 {
		 if(  (stmtime.localtim->tm_min <= 60) && (stmtime.need_update==1))
		 {
			 HAL_UART_Transmit(&huart3, "\r\ndedo\r\n", 8, 1000);
				xSemaphoreGive(finger_signal);
		 }
		 else			 
			HAL_UART_Transmit(&huart3, "\r\nnotd\r\n", 8, 1000);
	 }
	 
	 stmtime.need_update =1;
 }
