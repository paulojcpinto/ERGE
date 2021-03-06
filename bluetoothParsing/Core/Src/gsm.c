#include "gsm.h"
#include <gpio.h>
#include <stdio.h>

#define hardware_verify    									(  int )		 0
#define set_PIN    													(  int )		 1
#define wait_Pin_OK													(	 int )		 2
#define prepare_message											(	 int )		 3
#define update_tim													(	 int )		 4
#define send_message												(	 int )		 5
#define wait_send_message										(	 int )		 6
#define tread_message												(	 int )		 7
#define update_localtime										(	 int )		 8
#define send_SMS_to													(	 int )		 9
#define send_SMS_t													(	 int )		 10
#define SMS_sent														(	 int )		 11
#define discard_char												(	 int )		 12
	
char my_numeber[] = "+351934145654";
uint8_t message[256] ;

int its_updating = 0;
uint8_t size_SMS;
static uint8_t local = 0;
static uint8_t local_position = 0;
int total = 0;
uint8_t busy = 1;
uint8_t SMS_to_update = 0;

char response_ok[] 			=   "OK";
char set_PIN_notOK[] 		=   "SIM PIN";
char set_PIN_OK[] 			=   "READY";
char response_pin_ok[] 	=   "OK";
char PIN_correct[] 			=   "SMS Ready";
char ready_message[] 		=		">";
char message_sent[]			= 	"+CMGS";
char before_localtime[] = 	"xxxxxxxxxxxxx\",\"\","; //phone number + rest to do the local time


char *sim = response_ok;
int isim = sizeof( response_ok ) / sizeof( char ) -1;


void send_SMS ( uint8_t number[13], uint8_t *messag, uint8_t size_message )
{

	local = send_SMS_t;
	busy = 2;
	size_SMS = size_message;
	for ( uint8_t pos=0; pos <size_message; pos ++)
	{
		message [ pos ] = messag [ pos ];
	}
	SMS_to_update = 0;
	HAL_UART_Transmit(&huart6, "AT+CMGS=\"", sizeof("AT+CMGS=\"")/sizeof(char)-1, 10000);
	HAL_UART_Transmit(&huart6, number, sizeof(my_numeber)/sizeof(char)-1, 10000);
	HAL_UART_Transmit(&huart6, "\"\r\n", sizeof("\"\r\n")/sizeof(char)-1, 1000);
	local_position = 0;
		HAL_UART_Transmit(&huart3,"bota", 4, 1000);
	isim = sizeof( ready_message ) / sizeof( char ) - 1;
	sim = ready_message;
	
}



void prepare_send_message (  ) 
{
	busy = 1;
	HAL_UART_Transmit(&huart6, "AT+CMGS=\"", sizeof("AT+CMGS=\"")/sizeof(char)-1, 1000);
	HAL_UART_Transmit(&huart6, "+351934145654", sizeof(my_numeber)/sizeof(char)-1, 1000);
	HAL_UART_Transmit(&huart6, "\"\r\n", sizeof("\"\r\n")/sizeof(char)-1, 1000);
	local ++;
	local_position = 0;

	isim = sizeof( ready_message ) / sizeof( char ) - 1;
	sim = ready_message;

}

void send_sms_ ()
{
	uint8_t c ;
	c =26;
//	sprintf(c, "%c", 26);
	HAL_UART_Transmit(&huart6, message, size_SMS, 10000);
	HAL_UART_Transmit(&huart6, &c, 1, 10000);
//	printf("%c",26);
	local ++;
	local_position = 0;
	busy = 1;
	sim = response_ok;
	isim = sizeof( response_ok ) / sizeof( char ) -1;
}

void send_sms_me (  ) 
{
	printf("ok%c",26);
	local ++;
	local_position = 0;
	isim = sizeof( message_sent ) / sizeof( char ) - 1;
	sim = message_sent;
}

void update_local_time ( void )
{
	if (!its_updating)
	{
		its_updating = 1;
		while(busy == 2);
		busy = 1;
		local = update_tim;
		local_position = 0;
		sim = response_ok;
		isim = sizeof( response_ok ) / sizeof( char ) -1;
		HAL_UART_Transmit(&huart3,"Paulo",5,1000);
		printf("AT+CMGDA=\"DEL ALL\"\r\n");
	}
}

