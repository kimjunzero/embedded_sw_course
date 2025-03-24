/*
 * led.c
 *
 * Created: 2025-03-05 오전 10:21:53
 *  Author: microsoft
 */ 

#include "led.h"

void init_led(void);

void init_led(void)
{
	AUTO_RUN_LED_PORT_DDR |= 1 << AUTO_RUN_LED_PIN;
}