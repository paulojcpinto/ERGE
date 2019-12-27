#include "init.h"


xSemaphoreHandle finger_signal;
xSemaphoreHandle sim1;
volatile uint8_t ok;
uint8_t *pp = &ok;

uint8_t interrupt_count = 0;
uint8_t max_interrupt = 2;
