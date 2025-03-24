/*
 * pwm.h
 *
 * Created: 2025-03-13 오후 12:48:55
 *  Author: microsoft
 */ 


#ifndef PWM_H_
#define PWM_H_
#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <stdio.h>		 // printf scanf fgets puts gets 등이 들어있다.

#define MOTOR_PWM_DDR			DDRB
#define MOTOR_LEFT_PORT_DDR		5		// OC1A
#define MOTOR_RIGHT_PORT_DDR	6		// OC1B

#define MOTOR_DRIVER_DIRECTION_PORT PORTF
#define MOTOR_DRIVER_DIRECTION_DDR  DDRF

#include "button.h"

#endif /* PWM_H_ */