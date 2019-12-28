#ifndef __init_H
#define __init_H


#include "cmsis_os.h"

extern xSemaphoreHandle finger_signal;
extern xSemaphoreHandle sim1;
extern xSemaphoreHandle release_signal;

extern volatile uint8_t ok;
extern uint8_t *pp;
extern uint8_t interrupt_count;
extern uint8_t max_interrupt;


#endif
