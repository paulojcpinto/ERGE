#include "user.h"

#define discard_char					UART3Tx_index++

#define char_trama_init 							( char )		'<' 
#define char_trama_end  							( char ) 		'>'	
	
#define char_trama_nickName 					( char ) 		'N'
#define int_nickName									(  int )		 3
	
#define char_trama_start 							( char ) 		'L'
#define int_start											(  int )		 2
	
#define char_trama_error              (char)      'E'
#define int_error											(  int )		 4
	


static int out_index = 0;
int error_number = 0;

void receive_nickName ( user *me, int *c )
{
	*c = int_nickName;
//	HAL_UART_Transmit_IT(&huart4, "okoko", 5);
	me->nickName[out_index++] = char_trama_init;
	me->nickName[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
}

void receive_start ( user *me, int *c )
{
	*c = int_start;
	me->nickName[out_index++] = char_trama_init;
	me->nickName[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
}

void prepare_receive_info( user *me, int *c )
{
	switch ( UART3Rx_Buffer[UART3Tx_index] )
	{
		case char_trama_nickName:
		{
			*c= 3;
			out_index = 0;

			receive_nickName (me, c);
		}; break;
		
		case char_trama_start:
		{
			*c = 2;
			receive_start (me, c);
		}; break;
		case char_trama_error:
		{
			*c=int_error;
			UART3Tx_index++;
			error_number=0;
		};break;
	}
}

void end_receiving_trama ( user *me, int *c)
{
	switch (*c)
			{
				case int_nickName:
				{
					HAL_Delay(100);
					me->nickName[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
					HAL_UART_Transmit_IT(&huart4, me->nickName, out_index);

				}; break;
				
				case int_start:
				{
					//HAL_Delay(100);
					me->nickName[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
					HAL_UART_Transmit_IT(&huart4, " STM#", sizeof(" STM#")/sizeof(char));
		


				}; break;
				case int_error:
						//HAL_Delay(5000);
						HAL_UART_Transmit_IT(&huart4, me->nickName, out_index);
					break;
			}
	*c = -1;
}

void save_char ( user *me, int *c )
{
	switch (*c)
			{
				case int_nickName:
				{
					me->nickName[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
				}; break;
				case int_error:
				{
					char aux = UART3Rx_Buffer[UART3Tx_index++];
				//	error_number += (aux-48)+10^(UART3Tx_index-3);
				}break;
			}
			
}

int getNickName (user *me, int *c)
{

	
	while(UART3Tx_index != UART3Rx_index)
	{
		if(UART3Rx_Buffer[UART3Tx_index] == char_trama_init)
		{
			*c = 1;
			UART3Tx_index ++;
		}
		else if (*c == 1)
		{
			prepare_receive_info( me, c);
		}
		else if (UART3Rx_Buffer[UART3Tx_index] == char_trama_end)
		{
			end_receiving_trama ( me, c);
		}
		else if (*c == 2)
		{
			*c = -1;
		}
		else if (*c > 2)			
		{
			save_char( me, c );
		}
		else
			discard_char;
		
		out_index &= ~(1<<7);
		UART3Tx_index &= ~(1<<7);
	}
	return *c;
}
