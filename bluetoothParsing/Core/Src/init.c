#include "init.h"


xSemaphoreHandle finger_signal;
volatile uint8_t ok;
uint8_t *pp = &ok;