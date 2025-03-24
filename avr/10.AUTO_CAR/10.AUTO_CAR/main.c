#include "button.h"
#include <avr/interrupt.h>  // sei()등 
#include <stdio.h>  // printf scanf fgets puts gets 등이 들어 있다.
#include "extern.h"
#include "def.h"

volatile int msec_count=0;
volatile int ultrasonic_check_timer=0;

void init_timer0(void);
void manul_mode(void);
void distance_check(void);  // 초음파 거리 측정
void auto_mode(void);  // 자율 주행

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);


int func_index = MANUAL_MODE;
void (*pfunc[]) () =
{
	manul_mode,        // 수동 mode
	distance_check,    // 초음파 거리 측정
	auto_mode_check,   // button check
	auto_mode          // 자율 주행
};

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
	init_timer1();
	init_button();
	init_uart0();
	init_uart1();
	init_L298N();
	init_ultrasonic();
	stdout = &OUTPUT;   // printf가 동작 될 수 있도록 stdout에 OUTPUT 화일 포인터 assign 
	sei();              // 전역(대문)으로 interrupt 허용
	
	while(1)
	{
		//trigger_ultrasonic(); // 초음파 신호를 준다.
		manul_mode();
	}
	return 0;
}

void init_timer0(void)
{
	TCNT0=6;   // TCNT : 0~256 1ms 마다 TIMER0_OVF_vect로 진입 한다. 
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;  // TCCR0 |= 0xf4 보다는 죄측의 code 권장 
	TIMSK |= 1 << TOIE0;  // TIMSK |= 0x01; 
}

void manul_mode(void)  // 수동 mode
{
	switch(bt_data)
	{
		case 'F': // 전진
		case 'f':
			forward(2000);        // 4us  X   500 = 0.002sec (2ms)
			break;
		case 'B': // 후진
		case 'b':
			backward(2000);       // 4us  X   500 = 0.002sec (2ms)
			break;
		case 'L': // 좌회전
		case 'l':
			turn_left(2000);      
			break;
		case 'R': // 우회전
		case 'r':
			turn_right(2000);
			break;
			
		case 'S': // 정지
		case 's':
			stop();
			break;
		default: // 예외
			break;
	}
	
	func_index = DISTANCE_CHECK;
}
        
void distance_check(void)  // 초음파 거리 측정
{
	func_index = MODE_CHECK;
}

void auto_mode(void)  // 자율 주행
{
	func_index = MANUAL_MODE;
}
