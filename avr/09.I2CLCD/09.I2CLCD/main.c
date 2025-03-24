/*
 * 01.LED_CONTROL.c
 *
 * Created: 2025-03-04 오후 4:25:34
 * Author : microsoft
 */ 

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>  // sei()등 
#include <stdio.h>  // printf scanf fgets puts gets 등이 들어 있다. 
#include "button.h"
extern int led_main(void);   // 선언
extern void init_button(void);
extern int get_button(int button_num, int button_pin);
extern void led_all_on(void);
extern void led_all_off(void);
extern void shift_left_ledon(void);
extern void shift_right_ledon(void);
extern void shift_left_keep_ledon(void);
extern void shift_right_keep_ledon(void);
extern void flower_on(void);
extern void flower_off(void);
extern int fnd_main(void);
extern void init_uart0(void);
extern void UART0_transmit(uint8_t data);
extern void distance_ultrasonic();
extern void init_ultrasonic();
extern void init_dht11(void);
extern void dht11_main(void);
extern volatile uint8_t rx_buff[80];   // uart0로 부터 들어온 문자를 저장 하는 버퍼(변수)
extern volatile uint8_t rx_msg_received;
extern int mode;
volatile int msec_count=0;
volatile int ultrasonic_check_timer=0;
extern void I2C_LCD_Test();

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

int led_toggle=0; 
ISR(TIMER0_OVF_vect)
{
	// 6~256 : 250(1ms) 그래서 TCNT0를 6으로 설정하는것이다. 
	TCNT0=6;
	msec_count++;   // 1ms마다 1씩 증가
	ultrasonic_check_timer++;
}

int main(void)
{
	init_timer0();
	init_uart0();
	init_ultrasonic();
	stdout = &OUTPUT;   // printf가 동작 될 수 있도록 stdout에 OUTPUT 화일 포인터 assign 
	sei();   // 전역(대문)으로 interrupt 허용
	I2C_LCD_Test();
	//dht11_main();
	
	return 0;
}
// none O/S 또는 loop monitor방식 

void init_timer0(void)
{
	TCNT0=6;   // TCNT : 0~256 1ms 마다 TIMER0_OVF_vect로 진입 한다. 
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;  // TCCR0 |= 0xf4 보다는 죄측의 code 권장 
	TIMSK |= 1 << TOIE0;  // TIMSK |= 0x01; 

}
