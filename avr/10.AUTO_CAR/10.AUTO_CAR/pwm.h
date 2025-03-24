#ifndef PWM_H_
#define PWM_H_

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>  // PORTA PORTB등의 I/O 관련 register등이 있다.
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>
#include <stdio.h>

#define MOTOR_PWM_DDR   DDRB
#define MOTOR_LEFT_PORT_DDR   5  // OC1A
#define MOTOR_RIGHT_PROT_DDR 6  // OC1B

#define MOTOR_DRIVER_DIRECTION_PORT PORTF
#define MOTOR_DRIVER_DIRECTION_DDR  DDRF

#include "button.h"

#endif /* PWM_H_ */