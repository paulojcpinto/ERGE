#ifndef __bluetooth_module_H
#define __bluetooth_module_H


typedef struct User_parsing user_parsing;

struct User_parsing
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

};

extern int parsingBT ( int *c);





#endif