void read_message (  )
{
	local++;
	local_position = 0;
	HAL_UART_Transmit(&huart6,"AT+CMGF=1\r\n", 11, 10000);
}

void parsing_gsm1 ( void )
{
			//		HAL_UART_Transmit(&huart6, "AT+CMGS=\"+351933288042\"\r\n", sizeof("AT+CMGS=\"+351913753546\"\r\n")/sizeof(char)-1, 1000);
			//		HAL_Delay(500);
				//	HAL_UART_Transmit(&huart6, "Andre gay", sizeof("Andre gay")/sizeof(char)-1,1000);
}

int vHardware_verify ( void  )
{
	if ( UART6Rx_Buffer[UART6Tx_index] == sim[local_position] )
	{
		if ( ++local_position == ( isim ) )
		{
			return 1;
		}					
		return 0;
	}
	else
	{
		UART6Tx_index -= local_position;
		local_position = 0;
		return -1;
	}
}


void vSet_pin ( void )
{
	if ( UART6Rx_Buffer[UART6Tx_index] == set_PIN_notOK[local_position] )
	{
		if ( ++local_position == ( sizeof( set_PIN_notOK ) / sizeof( char ) - 1) )
		{
			HAL_GPIO_TogglePin(GPIOB, EmbLED_Blue_Pin);
			sim = PIN_correct;
			isim = sizeof( PIN_correct ) / sizeof( char ) -1;
			HAL_UART_Transmit(&huart6, "AT+CPIN=7484\r\n", 14, 10000);
			HAL_Delay(1000);
			local ++;
		}
	}
	else if ( UART6Rx_Buffer[UART6Tx_index] == set_PIN_OK[local_position] )
	{
		if ( ++local_position == ( sizeof( set_PIN_OK ) / sizeof( char ) - 1) )
		{
			local = discard_char; //set pin already made with success. does not need to do it again 
			HAL_GPIO_TogglePin(GPIOB, EmbLED_Red_Pin);
			sim = response_ok;
			isim = sizeof( response_ok ) / sizeof( char ) -1;
	
			HAL_UART_Transmit(&huart6,"AT+CMGF=1\r\n", 11, 10000);
			HAL_Delay(1000);
		}
	}
	else
	{
		UART6Tx_index -= local_position;
		local_position = 0;
	}
}

void vWait_message ( void )
{
	if ( UART6Rx_Buffer[UART6Tx_index] == before_localtime[local_position] )
	{
		if ( ++local_position == ( sizeof( before_localtime ) / sizeof( char ) - 1) )
		{
			HAL_GPIO_TogglePin(GPIOB, EmbLED_Blue_Pin);
			HAL_GPIO_TogglePin(GPIOB, EmbLED_Green_Pin);
			HAL_GPIO_TogglePin(GPIOB, EmbLED_Red_Pin);
			sim = PIN_correct;
			local_position = 0;
			isim = sizeof( PIN_correct ) / sizeof( char ) -1;
			//printf("AT+CPIN=0522\r\n");
			stmtime.localtim->tm_year =	0;	
			stmtime.localtim->tm_mon 	= 0;
			stmtime.localtim->tm_mday = 0;
			stmtime.localtim->tm_hour =	0;	
			stmtime.localtim->tm_min 	= 0;	
			stmtime.localtim->tm_sec 	= 0;	
			HAL_UART_Transmit(&huart3, "\r\n\r\n\r\nokokk\r\n\r\n\r\n", 16, 10000);			
			local = update_localtime;
		}
	}
	else if ( UART6Rx_Buffer[UART6Tx_index] == response_ok[local_position] )
	{
		if ( ++local_position == ( sizeof( response_ok ) / sizeof( char ) - 1) )
		{
			local_position = 0;
			
			HAL_UART_Transmit(&huart6,"AT+CMGF=1\r\n", 11, 10000);
			HAL_Delay(100);
		}
	}
	else
	{
		UART6Tx_index -= local_position;
		local_position = 0;
	}
}


