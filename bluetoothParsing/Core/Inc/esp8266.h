#ifndef __esp_H
#define __esp_H


#include "usart.h"
#include "localtime.h"
#include <gpio.h>
#include "bluetooth_module.h"

extern volatile uint8_t busy;
extern void parsing_esp( void );
extern void  wait1 ( void );
extern void publish_twitter ( uint8_t message_release[], uint8_t who_publish[] );
extern void ini ();
extern void publ();
#endif
