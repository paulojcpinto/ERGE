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
		 int i,result=0;
		 for(i=0;i<4;i++)
		 {
			 if(userAux->pinCode[i] != pinCode[i])
				 
			 result=1;
		 }
		 
		 if(!result)
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
 
 
 
 int updateUser(user_parsing updateInfo)
 {
	  user * toUpdate = getUser(updateInfo.nickName);
		if(!strcmp(toUpdate->nickName,updateInfo.nickName))
		{
			int error=1;
			error=toUpdate->funcs.updateEmail(updateInfo.email,toUpdate);
			error=toUpdate->funcs.updateEmailPassowrd(updateInfo.emailPassword,toUpdate);
			error=toUpdate->funcs.updatePhoneNumber(updateInfo.phoneNumber,toUpdate);
			error=toUpdate->mmessage.updateMessageToRelease(updateInfo.messageToRelease,&toUpdate->mmessage);
			error=toUpdate->mmessage.updatePlatformToRelease(updateInfo.platformToRelease,&toUpdate->mmessage);
			error=toUpdate->mmessage.updateRepeatTime(updateInfo.repeatTime,&toUpdate->mmessage);
			return error;
			
			
		}else return USER_NOT_FOUND;
 }

 int updatePinCode(char *mpinCode, user * mUser)
 {
	 memset(mUser->pinCode,0,strlen(mUser->pinCode));
	 strcpy(mUser->pinCode,mpinCode);
	 return 1;
 }
 

 int updatePhoneNumber(char * mphoneNumber, user *mUser)
 {
	 memset(mUser->phoneNumber,0,strlen(mUser->phoneNumber));
	 strcpy(mUser->phoneNumber,mphoneNumber);
	 return 1;
 }
 

 int updateEmail(char* mEmail,user* mUser)
 {
	 memset(mUser->email,0,strlen(mUser->email));
	 strcpy(mUser->email,mEmail);
	 return 1;
 }

 int updateEmailPassword(char* mEmailPassword, user* mUser)
 {
	 memset(mUser->emailPassword,0,strlen(mUser->emailPassword));
	 strcpy(mUser->emailPassword,mEmailPassword);
	 return 1;
 }
 