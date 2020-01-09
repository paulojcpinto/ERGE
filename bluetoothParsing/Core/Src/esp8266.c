#include "esp8266.h"
#include <stdio.h>
#include "user.h"


#define   wait_connect_face           ( int )   20
#define   wait_ask_face               ( int )   21
#define   wait_receive_predate        ( int )   22
#define   wait_receive_date           ( int )   23
#define   wait_end_date               ( int )   24
#define   wait_close_connection       ( int )   25
	
#define   wait_connect_twitter        ( int )   26
#define   wait_ask_twitter            ( int )   27
#define   wait_publish_twitter        ( int )   28
	
#define   discard                     ( int )   50
	
enum months { January = 1, February, March, April, May, June, July, August, September, October, November, December };

int u;
int oo=0;
static uint8_t local1 = 0;
static int local_position1 = 0;
static volatile uint8_t busy = 0;



char sDate[]              =   "Date: ";//take out the space
char sOK[]                =   "OK";
char sReady_message[]     =   ">";
char yapp[]               =   "Content-";
char error[]              =   "ERROR";
char rec[]                =   "byte";


char month1[3];
char *sim2 = sOK;
int isim2 = sizeof( sOK ) / sizeof( char ) -1;

uint8_t message_to_release[ MAX_USERS ][256];
uint8_t who_to_publish[MAX_USERS] [16];
uint8_t Tx_publish = 0;
uint8_t Rx_publish = 0;

void atribute_month ( void );
void ini ()
{
	local1 = 0x14;
}

