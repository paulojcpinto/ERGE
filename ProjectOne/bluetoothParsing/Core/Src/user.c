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
	
#define char_trama_pincode            (char)    'P'
#define int_pincode                   (int)     6
	
#define char_trama_email               (char)  'M'
#define int_email                      (int)    7
	


static int out_index = 0;
int error_number = 0;

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
						char * aux;
					   aux[0]='<';
					   aux[1]=c;
					   aux[2]=s[0];
					   aux[3]='>';
					   HAL_UART_Transmit_IT(&huart4, aux ,4 );
					}
					
					else
					{
						UlToStr(s,out_index,2);
						char * aux;
					  aux[0]='<';
					  aux[1]=c;
					  aux[2]=s[0];
					  aux[3]=s[1];
					  aux[4]='>';
					  HAL_UART_Transmit_IT(&huart4, aux ,5 );
						
					}
					
	       

					UART3Tx_index++;
					
	
}




void receive_echo ( user *me, int *c )
{
	*c = int_echo;
//	HAL_UART_Transmit_IT(&huart4, "okoko", 5);
	me->nickName[out_index++] = char_trama_init;
	me->nickName[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
}

void receive_start ( user *me, int *c )
{
	*c = int_start;
	//me->nickName[out_index++] = char_trama_init;
	//me->nickName[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
	UART3Tx_index++;
}

void prepare_receive_info( user *me, int *c )
{
	switch ( UART3Rx_Buffer[UART3Tx_index] )
	{
		case char_trama_echo:
		{
			*c= 3;
			receive_echo (me, c);
		}; break;
		
		case char_trama_start:
		{
			*c = 2;
			receive_start (me, c);
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
				case int_echo:
				{
					HAL_Delay(100);
					me->nickName[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
					HAL_UART_Transmit_IT(&huart4, me->nickName, out_index);

				}; break;
				
				case int_start:
				{
					//HAL_Delay(100);
					me->nickName[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
					HAL_UART_Transmit_IT(&huart4, " STM", sizeof(" STM")/sizeof(char));
		


				}; break;
				case int_error:
						//HAL_Delay(5000);
						HAL_UART_Transmit_IT(&huart4, me->nickName, out_index);
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
				
	*c = -1;
				}
			}

void save_char ( user *me, int *c )
{
	switch (*c)
			{
				case int_echo:
				{
					me->nickName[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
				}; break;
				case int_error:
				{
					char aux = UART3Rx_Buffer[UART3Tx_index++];
				//	error_number += (aux-48)+10^(UART3Tx_index-3);
				}break;
				case int_nick:
				{
					me->nickName[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
				};break;
				case int_pincode:
					me->pinCode[out_index++] = UART3Rx_Buffer[UART3Tx_index++];
					break;
				case int_email:
					me->email[out_index++]= UART3Rx_Buffer[UART3Tx_index++];
					break;
				
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
			out_index = 0;

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
