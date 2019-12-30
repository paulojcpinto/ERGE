#include "esp8266.h"
#include <stdio.h>

int oo=0;
static uint8_t local1 = 0;
static int local_position1 = 0;
static volatile uint8_t busy = 0;

#define discard   			( int )  50

char date_pars1[] 				= 	"Date: ";//take out the space
char response_ok1[] 			=   "OK";
char ready_message1[] 		=		">";
char yapp[]								=  "Content-";
char error[]							=		"ERROR";
char rec[]								=		"byte";


char month1[3];
char *sim2 = response_ok1;
int isim2 = sizeof( response_ok1 ) / sizeof( char ) -1;


void publish_twitter ( int i )
{
oo = i;
	if (busy)
	{
		HAL_UART_Transmit(&huart3, "s", 1, 100);

	}
		
	busy = 1;
	printf("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	sim2 = response_ok1;
	isim2 = sizeof( response_ok1 ) / sizeof( char ) -1;
	local1 =26;
	local_position1 = 0;
}

void wait_close ( void )
{
	if ( UART3Rx_Buffer[UART3Tx_index] == response_ok1[local_position1] )
	{
		if ( ++local_position1 == ( sizeof( response_ok1 ) / sizeof( char ) - 1) )
		{
			busy = 0;
			local1 = discard;
		}
	}
	else if (UART3Rx_Buffer[UART3Tx_index] == error[local_position1] )
	{
		if ( ++local_position1 == ( sizeof( error ) / sizeof( char ) - 1))
		{
			busy = 0;
			local1 = discard;
		}
	}
	else
	{
		UART3Tx_index -= local_position1;
		local_position1 = 0;
	}
	
}


void update_date1 ()
{
	switch ( local_position1 )
	{
		case 5:
		{
			stmtime.localtim->tm_yday = (UART3Rx_Buffer[UART3Tx_index] - '0')*10;
		}break ;
		case 6:
		{
			stmtime.localtim->tm_yday += (UART3Rx_Buffer[UART3Tx_index] - '0');
			if (stmtime.localtim->tm_yday == 29)
				HAL_UART_Transmit(&huart3, "\r\n\r\nits\r\n\r\n", 11, 1000);
		}break;
		
		case 8:
		{		}
		case 9:
		{		}
		case 10:
		{
			month1[local_position1 - 8] = UART3Rx_Buffer[UART3Tx_index];
		}break;
		
		case 12:
		{
			stmtime.localtim->tm_year = UART3Rx_Buffer[UART3Tx_index] - '0';
		}break;
		case 13:
		{		}
		case 14:
		{		}
		case 15:
		{
			stmtime.localtim->tm_year *= 10;
			stmtime.localtim->tm_year += UART3Rx_Buffer[UART3Tx_index] - '0';
		}break;
		
		case 17:
		{
			stmtime.localtim->tm_hour = UART3Rx_Buffer[UART3Tx_index] - '0';
		}break;
		case 18:
		{
			stmtime.localtim->tm_hour *= 10;
			stmtime.localtim->tm_hour += UART3Rx_Buffer[UART3Tx_index] - '0';
		}break;
		
		case 20:
		{
			stmtime.localtim->tm_min = UART3Rx_Buffer[UART3Tx_index] - '0';
		}break;
		case 21:
		{
			stmtime.localtim->tm_min *= 10;
			stmtime.localtim->tm_min += UART3Rx_Buffer[UART3Tx_index] - '0';
		}break;
		
		case 23:
		{
			stmtime.localtim->tm_sec = UART3Rx_Buffer[UART3Tx_index] - '0';
		}break;
		case 24:
		{
			stmtime.localtim->tm_sec *= 10;
			stmtime.localtim->tm_sec += UART3Rx_Buffer[UART3Tx_index] - '0';
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
	if (busy)

		HAL_UART_Transmit(&huart3, "w", 1, 100);

	busy = 1;
	printf("AT+CIPSTART=\"TCP\",\"%s\",80\r\n","facebook.com");
	sim2 = response_ok1;
	isim2 = sizeof( response_ok1 ) / sizeof( char ) -1;
	local1 =20;
	local_position1 = 0;
}



int vHardware_verify1 ( void  )
{
	if ( UART3Rx_Buffer[UART3Tx_index] == sim2[local_position1] )
	{
		if ( ++local_position1 == ( isim2 ) )
		{
			return 1;
		}					
		return 0;
	}
	else
	{
		UART3Tx_index -= local_position1;
		local_position1 = 0;
		return -1;
	}
}

void parsing_gsm11 ( void )
{
	while ( UART3Tx_index != UART3Rx_index )
	{
		switch ( local1 )
		{
			case 20:
			{
				if ( vHardware_verify1 ( ) > 0 )
				{
					printf("AT+CIPSEND=19\r\n");

		HAL_UART_Transmit(&huart3, "w1", 2, 100);

					sim2 = ready_message1;
					isim2 = sizeof( ready_message1 ) / sizeof( char ) -1;
					local1 = 21;
					local_position1 = 0;
				}
			}break;
			case 21:
			{
				if ( vHardware_verify1 ( ) > 0 )
				{
							HAL_UART_Transmit(&huart3, "w2", 2, 100);
					printf("HEAD / HTTP/1.1\r\n\r\n");
					sim2 = date_pars1;
					isim2 = sizeof( date_pars1 ) / sizeof( char ) -1;
					local1 = 22;
					local_position1 = 0;
				}				
			}break;
			
			case 22:
			{
				if ( vHardware_verify1 ( ) > 0 )
				{
							HAL_UART_Transmit(&huart3, "w3", 2, 100);
					local1 = 23;
					local_position1 = 0;
				}
			}break;
			case 23:
			{
						HAL_UART_Transmit(&huart3, "w4", 2, 100);
				update_date1 ();
			}break;
			case 24:
			{
				if ( vHardware_verify1 ( ) > 0 )
				{
							HAL_UART_Transmit(&huart3, "w5", 2, 100);
				printf("AT+CIPCLOSE\r\n");
					stmtime.updated = 0;
				local1 =25;}
			}break;
			
			case 25:
			{
						HAL_UART_Transmit(&huart3, "w6", 2, 100);
				busy = 0;
				wait_close();
			}break;
			case 26:
			{
				if ( vHardware_verify1 ( ) > 0 )
				{
					printf("AT+CIPSEND=%d\r\n", sizeof("GET /apps/thingtweet/1/statuses/update?api_key=6TOVXUMDMAKFHWLF&status=My first tweet from ESP8266\r\n"));
					sim2 = ready_message1;
					isim2 = sizeof( ready_message1 ) / sizeof( char ) -1;
					local1 = 27;
					local_position1 = 0;
				}
			}break;
			case 27:
			{
				if ( vHardware_verify1 ( ) > 0 )
				{
					HAL_UART_Transmit(&huart3, "\r\nsim\r\n",7, 1000);
					printf("GET /apps/thingtweet/1/statuses/update?api_key=GWA7NVWHFSUK8YW3&status=My first tweet from ESP8711%d\r\n", oo);
					sim2 = rec;
					isim2 = sizeof( rec ) / sizeof( char ) -1;
					local1 = 28;
					local_position1 = 0;
				}				
				
			}break;
			case 28:
			{
				if ( vHardware_verify1 ( ) > 0 )
				{
					busy = 0;
					local1 = discard;
				}
			}break;
			case discard :
			{
			}break;
				
				
		}	
		UART3Tx_index++;
		UART3Tx_index &= ~(1<<9);		
	}
}