void publish_twitter ( uint8_t message_release[], uint8_t who_publish[] )
{
	strcpy( message_to_release[Rx_publish], message_release );
	strcpy( who_to_publish[Rx_publish++], who_publish );
	Rx_publish &= ~MAX_USERS;

}
void publ()
{
	if (Rx_publish !=Tx_publish)
	{
		stmtime.need_update=2;
//	while (busy)
//	{
//		HAL_UART_Transmit(&huart3, "s", 1, 100);

//	}
//		
	busy = 1;
	printf("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	sim2 = sOK;
	isim2 = sizeof( sOK ) / sizeof( char ) -1;
	local1 =26;
	local_position1 = 0;
	}
}
void publish_twitter1 ( void )
{
	
	stmtime.need_update=2;
//	while (busy)
//	{
//		HAL_UART_Transmit(&huart3, "s", 1, 100);

//	}
//		
	busy = 1;
	printf("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	sim2 = sOK;
	isim2 = sizeof( sOK ) / sizeof( char ) -1;
	local1 =26;
	local_position1 = 0;
}

void wait_close ( void )
{
	if ( UART5Rx_Buffer[UART5Tx_index] == sOK[local_position1] )
	{
		if ( ++local_position1 == ( sizeof( sOK ) / sizeof( char ) - 1) )
		{
			stmtime.updated = 0;
			busy = 0;
			local1 = discard;
		}
	}
	else if (UART5Rx_Buffer[UART5Tx_index] == error[local_position1] )
	{
		if ( ++local_position1 == ( sizeof( error ) / sizeof( char ) - 1))
		{
			stmtime.updated = 0;
			busy = 0;
			local1 = discard;
		}
	}
	else
	{
		UART5Tx_index -= local_position1;
		local_position1 = 0;
	}
	
}


void update_date1 ()
{
	switch ( local_position1 )
	{
		case 5:
		{
			stmtime.localtim->tm_mday = (UART5Rx_Buffer[UART5Tx_index] - '0')*10;
		}break ;
		case 6:
		{
			stmtime.localtim->tm_mday += (UART5Rx_Buffer[UART5Tx_index] - '0');
			if (stmtime.localtim->tm_mday == 29)
				HAL_UART_Transmit(&huart3, "\r\n\r\nits\r\n\r\n", 11, 1000);
		}break;
		
		case 8:
		{		}
		case 9:
		{				
			month1[local_position1 - 8] = UART5Rx_Buffer[UART5Tx_index];
		}break;
		case 10:
		{
			month1[local_position1 - 8] = UART5Rx_Buffer[UART5Tx_index];
		}break;
		
		case 12:
		{
			atribute_month();
			stmtime.localtim->tm_year = UART5Rx_Buffer[UART5Tx_index] - '0';
		}break;
		case 13:
		{		}
		case 14:
		{		}
		case 15:
		{
			stmtime.localtim->tm_year *= 10;
			stmtime.localtim->tm_year += UART5Rx_Buffer[UART5Tx_index] - '0';
		}break;
		
		case 17:
		{
			stmtime.localtim->tm_hour = UART5Rx_Buffer[UART5Tx_index] - '0';
		}break;
		case 18:
		{
			stmtime.localtim->tm_hour *= 10;
			stmtime.localtim->tm_hour += UART5Rx_Buffer[UART5Tx_index] - '0';
		}break;
		
		case 20:
		{
			stmtime.localtim->tm_min = UART5Rx_Buffer[UART5Tx_index] - '0';
		}break;
		case 21:
		{
			stmtime.localtim->tm_min *= 10;
			stmtime.localtim->tm_min += UART5Rx_Buffer[UART5Tx_index] - '0';
		}break;
		
		case 23:
		{
			stmtime.localtim->tm_sec = UART5Rx_Buffer[UART5Tx_index] - '0';
		}break;
		case 24:
		{
			stmtime.localtim->tm_sec *= 10;
			stmtime.localtim->tm_sec += UART5Rx_Buffer[UART5Tx_index] - '0';

		}break;
		
		case 25:
		{
			stmtime.localtim->tm_wday = 0;
			stmtime.localtim->tm_yday = 0;
			stmtime.localtim->tm_isdst = 0;
					sim2 = yapp;
					isim2 = sizeof( yapp ) / sizeof( char ) -1;
					local1 = 24;
					local_position1 = -1;
			//printf("AT+CIPCLOSE\r\n");
		}break;
	}
	local_position1 ++;

}


void  wait1 ()
{ 
//	while (busy)
//	{
//		vTaskDelay(10);
//		HAL_UART_Transmit(&huart3, "w", 1, 100);
//		
//	}
	stmtime.need_update = 1;
	busy = 1;
	printf("AT+CIPSTART=\"TCP\",\"%s\",80\r\n","facebook.com");
	sim2 = sOK;
	isim2 = sizeof( sOK ) / sizeof( char ) -1;
	local1 =20;
	local_position1 = 0;
}



int vHardware_verify1 ( void  )
{
	if ( UART5Rx_Buffer[UART5Tx_index] == sim2[local_position1] )
	{
		if ( ++local_position1 == ( isim2 ) )
		{
			return 1;
		}					
		return 0;
	}
	else
	{
		UART5Tx_index -= local_position1;
		local_position1 = 0;
		return -1;
	}
}

void atribute_month ( void )
{
	switch ( month1[0] )
	{
		case 'J':
		{
			switch ( month1[1] )
			{
				case 'a':
				{
					stmtime.localtim->tm_mon = January;
				}break;
				case 'u':
				{
					switch ( month1[2] )
					{
						case 'n':
						{
							stmtime.localtim->tm_mon = June;
						}break;
						case 'l':
						{
							stmtime.localtim->tm_mon = July;
						}break;
					}
				}break;
			}
		}break;
		case 'F':
		{
			stmtime.localtim->tm_mon = February;
		}break;
		case 'M':
		{
			switch ( month1[2] )
			{
				case 'r':
				{
					stmtime.localtim->tm_mon = March;
				}break;
				case 'y':
				{
					stmtime.localtim->tm_mon = May;
				}break;
			}
		}break;
		case 'A':
		{
			switch ( month1[1] )
			{
				case 'p':
				{
					stmtime.localtim->tm_mon = April;
				}break;
				case 'u':
				{
					stmtime.localtim->tm_mon = August;
				}break;
			}
		}break;
		case 'S':
		{
			stmtime.localtim->tm_mon = September;
		}break;
		case 'O':
		{
			stmtime.localtim->tm_mon = October;
		}break;
		case 'N':
		{
			stmtime.localtim->tm_mon = November;
		}break;
		case 'D':
		{
			stmtime.localtim->tm_mon = December;
		}break;
	}
}

void parsing_gsm11 ( void )
{
	while ( UART5Tx_index != UART5Rx_index )
	{
		switch ( local1 )
		{
			case wait_connect_face:
			{
				if ( vHardware_verify1 ( ) > 0 )
				{
					printf("AT+CIPSEND=19\r\n");

					HAL_UART_Transmit(&huart3, "w1", 2, 100);

					sim2 = sReady_message;
					isim2 = sizeof( sReady_message ) / sizeof( char ) -1;
					local1 = 21;
					local_position1 = 0;
				}
			}break;
			
			case wait_ask_face:
			{
				if ( vHardware_verify1 ( ) > 0 )
				{
					HAL_UART_Transmit(&huart3, "w2", 2, 100);
					printf("HEAD / HTTP/1.1\r\n\r\n");
					sim2 = sDate;
					isim2 = sizeof( sDate ) / sizeof( char ) -1;
					local1 = 22;
					local_position1 = 0;
				}				
			}break;
			
			case wait_receive_predate:
			{
				if ( vHardware_verify1 ( ) > 0 )
				{
					HAL_UART_Transmit(&huart3, "w3", 2, 100);
					local1 = 23;
					local_position1 = 0;
				}
			}break;
			
			case wait_receive_date:
			{
				HAL_UART_Transmit(&huart3, "w4", 2, 100);
				update_date1 ();
			}break;
			
			case wait_end_date:
			{
				if ( vHardware_verify1 ( ) > 0 )
				{
					HAL_UART_Transmit(&huart3, "w5", 2, 100);
					printf("AT+CIPCLOSE\r\n");
					
				local1 =25;}
			}break;
			
			case wait_close_connection:
			{
						HAL_UART_Transmit(&huart3, "w6", 2, 100);
				stmtime.need_update = 0;
				busy = 0;
				wait_close();
			}break;
			
			case wait_connect_twitter:
			{
				if ( vHardware_verify1 ( ) > 0)
				{
					printf("AT+CIPSEND=%d\r\n",  strlen("GET /apps/thingtweet/1/statuses/update?api_key=&status=\r\n") + strlen(message_to_release[Tx_publish])+16 );
					sim2 = sReady_message;
					isim2 = sizeof( sReady_message ) / sizeof( char ) -1;
					local1 = 27;
					local_position1 = 0;
				}
			}break;
			
			case wait_ask_twitter:
			{
				if ( vHardware_verify1 ( ) > 0 )
				{
					//HAL_UART_Transmit(&huart3, "\r\nsim\r\n",7, 1000);
					//printf("GET /apps/thingtweet/1/statuses/update?api_key=6TOVXUMDMAKFHWLF&status=11%d\r\n", oo);
					HAL_UART_Transmit(&huart5,"GET /apps/thingtweet/1/statuses/update?api_key=" ,strlen("GET /apps/thingtweet/1/statuses/update?api_key="), 10000);
					HAL_UART_Transmit(&huart5, who_to_publish[Tx_publish], 16, 10000);
					HAL_UART_Transmit(&huart5, "&status=", 8, 10000);
					HAL_UART_Transmit(&huart5, message_to_release[Tx_publish], strlen(message_to_release[Tx_publish]), 10000);
					HAL_UART_Transmit(&huart5, "\r\n", 2, 10000);
					sim2 = rec;
					isim2 = sizeof( rec ) / sizeof( char ) -1;
					Tx_publish++;
					Tx_publish &= ~MAX_USERS;

					local1 = 28;
					local_position1 = 0;
				}				
				
			}break;
			
			case wait_publish_twitter:
			{
				if ( vHardware_verify1 ( ) > 0 )
				{
					//printf("AT+CIPCLOSE\r\n");
					stmtime.need_update = 0;
					busy = 0;
					local1 = discard;
				}
			}break;
			
			
			case discard :
			{
				if ( Tx_publish != Rx_publish )
					publish_twitter1();
				stmtime.need_update = 0;
			}break;
				
				
		}	
		UART5Tx_index++;
		UART5Tx_index &= ~(1<<10);		
	}
}
