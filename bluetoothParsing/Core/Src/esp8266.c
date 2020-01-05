#include "esp8266.h"
#include <stdio.h>


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

uint8_t message_to_release[256];


void publish_twitter ( uint8_t message_release[] )
{
	strcpy( message_to_release, message_release );
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
			stmtime.localtim->tm_yday = (UART5Rx_Buffer[UART5Tx_index] - '0')*10;
		}break ;
		case 6:
		{
			stmtime.localtim->tm_yday += (UART5Rx_Buffer[UART5Tx_index] - '0');
			if (stmtime.localtim->tm_yday == 29)
				HAL_UART_Transmit(&huart3, "\r\n\r\nits\r\n\r\n", 11, 1000);
		}break;
		
		case 8:
		{		}
		case 9:
		{		}
		case 10:
		{
			month1[local_position1 - 8] = UART5Rx_Buffer[UART5Tx_index];
		}break;
		
		case 12:
		{
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
					printf("AT+CIPSEND=%d\r\n", sizeof("GET /apps/thingtweet/1/statuses/update?api_key=6TOVXUMDMAKFHWLF&status=00\r\n"));
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
					printf("GET /apps/thingtweet/1/statuses/update?api_key=6TOVXUMDMAKFHWLF&status=11%d\r\n", oo);
					sim2 = rec;
					isim2 = sizeof( rec ) / sizeof( char ) -1;
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
				stmtime.need_update = 0;
			}break;
				
				
		}	
		UART5Tx_index++;
		UART5Tx_index &= ~(1<<10);		
	}
}
