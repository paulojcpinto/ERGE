#include "bluetooth_module.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"

#include "stdlib.h"
#include "user.h"


#define discard_char					UART3Tx_index++

#define char_trama_init 							( char )		'<' 
#define char_trama_end  							( char ) 		'>'	
	
#define char_trama_echo     					( char ) 		'N'
#define int_echo    									(  int )		 3
	
#define char_trama_start 							( char ) 		'L'
#define int_start											(  int )		 2
	
#define char_trama_error              (char)      'E'
#define int_error											(  int )		 4
	
#define char_trama_nick               (char)     'S'
#define int_nick                      (int)       5
	
#define char_trama_pincode            (char)     'P'
#define int_pincode                   (int)       6
	
#define char_trama_email               (char)    'M'
#define int_email                      (int)      7
	
#define char_trama_email_password       (char)   'X'
#define int_email_password              (int)      8

#define char_trama_phone_number        (char)    'T'
#define int_phone_number                (int)     9

#define char_trama_message              (char)   'R'
#define int_message                      (int)   10
	
#define char_trama_repeat               (char)   'O'
#define int_repeat                        (int)  11
	
#define char_trama_date                  (char)  'D'
#define int_date                         (int)   12
	
#define char_trama_platform              (char)  'A'
#define int_platform                      (int)  13

#define char_create_user                 (char)  'C'
#define int_create_user                  (int)   14
	
#define  char_app_login                  (char)  'Q'
#define  int_app_login                   (int)   15

#define char_get_user                     (char)  'G'
#define int_get_user                       (int)   16

#define char_get_phone                    (char) 't'
#define int_get_phone                     (char)  17
	
#define char_get_repeat                   (char)  'o'
#define int_get_repeat                    (int)   18
	
#define char_get_email                     (char)  'm'
#define int_get_email												(int)   19
	
#define char_get_emailpassword             (char) 'x'
#define int_get_emailpassword              (int)  20

#define char_get_message                    (char)  'r'
#define int_get_message                     (int)  21
	
#define char_get_platform                    (char)  'a'
#define int_get_platform                     (int)  22
 	

	



static int out_index = 0;
int error_number = 0;
char echo[255];
user_parsing user_pars;
user* userBluetooh;
char userInfo[50];
int counterUserInfo;
char repeatTime[20];

void UlToStr(char *s, unsigned int bin, unsigned char n)
{
    s += n;
    //*s = '\0';

    while (n--)
    {
        *--s = (bin % 10) + '0';
        bin /= 10;
    }
}


void print_response(char c)
{
	char* s;
					if(out_index<=9)
					{
						
				  	UlToStr(s,out_index,1);
						char aux[4];
						 memset(aux,0,4);
					   aux[0]='<';
					   aux[1]=c;
					   aux[2]=s[0];
					   aux[3]='>';
					   HAL_UART_Transmit_IT(&huart4, aux ,4 );
					}
					
					else if(out_index>9 && out_index<100)
					{
						UlToStr(s,out_index,2);
						char aux[5];
   				 memset(aux,0,5);
					  aux[0]='<';
					  aux[1]=c;
					  aux[2]=s[0];
					  aux[3]=s[1];
					  aux[4]='>';
					  HAL_UART_Transmit_IT(&huart4, aux ,5 );
						
					}
					else 
					{
						UlToStr(s,out_index,2);
						char aux[6];
					  memset(aux,0,6);
					  aux[0]='<';
					  aux[1]=c;
					  aux[2]=s[0];
					  aux[3]=s[1];
						aux[4]= s[2];
					  aux[5]='>';
					  HAL_UART_Transmit_IT(&huart4, aux ,5 );
					}
					
	       

					UART3Tx_index++;
					
	
}





void print_responseLogin(int log)
{
	char* s;
	
	if(log<=9)
					{						
				  	UlToStr(s,log,1);
						uint8_t  pp[4];
						memset(pp,0,4);
					   pp[0]='<';
					   pp[1]=char_app_login;
					   pp[2]=s[0];
					   pp[3]='>';
					   HAL_UART_Transmit_IT(&huart4,  pp,4 );
					}				      
					UART3Tx_index++;
	
}



