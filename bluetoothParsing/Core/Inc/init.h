#ifndef __init_H
#define __init_H


#include "cmsis_os.h"


#define hour         61
#define day         ( hour   *  24 )
#define month       ( day    *  31 )
#define year        ( month  *  12 )


typedef struct message_info
{
	uint8_t message [256];
	uint8_t cardentials_twitter[16];
	uint8_t phone_number[13];
	uint8_t where;
	uint8_t to_publish;
}message_info;

extern xSemaphoreHandle release_signal;
extern xSemaphoreHandle sim1;
extern xSemaphoreHandle release_signal;
extern xSemaphoreHandle numPad;

extern int cp ;
extern volatile uint8_t ok;
extern uint8_t *pp;
extern uint8_t interrupt_count;
extern uint8_t max_interrupt;

extern uint8_t code_number;
extern char pin_code[5];
extern char choose_pin[5];
extern char connected_user [20];

extern message_info to_release[8];
extern uint8_t number_release;
extern message_info to_warning[8];
extern uint8_t number_warning; 

#endif
