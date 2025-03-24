/*
 * 01.LED_CONTROL.c
 *
 * Created: 2025-03-04 오후 4:25:34
 * Author : microsoft
 */ 

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>
#include <stdio.h>		// printf
#include "button.h"
#include "led.h"
#include "uart0.h"

// extern----------------------------------------------------------
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
extern void pc_command_processing(void);

extern void init_ultrasonic(void);
extern void distance_ultrasonic(void);

extern int led_state;
extern void (*funcs[])(void);
extern volatile uint8_t rx_message_received;

extern void dht11_main(void);
// 선언 ----------------------------------------------------
void init_timer0(void);

volatile int msec_count = 0;
volatile int ultrasonic_check_timer = 0;

volatile int auto_mode = 1;
volatile int led_select = 0;


FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

// interrupt service routine은 반드시 main함수 이전에 정의한다.
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6; // 6 ~ 256으로 돌리기 위해
	msec_count++;

	ultrasonic_check_timer++; // 초음파센서에 활용할 타이머!
}



int main(void)
{
	init_timer0();
	init_uart0();
	init_ultrasonic();
	stdout = &OUTPUT;	
	sei();				

	dht11_main2();
	

	return 0;
}



// AVR의 8bit counter timer0를 초기화한다.
// 임베디드/FPGA에서 가장 중요한건 초기화 -> init함수는 특히 신경쓰기!!
void init_timer0(void)
{
	// 분주 (divider / prescale)
	// 16MHz / 64 down!!
	// 16000000Hz / 64 = 250,000 Hz 로 만든다!
	// T(주기) = 1clock에 걸리는 시간 = 1 / 250000 = 0.000004sec = 0.004ms (4us)
	// 8bit timer overflow 발생 시 소요시간 = 0.004ms * 256 = 1.024ms = 0.001024sec
	//	-> interrupt를 정확하게 1ms마다 오게 하고싶으면, 
	//    사용자 지정으로 256이 아니라 250클록마다 인터럽트가 생기도록
	
	// TCMT0 초기값 설정
	TCNT0 = 6; // 정확히 1ms를 세기 위해 초기값을 0이 아니라 6으로 해준다. (250 펄스 이후에 인터럽트)
	
	// 분주비 설정 (64)
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;
	
	// timer overflow interrupt 허용 (TIMER0 OVF)
	TIMSK |= 1 << TOIE0;	// 지역 인터럽트 허용

	
}

