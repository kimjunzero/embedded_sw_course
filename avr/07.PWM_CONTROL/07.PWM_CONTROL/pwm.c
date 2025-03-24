/*
 * pwm.c
 *
 * Created: 2025-03-13 오후 12:49:07
 *  Author: microsoft
 */ 
#include "pwm.h"

extern void init_button(void);
extern int get_button(int button_num, int button_pin);

void init_timer3(void);
void pwm_fan_control_main(void);
void L298N_pwm_fan_control_main(void);
void init_L298N(void);
/*
	16비트 timer 3번 활용
	PWM 출력 신호
	=============
	PE3: 0C3A
	PE4: OC3B / INT4  ---> 사용중(초음파 echo펄스 externel INT용)
	PE5: 0C3C ---> 모터 연결
	BTN0: start/stop
	BTN1: speed-up(OC3C: 20씩 증가 max 250)
	BTN2: speed-down(OC3C: 20씩 감소 min 60)
*/
void init_timer3(void)
{
	DDRE |= 1 << 3 | 1 << 5;  // 현재 4핀은 초음파 echo
	// 16 timer 3번의 모드 5: 8비트 고속 PWM (P348 표 15-6)
	TCCR3A |= 1 << WGM30;
	TCCR3B |= 1 << WGM32;
	
	// 비반전 모드
	// 사용자가 설정한 PWM값에 일치시 OC3C의 출력핀이 LOW로 바뀌고 BOTTOM에서 HIGH로 바뀐다.
	// P350 표15-7
	TCCR3A |= 1 << COM3C1;
	// 분주비 : 64.  16000000 / 64 ---> 2500000Hz
	// T = 1/f = 1/2500000Hz ==> 0.000004sec(4us)
	// 2500000Hz에서 256개의 펄스를 count하면 소요시간 1.02ms
	//               127개                           0.5ms
	// 표318 표 14-1
	TCCR3B |= 1 << CS31 | 1 << CS30; // 분주비 : 64
	OCR3C = 0;  // OCR(Output Compare Register) : PWM 값
}

// PF6 : IN1(L298N)
// PF7 : IN2(L298N)
// PF6(IN1) P7(IN2)
//    0        1   : 역회전
//    1        0   : 정회전
//    1        1   : STOP
void init_L298N(void)
{
	DDRF |= 1 << 6 | 1 << 7; // 출력 모드로 설정
	PORTF &= ~(1 << 6 | 1 << 7);
	PORTF |= 1 << 6; // 정회전
}

// PE5 모터 연결
void pwm_fan_control_main(void)
{
	uint8_t start_button = 0;
	
	init_button();
	init_timer3();
	
	while(1)
	{
		if(get_button(BUTTON0, BUTTON0PIN)) // start/stop
		{
			start_button = !start_button; // toggle
			OCR3C = (start_button) ? 250 : 0; // 250 : 모터 회전의 기본 값, 0: stop
		}
		else if(get_button(BUTTON1, BUTTON1PIN)) // speed-up
		{
			OCR3C = (OCR3C >= 250) ? 250 : OCR3C + 20;
		}
		else if(get_button(BUTTON2, BUTTON2PIN)) // speed-up
		{
			OCR3C = (OCR3C <= 70) ? 60 : OCR3C - 20;
		}
	}
}

void L298N_pwm_fan_control_main(void)
{
	uint8_t start_button = 0;
	uint8_t forward = 1;  // forward <---> backward
	
	init_button();
	init_timer3();
	init_L298N();
	
	while(1)
	{
		if(get_button(BUTTON0, BUTTON0PIN)) // start/stop
		{
			start_button = !start_button; // toggle
			OCR3C = (start_button) ? 250 : 0; // 250 : 모터 회전의 기본 값, 0: stop
		}
		else if(get_button(BUTTON1, BUTTON1PIN)) // speed-up
		{
			OCR3C = (OCR3C >= 250) ? 250 : OCR3C + 20;
		}
		else if(get_button(BUTTON2, BUTTON2PIN)) // speed-down
		{
			OCR3C = (OCR3C <= 70) ? 60 : OCR3C - 20;
		}
		else if(get_button(BUTTON3, BUTTON3PIN)) // 방향 설정
		{
			forward = !forward;
			PORTF &= ~(1 << 6 | 1 << 7); // IN1 IN2 reset
			PORTF = (forward) ? (PORTF |= 1 << 6) : (PORTF |= 1 << 7);
			//                   역회전                정회전
		}
	}
}