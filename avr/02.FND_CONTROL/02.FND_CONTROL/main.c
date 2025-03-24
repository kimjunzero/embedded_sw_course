/*
 * 01.LED_CONTROL.c
 *
 * Created: 2025-03-04 오후 4:25:34
 * Author : microsoft
 */ 

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
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

// none O/S 또는 loop monitor방식 
int main(void)
{
	int button0_state=0;  // 초기 상태를 0으로 출발
	
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
#if 1   // 함수 포인터 배열 
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

