/*
 * led.h
 *
 * Created: 2025-03-05 오전 10:21:21
 *  Author: microsoft
 */ 

#ifndef LED_H_
#define LED_H_

#include <avr/io.h>  // PORTA PORTB등의 I/O 관련 register등이 있다. 

#define AUTO_RUN_LED_PORT PORTG
#define AUTO_RUN_LED_PORT_DDR DDRG
#define AUTO_RUN_LED_PIN 3

#endif /* LED_H_ */