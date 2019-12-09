#ifndef __user_H
#define __user_H

#include "usart.h"
#include "bluetooth_module.h"

typedef struct User user;


struct User
{
	char nickName[25];
	char pinCode[4];
	char phoneNumber[13];
	char email[50];
	char emailPassword[12];
	char messageToRelease[255];
	int repeatTime;
	char dateToStart[10];
	char platformToRelease[50];
	char unlocked;
	char prenseceCheck;
	
	
};

extern user getUser(char* nickName, int lengh);

extern int createUser(user_parsing newUser);





//extern int parsingBT (user *me, int *c);


#endif /*__ user_H */
