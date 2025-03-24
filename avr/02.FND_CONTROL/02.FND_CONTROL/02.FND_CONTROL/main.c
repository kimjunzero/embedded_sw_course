/*
 * 01.LED_CONTROL.c
 *
 * Created: 2025-03-04 오후 4:25:04
 * Author : microsoft
 */ 

#include "button.h"
#include "led.h"
#include "fnd.h"

#define F_CPU 16000000UL // 16MHZ
#include <avr/io.h>
#include <util/delay.h> // _delay_ms _delay_us

// none O/S 또는 loop monitor방식
int main(void)
{
	int button0_state = 0;  // 초기 상태를 0으로 출발
	init_fnd();
	void (*fp[]) (void) = // 함수 포인터 배열
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
	
	//int button1_state = 0;
	//int button2_state = 0;
	//int button3_state = 0;
	//int button_state[4] =0
	init_button();
	// led_main();
	       //76543210
    DDRA = 0b11111111; // PORTA를 출력 모드(1)로 설정
	       //----상위 nibble : 상위 4bits
		   //    ---- 하위 nibble 4bits
	                   // DDR(Data Direction Register) : 방향 설정
					   // 1 : 출력 0 : 입력을 의미
					   // 0b : 2진수
					   // 0x : hex
					   // DDRA = 0xff;
#if 1 // 버튼 제어
while(1)
{
	if(get_button(BUTTON0, BUTTON0PIN))
	{
		button0_state = !button0_state;
	
		if(button0_state)
			fnd_mode1();
		else
			fnd_mode2();
	}
}
#endif
#if 0 // 함수 포인터 배열
while (1) // for(;;)
{
	if(get_button(BUTTON0, BUTTON0PIN))
	{
		button0_state++;
		button0_state %= 8;
	}
	fp[button0_state]();
}
#endif
#if 0 // switch ~ case
 while (1) // for(;;)
 {
	 if(get_button(BUTTON0, BUTTON0PIN))
	 {
		 button0_state++;
		 button0_state %= 8;
	 }
	 switch(button0_state){
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
#if 0 // 0~4번까지 버튼
    while (1) // for(;;)
    {
		// 1 button버튼 처리(toggle)
		// button0를 1번 누르면 led_all_on
		//                      led_all-off
		
		if(get_button(BUTTON0, BUTTON0PIN))
		{
			button0_state = !button0_state; // 반전 0 <--> 1
			if(button0_state == 1)
				led_all_on();
			else 
				led_all_off();
		}
		else if(get_button(BUTTON1, BUTTON1PIN))
		{
			button1_state = !button1_state; // 반전 0 <--> 1
			if(button1_state == 1)
				shift_left_ledon();
			else
				shift_right_ledon();
		}
		else if(get_button(BUTTON2, BUTTON2PIN))
		{
			button2_state = !button2_state; // 반전 0 <--> 1
			if(button2_state == 1)
				shift_left_keep_ledon();
			else
				shift_right_keep_ledon();
		}
		else if((get_button(BUTTON3, BUTTON3PIN)))
		{
			button3_state = !button3_state; // 반전 0 <--> 1
			if(button3_state == 1)
				flower_on();
			else
				flower_off();
		}
    }
#endif
	
	return 0; // main 함수는 int type를 리턴하는 함수
	          // 0의 의미는 error없이 정상 종료
}

