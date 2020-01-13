#include "init.h"
#include "user.h"




xSemaphoreHandle release_signal;
xSemaphoreHandle sim1;
xSemaphoreHandle release_signal;
xSemaphoreHandle numPad;

volatile uint8_t ok;
uint8_t *pp = &ok;

int cp = 0;
uint8_t code_number=0;
char pin_code[5];
char choose_pin[5];

uint8_t interrupt_count = 0;
uint8_t max_interrupt = 2;

char connected_user [20];

message_info to_release[MAX_USERS];
uint8_t number_release;
message_info to_warning[MAX_USERS];
uint8_t number_warning;
