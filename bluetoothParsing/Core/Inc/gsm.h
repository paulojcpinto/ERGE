#ifndef __gsm_H
#define __gsm_H

#include "usart.h"
#include "localtime.h"
#include "init.h"

static int p = 0;
static int *c = &p;

void parsing_gsm ( void );
void update_local_time ();
void send_SMS ( uint8_t number[13], uint8_t *messag, uint8_t size_message );

void parsing_gsm1 ( void );

#endif /*__gsm_H */
