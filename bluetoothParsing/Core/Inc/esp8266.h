#ifndef __esp_H
#define __esp_H


#include "usart.h"
#include "localtime.h"
#include <gpio.h>

extern volatile uint8_t busy;
extern void parsing_gsm11( void );
extern void  wait1 ( void );
extern void publish_twitter ( int i );

#endif