void update_localtimeStm ( void )
{
	if (total++ % 3)
	{
		switch ( local_position / 2 )
		{
			case year_position:
			{
				stmtime.localtim->tm_year += ( UART6Rx_Buffer[UART6Tx_index] - '0' ) * (10- (9 * ((local_position++)%2)));
			}break;
			
			case month_position:
			{
				stmtime.localtim->tm_mon += ( UART6Rx_Buffer[UART6Tx_index] - '0' ) * (10- (9 * ((local_position++)%2)));
			}break;
			
			case day_position:
			{
				stmtime.localtim->tm_mday += ( UART6Rx_Buffer[UART6Tx_index] - '0' ) * (10- (9 * ((local_position++)%2)));
			}break;
			
			case hour_position:
			{
				stmtime.localtim->tm_hour += ( UART6Rx_Buffer[UART6Tx_index] - '0' ) * (10- (9 * ((local_position++)%2)));
			}break;
			
			case minutes_position:
			{
				stmtime.localtim->tm_min += ( UART6Rx_Buffer[UART6Tx_index] - '0' ) * (10- (9 * ((local_position++)%2)));
			}break;
			
			case seconds_position:
			{
				stmtime.localtim->tm_sec += ( UART6Rx_Buffer[UART6Tx_index] - '0' ) * (10- (9 * ((local_position++)%2)));
			}break;
			case seconds_position + 1:
			{
its_updating=0;
				if (stmtime.localtim->tm_year >= 19)
				{
					HAL_UART_Transmit(&huart3, "\r\nIT'S OK\r\n",11, 1000);
					local = discard_char ;
				busy = 0;
				//stmtime.updated=1;
				//stmtime.need_update=0;
				}
				else
				{
					HAL_UART_Transmit(&huart3, "\r\nnotS OK\r\n",11, 1000);
					busy = 0;
					update_local_time();
						
				}
			}
		}
	}

}



void parsing_gsm ( void )
{
	while ( UART6Tx_index != UART6Rx_index )
	{
		switch ( local )
		{
			case hardware_verify:
			{
				if ( vHardware_verify ( ) > 0 )
				{
					busy = 0;
					for (int position = 0; position < sizeof( my_numeber) / sizeof( char ) - sizeof( char ); position ++ )
					{
						before_localtime [ position ] = my_numeber [ position ];
					}
					local++;
					HAL_GPIO_TogglePin(GPIOB, EmbLED_Green_Pin);
					HAL_UART_Transmit(&huart6,"AT+CPIN?\r\n",10, 1000);
					sim = response_pin_ok;
					isim = sizeof( response_pin_ok ) / sizeof( char ) -1;
				}
			}break;
			
			case set_PIN:
			{
				vSet_pin ( );
			}break;
			case wait_Pin_OK:
			{
				if ( vHardware_verify ( ) > 0 )
				{
					HAL_UART_Transmit(&huart3, "\r\nOK OK\r\n",9, 1000);
					local = discard_char;
					HAL_GPIO_TogglePin(GPIOB, EmbLED_Red_Pin);
					HAL_UART_Transmit(&huart6,"AT+CMGF=1\r\n",11, 1000);
					sim = response_ok;
					isim = sizeof( response_ok ) / sizeof( char ) -1;
				}
			}break;
			case prepare_message:
			{
				if ( vHardware_verify ( ) > 0 )
				{
					update_local_time();
					HAL_GPIO_TogglePin(GPIOB, EmbLED_Blue_Pin);//Init Ready
				}
			}break;
			case update_tim:
			{
				if ( vHardware_verify ( ) > 0 )
				{
					prepare_send_message();
				}
			}break;
			case send_message:
			{
				if ( vHardware_verify ( ) > 0 )
				{
					send_sms_me ();
				}				
			}break;
			case wait_send_message:
			{
				if ( vHardware_verify ( ) > 0 )
				{
					read_message();//send_sms();
					HAL_Delay ( 100 );
				}				
			}break;
			case tread_message:
			{
				vWait_message();				
			}break;
			case update_localtime:
			{
				update_localtimeStm();
			}break;
			
			case send_SMS_to:
			{
				if ( vHardware_verify ( ) > 0 )
				{
					prepare_send_message();
				}
			}break;
			
			case send_SMS_t:
			{	HAL_UART_Transmit(&huart3,"bota", 4, 1000);
				if ( vHardware_verify ( ) > 0 )
				{
					send_sms_ ();
				}
			}break;
			case SMS_sent:
			{
				if ( vHardware_verify ( ) > 0 )
				{
					local = discard_char;
					busy = 0;
					SMS_to_update = 0;
				}
			}break;
			case discard_char:
			{
				
			}break;
				
		}	
		UART6Tx_index++;
		UART6Tx_index &= ~(1<<9);		
	}
}
