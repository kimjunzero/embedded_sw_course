/*
 * led.h
 *
 * Created: 2025-03-05 오전 10:21:23
 *  Author: microsoft
 */ 


#ifndef LED_H_ // 조건부 컴파일
#define LED_H_
#define F_CPU 16000000UL // 16MHZ
#include <avr/io.h>
#include <util/delay.h> // _delay_ms _delay_us
int led_main(void);  // 선언 void led_all_on(void);
void led_all_on(void);
void led_all_off(void);
void shift_left_ledon(void);
void shift_right_ledon(void);
void shift_left_keep_ledon(void);
void shift_right_keep_ledon(void);
void flower_on(void);
void flower_off(void);
#endif /* LED_H_ */