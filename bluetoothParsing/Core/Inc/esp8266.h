#ifndef __esp_H
#define __esp_H


#include "usart.h"
#include "localtime.h"
#include <gpio.h>
extern volatile uint8_t busy;
extern void parsing_gsm11();
extern void  wait1 ();


#endif