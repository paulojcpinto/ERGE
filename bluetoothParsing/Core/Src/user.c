#include "user.h"
#include <string.h>
#include "bluetooth_module.h"


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
	 for(i=0;i<nextUser;i++)
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
		 if(strcmp(getUser(newUser.nickName)->nickName,newUser.nickName))
		 {
			 strcpy(users[nextUser].nickName,newUser.nickName);
			 strcpy(users[nextUser].pinCode,newUser.pinCode);
			 strcpy(users[nextUser].email,newUser.email);
			 strcpy(users[nextUser].emailPassword,newUser.emailPassword);
			 strcpy(users[nextUser].phoneNumber,newUser.phoneNumber);
			 strcpy(users[nextUser].mmessage.dateToStart,newUser.dateToStart);
			 strcpy(users[nextUser].mmessage.messageToRelease,newUser.messageToRelease);
			 strcpy(users[nextUser].mmessage.platformToRelease,newUser.platformToRelease);
			 strcpy(pin_code, newUser.pinCode);
			 users[nextUser].mmessage.repeatTime= newUser.repeatTime;
			 users[nextUser].unlocked=0;
			 users[nextUser].mmessage.dateToRelease1.tm_year = 0;
			 users[nextUser].mmessage.dateToRelease1.tm_mon = 0;
			 users[nextUser].mmessage.dateToRelease1.tm_mday = 0;
			 users[nextUser].mmessage.dateToRelease1.tm_min = 0;
			 users[nextUser].mmessage.dateToRelease1.tm_hour = 0;
			 users[nextUser].release_memory = 0;
			 for(int position = 0; position < strlen(users[nextUser].mmessage.dateToStart); position++)
			 {
					if ( users[nextUser].mmessage.dateToStart[ position ] == '/' )
					 i++;
					else
						switch ( i )
						{
							case 0:
							{
								users[nextUser].mmessage.dateToRelease1.tm_mday *= 10;
								users[nextUser].mmessage.dateToRelease1.tm_mday += users[nextUser].mmessage.dateToStart[ position ] - '0';
							}break;
							case 1:
							{
								users[nextUser].mmessage.dateToRelease1.tm_mon *= 10;
								users[nextUser].mmessage.dateToRelease1.tm_mon += users[nextUser].mmessage.dateToStart[ position ] - '0';
							}break;
							case 2:
							{
								users[nextUser].mmessage.dateToRelease1.tm_year *= 10;
								users[nextUser].mmessage.dateToRelease1.tm_year += users[nextUser].mmessage.dateToStart[ position ] - '0';
							}break;
							case 3:
							{
								users[nextUser].mmessage.dateToRelease1.tm_hour *= 10;
								users[nextUser].mmessage.dateToRelease1.tm_hour += users[nextUser].mmessage.dateToStart[ position ] - '0';
							}break;
							case 4:
							{
								users[nextUser].mmessage.dateToRelease1.tm_min *= 10;
								users[nextUser].mmessage.dateToRelease1.tm_min += users[nextUser].mmessage.dateToStart[ position ] - '0';
							}break;
						}
			 }
			 users[nextUser].mmessage.phone_to_release[0] = '+';
			 users[nextUser].mmessage.phone_to_release[1] = '3';
			 users[nextUser].mmessage.phone_to_release[2] = '5';
			 users[nextUser].mmessage.phone_to_release[3] = '1';
			 if ( users[nextUser].mmessage.platformToRelease[0] == 'S' ) 
			 {
				 for ( uint8_t position = 4; position < 13; position++)
				 {
					 users[nextUser].mmessage.phone_to_release[position] = users[nextUser].mmessage.platformToRelease[position];
				 }
			 }
			 else 
			 {
				 for ( uint8_t position = 8; position < 24; position++)
				 {
					 users[nextUser].mmessage.twitter_cardentials[position-8] = users[nextUser].mmessage.platformToRelease[position];
				 }
			 }
		 
			 return 1;	 
		 }
		 return 2;	 
	 }
	  return 3;
	 
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
				 return LOGIN_SUCCESS;
				 //login success
			 }
			 else 
			 {
  			 strcpy(pin_code, pinCode);
				 xSemaphoreGive(numPad);
				 return USER_BLOCKED;   //user blocked

			 }
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
 
 void update_time( uint8_t use )
 {
	 int aux = users[use].mmessage.dateToRelease1.tm_min + users[use].mmessage.repeatTime;
	 while(aux > 0)
	 {
	 if (aux > 59)
	 {
	  users[use].mmessage.dateToRelease1.tm_hour += 1;
	 
	 }
	 else 
		 users[use].mmessage.dateToRelease1.tm_min  =aux;
	 	aux -= 60;	
 }
	 
 }
 
 
 void verify_release_time1 ( void )
 {
	 uint64_t user_time;
	 uint64_t current_time=0;
	 current_time = stmtime.localtim->tm_min;
	 current_time += stmtime.localtim->tm_hour * hour;
	 current_time += stmtime.localtim->tm_mday * day;
	 current_time += stmtime.localtim->tm_mon * month;
	 current_time += stmtime.localtim->tm_year * year;
	 for ( int count = 0; count < nextUser; count ++)
	 {
		 user_time  = users[ count ].mmessage.dateToRelease1.tm_min;
		 user_time += users[ count ].mmessage.dateToRelease1.tm_hour * hour;
		 user_time += users[ count ].mmessage.dateToRelease1.tm_mday * day;
		 user_time += users[ count ].mmessage.dateToRelease1.tm_mon * month;
		 user_time += users[ count ].mmessage.dateToRelease1.tm_year * year;
		 stmtime.localtim->tm_isdst = 0;
		 stmtime.localtim->tm_wday = 0;
		 stmtime.localtim->tm_yday = 0;
		 
		 aux.localtim->tm_hour =  stmtime.localtim->tm_hour;
		 aux.localtim->tm_mday =  stmtime.localtim->tm_mday;
		 aux.localtim->tm_min =  stmtime.localtim->tm_min;
		 aux.localtim->tm_year =  stmtime.localtim->tm_year;
		 aux.localtim->tm_sec =  stmtime.localtim->tm_sec;
		 aux.localtim->tm_isdst =  stmtime.localtim->tm_isdst;
		 aux.localtim->tm_wday =  stmtime.localtim->tm_wday;
		 aux.localtim->tm_yday =  stmtime.localtim->tm_yday;
		 aux1.localtim->tm_hour =  users[count].mmessage.dateToRelease1.tm_hour;
		 aux1.localtim->tm_mday =  users[count].mmessage.dateToRelease1.tm_mday;
		 aux1.localtim->tm_min =  users[count].mmessage.dateToRelease1.tm_min;
		 aux1.localtim->tm_year =  users[count].mmessage.dateToRelease1.tm_year;
		 aux1.localtim->tm_sec =  users[count].mmessage.dateToRelease1.tm_sec;
		 aux1.localtim->tm_isdst =  users[count].mmessage.dateToRelease1.tm_isdst;
		 aux1.localtim->tm_wday =  users[count].mmessage.dateToRelease1.tm_wday;
		 aux1.localtim->tm_yday =  users[count].mmessage.dateToRelease1.tm_yday;
		 	time_t t1, t2;
			t1 = mktime( aux.localtim);
			t2 = mktime(aux1.localtim);
		 if ( difftime(t1,t2) > users[count].release_memory && !(users[count].presenceCheck) )
		 {
			 users[count].release_memory = 0;
			 strcpy(to_release[ count ].phone_number, users[count].phoneNumber);
			 to_release[count].where = 1;
			 strcpy(to_release[count].message, "Time is running out. Do your Presence Check");
			 to_release[ count ].to_publish = 1;
		 }
		 
		 if ( user_time <= current_time && !(users[count].presenceCheck) )
		 {
			 if ( users[count].mmessage.platformToRelease[0] == 'T' )
			 {
				 strcpy(to_release[ count ].cardentials_twitter, users[count].mmessage.twitter_cardentials);
				 to_release[ count ].to_publish = 1;
				 to_release[count].where = 0;
			 }
			 else 
			 {
				 strcpy(to_release[ count ].phone_number, users[count].mmessage.phone_to_release);
				 to_release[count].where = 1;
			 }
			 strcpy(to_release[count].message, users[count].mmessage.messageToRelease);
			 to_release[ count ].to_publish = 1;
			 users[ count-- ] = users[ --nextUser ];

		 }
		 else if ( user_time >= current_time && users[count].presenceCheck )
		 {
			 update_time(count);
			 users[count].presenceCheck =0;
			 to_release[ count ].to_publish = 0;
		 }
		 else 
		 {
			 to_release[ count ].to_publish = 0;
		 }
	 }
	 stmtime.updated = 0;
	 				 xSemaphoreGive(finger_signal);
 }

 void verify_release_time ( void )
 {

	 for ( int count = 0; count < 1; count ++)
	 {
		 if(  (stmtime.localtim->tm_min <= 60) && stmtime.fingerp == 0 )
		 {
			 HAL_UART_Transmit(&huart3, "\r\ndedo\r\n", 8, 1000);
				xSemaphoreGive(finger_signal);
		 }
		 else			 
			HAL_UART_Transmit(&huart3, "\r\nnotd\r\n", 8, 1000);
	 }
	 
	 stmtime.fingerp = 0;
 }
 
 uint8_t get_fingerID_avaiable ( void )
 {
	 uint8_t fingerID = 1;
	 for ( uint8_t position = 0; ( position < nextUser ) && ( fingerID <= MAX_USERS ) ; position++ )
	 {
		 if ( users[ position ].fingerID == fingerID )
		 {
			 position = 0;
			 fingerID ++;
		 }
	 }
	 if ( fingerID > MAX_USERS )
		 return 0;
	 
	 return fingerID;
 }
 
 void update_presenceCheck ( char ID )
 {
	
	 uint8_t fingerIDp = ID - '0';
	 for (uint8_t pos = 0; pos < nextUser; pos++)
		if ( users[pos].fingerID == ID )
		{
			stmtime.localtim->tm_isdst = 0;
		 stmtime.localtim->tm_wday = 0;
		 stmtime.localtim->tm_yday = 0;
		 
		 aux.localtim->tm_hour =  stmtime.localtim->tm_hour;
		 aux.localtim->tm_mday =  stmtime.localtim->tm_mday;
		 aux.localtim->tm_min =  stmtime.localtim->tm_min;
		 aux.localtim->tm_year =  stmtime.localtim->tm_year;
		 aux.localtim->tm_sec =  stmtime.localtim->tm_sec;
		 aux.localtim->tm_isdst =  stmtime.localtim->tm_isdst;
		 aux.localtim->tm_wday =  stmtime.localtim->tm_wday;
		 aux.localtim->tm_yday =  stmtime.localtim->tm_yday;
		 aux1.localtim->tm_hour =  users[pos].mmessage.dateToRelease1.tm_hour;
		 aux1.localtim->tm_mday =  users[pos].mmessage.dateToRelease1.tm_mday;
		 aux1.localtim->tm_min =  users[pos].mmessage.dateToRelease1.tm_min;
		 aux1.localtim->tm_year =  users[pos].mmessage.dateToRelease1.tm_year;
		 aux1.localtim->tm_sec =  users[pos].mmessage.dateToRelease1.tm_sec;
		 aux1.localtim->tm_isdst =  users[pos].mmessage.dateToRelease1.tm_isdst;
		 aux1.localtim->tm_wday =  users[pos].mmessage.dateToRelease1.tm_wday;
		 aux1.localtim->tm_yday =  users[pos].mmessage.dateToRelease1.tm_yday;
		 	time_t t1, t2;
			t1 = mktime( aux.localtim);
			t2 = mktime(aux1.localtim);
			users[pos].release_memory = difftime(t1,t2);
			users[pos].presenceCheck = 1;
			if (!strcmp(connected_user,users[pos].nickName))
			{
				HAL_UART_Transmit(&huart4, "<F1>", 4, 1000);
			}
			else
				HAL_UART_Transmit(&huart4, "<F2>", 4, 1000);
		}
 }
 

 int create_user_finger ( void )
 {
		uint8_t fingerID = get_fingerID_avaiable ( );
		if ( fingerID )
		{
			if ( Fingerprint_SaveNewFinger(fingerID,40) )
			{				
				users[ nextUser++ ].fingerID = fingerID;
				xSemaphoreGive(numPad);
				return 1;
			}
			else
			{
				return 2;
			}
		}
		else 
		{
			return 3;
		}
 }
  int getPresenceStatus(char* nickname)
 {
	 user* userAux;
	 userAux=getUser(nickname);
	 if(!strcmp(userAux->nickName,nickname))
	 {
		 return userAux->presenceCheck;
	 }
	 else return USER_NOT_FOUND;
 }