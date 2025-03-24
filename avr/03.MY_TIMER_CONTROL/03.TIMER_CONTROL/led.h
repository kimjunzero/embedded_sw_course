/*
 * led.h
 *
 * Created: 2025-03-05 오전 10:21:21
 *  Author: microsoft
 */ 

#ifndef LED_H_
#define LED_H_
#define FUNC_NUMBER 6
#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us

int led_main(void);   // 선언
void led_all_on(void);
void led_all_off(void);
// 자동 모드
void shift_left_ledon1(void);
void shift_right_ledon1(void);
void shift_left_keep_ledon1(void);
void shift_right_keep_ledon1(void);
void flower_on1(void);
void flower_off1(void);

// 수동 모드
void shift_left_ledon2(void);
void shift_right_ledon2(void);
void shift_left_keep_ledon2(void);
void shift_right_keep_ledon2(void);
void flower_on2(void);
void flower_off2(void);

void state_transition(void);

extern volatile int msec_count;

#endif /* LED_H_ */