#include "init.h"
#include "user.h"




xSemaphoreHandle finger_signal;
xSemaphoreHandle sim1;
xSemaphoreHandle release_signal;

volatile uint8_t ok;
uint8_t *pp = &ok;

int cp = 0;

uint8_t interrupt_count = 0;
uint8_t max_interrupt = 2;


message_info to_release[MAX_USERS];
uint8_t number_release;
message_info to_warning[MAX_USERS];
uint8_t number_warning;
