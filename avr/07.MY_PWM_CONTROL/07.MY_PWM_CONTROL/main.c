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
void pwm_cnt(void);
extern int led_main(void);   // 선언
extern void init_button(void);
extern void init_fnd(void);
extern int get_button(int button_num, int button_pin);
extern void pwm_cnt(void);
extern void fnd_display1(void);
extern void fnd_display2(void);
extern void make_pwm_led_control(void);
extern void L298N_pwm_fan_control_main(void);
extern volatile uint8_t rx_buff[80];   // uart0로 부터 들어온 문자를 저장 하는 버퍼(변수)
extern volatile uint8_t rx_msg_received;
volatile int msec_count=0;
volatile uint32_t sec_count = 0;  // 초를 재는 count변수 unsigned int --> uint32_t

ISR(TIMER0_OVF_vect)
{
	TCNT0=6;
	msec_count++;   // 1ms마다 1씩 증가
}

int main(void)
{
	init_fnd();
	init_timer0();
	init_button();
	init_timer3();
	init_L298N();
	
	sei();   // 전역(대문)으로 interrupt 허용
	
	while(1)
	{
		pwm_cnt();
		fnd_display1();
		_delay_ms(1);
		fnd_display2();
		_delay_ms(1);
		L298N_pwm_fan_control_main();
	}
	
	return 0;
}
void init_timer0(void)
{
	TCNT0 = 6;   
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;  // TCCR0 |= 0xf4 보다는 죄측의 code 권장 
	TIMSK |= 1 << TOIE0;  // TIMSK |= 0x01; 
}
void pwm_cnt(void)
{
	if(msec_count >= 1000)
	{
		msec_count = 0;
		sec_count++;
	}
}