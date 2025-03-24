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

extern volatile uint8_t rx_buff[80];   // uart0로 부터 들어온 문자를 저장 하는 버퍼(변수)
extern volatile uint8_t rx_msg_received;
extern int mode;
volatile int msec_count=0;
volatile int ultrasonic_check_timer=0;
extern volatile char scm[50];
// for printf 
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);
// volatile 변수 type앞에 volatile을 선언 하는 이유: compiler한테 최적화 방지 지시 
// cc -O
/*
예) 1~10 합을 구하는 프로그램
  int i=0; int sum=0;
  while (i <= 10)
  {
	  i++; sum += i;
  }
  printf("sum--> %d\n", sum);   // 1~10 : 55가 된다.
  
   volatile int i=0; volatile int sum=0;
    while (i <= 10)
    {
	    i++; sum += i;
    }
    printf("sum--> %d\n", sum);   // 1~10 : 55가 된다.
*/
// TIMER0_OVF_vect : P278 표12-3
// ISR(Interrupt Service Routine(함수)
// --> H/W가 SW한테 event(상황변화)가 일어 났다고 알려 주는 공간 
// 250개의 pulse를 count(1ms)하면 이곳으로 자동적으로 들어 온다. 
// ISR루틴(함수)은 가능한 짧게 작성 한다.

int led_toggle=0; 
ISR(TIMER0_OVF_vect)
{
	// 6~256 : 250(1ms) 그래서 TCNT0를 6으로 설정하는것이다. 
	TCNT0=6;
	msec_count++;   // 1ms마다 1씩 증가
	ultrasonic_check_timer++;
}

void (*fp1[]) (void) =
{
	led_all_off,
	led_all_on,
	shift_left_ledon,
	shift_right_ledon,
	shift_left_keep_ledon,
	shift_right_keep_ledon,
	flower_on,
	flower_off
};

int main(void)
{
	init_timer0();
	init_uart0();
	init_ultrasonic();
	stdout = &OUTPUT;   // printf가 동작 될 수 있도록 stdout에 OUTPUT 화일 포인터 assign 
	sei();   // 전역(대문)으로 interrupt 허용
		
	while(1)
	{
		printf("%s", scm);
		_delay_ms(500);
	}
	
	return 0;
}
// none O/S 또는 loop monitor방식 
#if 0
int main(void)
{
	int button0_state=0;  // 초기 상태를 0으로 출발
	
	init_timer0();
	
	led_main();
    fnd_main();
	
	void (*fp[]) (void) =
	{
		led_all_off,
		led_all_on,
		shift_left_ledon,
		shift_right_ledon,
		shift_left_keep_ledon,
		shift_right_keep_ledon,
		flower_on,
		flower_off
	};
	
	init_button();
	// led_main();    
	       //76543210    
	DDRA = 0b11111111;   // PORTA를 출력 모드(1)로 설정
	       //---- 상위 nibble : 상위 4bits
		   //    ---- 하위 nibble 
	                     // DDR(Data Direction Register) : 방향 설정 
						 // 1: 출력 0: 입력을 의미 
						 // 0b : 2진수
						 // 0x : hex 
						 // DDRA = 0xff;
#if 0   // 함수 포인터 배열 
	while (1)
	{
		if (get_button(BUTTON0, BUTTON0PIN))
		{
			button0_state++;
			button0_state %= 8;
		}
		fp[button0_state] ();
	}
#endif

#if 0   // switch ~ case 
	while (1)
	{
		if (get_button(BUTTON0, BUTTON0PIN))
		{
			button0_state++;
			button0_state %= 8;
		}
		switch (button0_state) {
		case 0:
			led_all_off();
			break;
		case 1:
			led_all_on();
			break;
		case 2:
			shift_left_ledon();
			break;
		case 3:
			shift_right_ledon();
			break;
		case 4:
			shift_left_keep_ledon();
			break;
		case 5:
			shift_right_keep_ledon();
			break;
		case 6:
			flower_on();
			break;
		case 7:
			flower_off();
			break;
		}
	}
#endif

#if 0	// org
    while (1)   // for(;;) 
    {
		// 1 button버튼 처리 (toggle)
		// button0를 1번 누르면 led_all_on
		//                      led_all_off
		if (get_button(BUTTON0, BUTTON0PIN))
		{
			button0_state = !button0_state;  // 반전 0 <--> 1
			if (button0_state)
				led_all_on();
			else led_all_off();
		}
    }
#endif 
}
#endif 

// timer0를 초기화 시키는 부분
// AVR에서 8bit timer 0/2번중 0번을 초기화 한다.
// 임베디드/FPGA 등에서 제일 중요한것은 초기화를 정확히 해주는것이다. 
// 그래서 이부분을 특별히 신경을 써서 작성 한다. 

void init_timer0(void)
{
// 16MHZ /64 분주(down) 분주: divider/prescale
// ------ 분주비 계산 ----
// (1) 16000000Hz/64 ==> 250,000HZ
// (2) T(주기) 1clock의 소요시간 : 1/f = 1/250,000 ==> 0.0000004sec(4us) : 0.004ms	
// (3) 8bit timer OV(OVflow) : 0.004ms x 256 = 0.001024sec --> 1.024ms 
// 1ms마다 정확하게 INT를 띄우고 싶은면 0.004ms x 250개를 count = 0.001sec ==>1ms
	TCNT0=6;   // TCNT : 0~256 1ms 마다 TIMER0_OVF_vect로 진입 한다. 
	           // TCNT0 = 6으로 설정을 한 이유: 6-->256 : 250개의 펄스를 count하기 때문에 정확히 1ms가 된다.
// (4) 분주비 설정 64분주 (250,000HZ --> 250KHz) P296 표13-1
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;  // TCCR0 |= 0xf4 보다는 죄측의 code 권장 
// (5) Timer0 overflow INT를 허용(enable)
	TIMSK |= 1 << TOIE0;  // TIMSK |= 0x01; 

}
