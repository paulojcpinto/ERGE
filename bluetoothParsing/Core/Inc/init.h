#ifndef __init_H
#define __init_H


#include "cmsis_os.h"


typedef struct message_info
{
	uint8_t message [256];
	uint8_t cardentials_twitter[16];
	uint8_t phone_number[13];
	uint8_t where;
}message_info;

extern xSemaphoreHandle finger_signal;
extern xSemaphoreHandle sim1;
extern xSemaphoreHandle release_signal;

extern int cp ;
extern volatile uint8_t ok;
extern uint8_t *pp;
extern uint8_t interrupt_count;
extern uint8_t max_interrupt;


extern message_info to_release[5];
extern uint8_t number_release;
extern message_info to_warning[5];
extern uint8_t number_warning; 

#endif
