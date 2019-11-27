#ifndef __user_H
#define __user_H

#include "usart.h"
	
typedef struct User user;


struct User
{
	char nickName[15];
	char phoneNumber[9];
	int (*getNickName)(user *, int *); 
};


extern int getNickName (user *me, int *c);


#endif /*__ user_H */
