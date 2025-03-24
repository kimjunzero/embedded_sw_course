/*
 * ultrasonic.h
 *
 * Created: 2025-03-12 오후 2:49:06
 *  Author: microsoft
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>
#include <stdio.h>

#define TRIG 0  // echo pin 번호
#define TRIG_PORT PORTG
#define TRIG_DDR DDRG

#define ECHO 4 // echo pin 번호
#define ECHO_PIN PINE // External INT 4
#define ECHO_DDR DDRE

//main.c
extern volatile int ultrasonic_check_timer;

//ultrasoni.c

void init_ultrasonic(void);
void trigger_ultrasonic(void);
void ultrasonic_distance_check(void);




//new_ultrasonic.c

//=====Left======

#define Ultra_TCNT_LEFT TCTN2	//8bit timer2 counter
#define TRIG_LEFT 0				//TRIG==>PA0
#define TRIG_DDR_LEFT DDRA		//output mode 설정
#define TRIG_PORT_LEFT PORTA	//data
//HW external INT: PE4
#define ECHO_LEFT 4				//ECHO==>PE4
#define ECHO_DDR_LEFT DDRE		//output mode 설정	
#define ECHO_PIN_LEFT PINE		//data

//=====Center======
#define Ultra_TCNT_CENTER TCTN2	//8bit timer2 counter
#define TRIG_CENTER 1				//TRIG==>PA1
#define TRIG_DDR_CENTER DDRA		//output mode 설정
#define TRIG_PORT_CENTER PORTA	//data
//HW external INT: PE5
#define ECHO_CENTER 5				//ECHO==>PE5
#define ECHO_DDR_CENTER DDRE		//output mode 설정
#define ECHO_PIN_CENTER PINE		//data

//=====Right======
#define Ultra_TCNT_RIGHT TCTN2	//8bit timer2 counter
#define TRIG_RIGHT 2				//TRIG==>PA2
#define TRIG_DDR_RIGHT DDRA		//output mode 설정
#define TRIG_PORT_RIGHT PORTA	//data
//HW external INT: PE6
#define ECHO_RIGHT 6				//ECHO==>PE6
#define ECHO_DDR_RIGHT DDRE		//output mode 설정
#define ECHO_PIN_RIGHT PINE		//data

volatile int left_distance;
volatile int middle_distance;
volatile int right_distance;

extern int warning_left;
extern int warning_right;
extern int warning_middle;


#endif /* ULTRASONIC_H_ */