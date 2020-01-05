#ifndef __user_H
#define __user_H

#include "usart.h"
#include "bluetooth_module.h"
#include "message.h"
#include "init.h"
#include "localtime.h"

#define LOGIN_SUCCESS 1
#define USER_NOT_FOUND 2
#define USER_BLOCKED  3
#define BAD_CREDENTIALS  4


typedef struct UserFuncitons userfuncs;
typedef struct User user;


struct UserFuncitons
{//functions
	int (*updatePinCode)(char *,user*);
	int (*updatePhoneNumber)(char *,user*);
	int (*updateEmail)(char *,user*);
  int  (*updateEmailPassowrd) (char*,user*);
	
	
};


struct User
{
	message mmessage; 
	userfuncs funcs;
	int position;
	char nickName[25];
	char pinCode[4];
	char phoneNumber[13];
	char email[50];
	char emailPassword[12];
	char unlocked;
	char presenceCheck;
 	
};


extern  void initUser(void);

extern user* getUser(char* nickName);

extern int createUser(user_parsing newUser);

extern int updatePinCode(char *mpinCode, user * mUser);

extern int updatePhoneNumber(char * mphoneNumber, user *mUser);

extern int updateEmail(char* mEmail,user* mUser);

extern int login(char* nickName, char* pinCode);

extern int updateEmailPassword(char* mEmailPassword, user* mUser);

extern int updateUser(user_parsing updateInfo);






//extern int parsingBT (user *me, int *c);


#endif /*__ user_H */