void printParameter(char* parameter, char category)
{
	int i;
	char message[300];
	memset(message,0,300);
	message[0]='<';
	message[1]=category;
	for(i=2; i<strlen(parameter)+2;i++)
	{
		message[i] = parameter[i-2];
	}
	int aux;
	aux=strlen(message);
	message[aux]= '>';
  HAL_UART_Transmit_IT(&huart4,  message,aux+1 );
	UART3Tx_index++;

	
}

void printNumber(int number,char c)
{
	char* s;
	char numberChar[30];
	memset(numberChar,0,30);
	sprintf(numberChar,"<%c%d>",c,number);
  HAL_UART_Transmit_IT(&huart4,  numberChar,strlen(numberChar) );
	UART3Tx_index++;
	
}



void receive_start (int *c )
{
	*c = int_start;
	UART3Tx_index++;
	
}


void receive_echo ( int *c )
{
	*c = int_echo;
	echo[out_index++] = char_trama_init;
	echo[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
}



void prepare_receive_info(int *c )
{
	switch ( UART3Rx_Buffer[UART3Tx_index] )
	{
		case char_trama_echo:
		{
			*c= 3;
			receive_echo (c);
		}; break;
		
		case char_trama_start:
		{
			*c = 2;
			receive_start (c);
		}; break;
		
		case char_trama_nick:
			*c= int_nick;
			UART3Tx_index++;
			break;
		
		case char_trama_pincode:
			*c= int_pincode;
			UART3Tx_index++;
			break;
		
		case char_trama_email:
			*c= int_email;
			UART3Tx_index++;
			break;
		
		case char_trama_email_password:
			*c= int_email_password;
			UART3Tx_index++;
			break;
		case char_trama_phone_number:
			*c= int_phone_number;
			UART3Tx_index++;
			break;
	
		case char_trama_message:
			*c= int_message;
			UART3Tx_index++;
			break;
		
		case char_trama_repeat:
			*c= int_repeat;
			UART3Tx_index++;
	  	memset(repeatTime,0,20);
			break;
		
		case char_trama_date:
			*c= int_date;
			UART3Tx_index++;
			break;
		
		case char_trama_platform:
			*c= int_platform;
			UART3Tx_index++;		
		break;
		
		case char_create_user:
			*c=int_create_user;
		  UART3Tx_index++;	
			break;
		case char_app_login:
			*c=int_app_login;
			 UART3Tx_index++;	
			break;
		
		case char_get_phone:
			*c=int_get_phone;
			UART3Tx_index++;	
			break;
		case char_get_user:
			*c=int_get_user;
		  counterUserInfo=0;
			UART3Tx_index++;	
			break;
		
		case char_get_repeat:
			*c=int_get_repeat;
			 UART3Tx_index++;	
			break;
		case char_get_email:
			*c=int_get_email;
		  UART3Tx_index++;	
			break;
		case char_get_emailpassword:
			*c= int_get_emailpassword;
			UART3Tx_index++;	
			break;
		case char_get_message:
			*c=int_get_message;
  		UART3Tx_index++;	
			break;
		case char_get_platform:
			*c=int_get_platform;
		  UART3Tx_index++;	
			break;
		
		case char_trama_error:
		{
			*c=int_error;
			UART3Tx_index++;
			error_number=0;
		};break;
	}
}


void end_receiving_trama (int *c)
{
	int loginAnswer;
	switch (*c)
			{
				case int_echo:
				{
					echo[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
					HAL_UART_Transmit_IT(&huart4, echo, out_index);

				}; break;
				
				case int_start:
				{
					UART3Tx_index++;
					HAL_UART_Transmit_IT(&huart4, " STM", sizeof(" STM")/sizeof(char));
		


				}; break;
				case int_error:
						HAL_UART_Transmit_IT(&huart4, user_pars.nickName, out_index);
					break;
				
				case int_nick:
				 print_response(char_trama_nick);
				break;
				
				case int_pincode:
				 print_response(char_trama_pincode);
				break;
				
				case int_email:
					print_response(char_trama_email);
					break;
				
				case int_email_password:
					print_response(char_trama_email_password);
					break;
				
				case int_phone_number:
					print_response(char_trama_phone_number);
					break;
				
				case int_message:
					print_response(char_trama_message);
					break;
				
				case int_repeat:
					user_pars.repeatTime= atoi(repeatTime);
					print_response(char_trama_repeat);
					break;
				
				case int_date:
					print_response(char_trama_date);
					break;
				
				case int_platform:
					print_response(char_trama_platform);				
					break;
				case int_create_user:
					UART3Tx_index++;
					createUser(user_pars);
					break;
				case int_app_login:
				  loginAnswer=login(user_pars.nickName,user_pars.pinCode);
				  print_responseLogin(loginAnswer);
					break;
				case int_get_user:
				  userBluetooh = getUser(userInfo);
			  	printParameter(userBluetooh->phoneNumber,char_get_phone);
					break;
				case int_get_repeat:
					printNumber(userBluetooh->mmessage.repeatTime,char_get_repeat);					
					break;
				case int_get_email:
					printParameter(userBluetooh->email,char_get_email);
					break;
				case int_get_emailpassword:
					printParameter(userBluetooh->emailPassword,char_get_emailpassword);
					break;
				case int_get_message:
					printParameter(userBluetooh->mmessage.messageToRelease,char_get_message);
				break;
				case int_get_platform:
					printParameter(userBluetooh->mmessage.platformToRelease,char_get_platform);
					break;
				case int_get_phone:
					printParameter(userBluetooh->phoneNumber,char_get_phone);
					break;
				

				}
				*c = -1;
			}
void save_char (int *c )
{
	switch (*c)
			{
				case int_echo:
				{
					echo[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
				}; break;
				case int_error:
				{
					char aux = UART3Rx_Buffer[UART3Tx_index++];
				//	error_number += (aux-48)+10^(UART3Tx_index-3);
				}break;
				case int_nick:
				{
					user_pars.nickName[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
				};break;
				case int_pincode:
					user_pars.pinCode[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
					break;
				case int_email:
					user_pars.email[out_index++]= UART3Rx_Buffer[UART3Tx_index++];
					break;
				case int_email_password:
					user_pars.emailPassword[out_index++]= UART3Rx_Buffer[UART3Tx_index++];
				break;
				case int_phone_number:
					user_pars.phoneNumber[out_index++]= UART3Rx_Buffer[UART3Tx_index++];
					break;
				case int_message:
					user_pars.messageToRelease[out_index++]=UART3Rx_Buffer[UART3Tx_index++];
					break;
				case int_repeat:
				//	user_pars.repeatTime += (UART3Rx_Buffer[UART3Tx_index++]-'0')*(10 ^(out_index++-2));
				  repeatTime[out_index++]=UART3Rx_Buffer[UART3Tx_index++];
					break;
				case int_date:
					user_pars.dateToStart[out_index++]=UART3Rx_Buffer[UART3Tx_index++];
					break;
				case int_platform:
					user_pars.platformToRelease[out_index++]=UART3Rx_Buffer[UART3Tx_index++];
				  
					break;
				case int_get_user:
					userInfo[counterUserInfo++]=UART3Rx_Buffer[UART3Tx_index++];
					break;
				
				
				default : *c = -1;
				
			}
			
}


int parsingBT (int *c)
{

	
	while(UART3Tx_index != UART3Rx_index)
	{
		if(UART3Rx_Buffer[UART3Tx_index] == char_trama_init)
		{
			
			*c = 1;
			UART3Tx_index ++;
			out_index = 0;

		}
		else if (*c == 1)
		{
			prepare_receive_info(c);
		}
		else if (UART3Rx_Buffer[UART3Tx_index] == char_trama_end)
		{
			end_receiving_trama (c);
		}
		else if (*c == 2)
		{
			*c = -1;
		}
		else if (*c > 2)			
		{
			save_char(c);
		}
		else
			discard_char;
		
		out_index &= ~(1<<7);
		UART3Tx_index &= ~(1<<7);
	}
	return *c;
}





