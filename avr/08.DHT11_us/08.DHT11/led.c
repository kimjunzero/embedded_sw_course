﻿/*
 * led.c
 *
 * Created: 2025-03-05 오전 10:21:53
 *  Author: microsoft
 */ 



#include "led.h"

int led_main(void);   // 선언
void led_all_on(void);
void led_all_off(void);
void shift_left_ledon(void);
void shift_right_ledon(void);
void shift_left_keep_ledon(void);
void shift_right_keep_ledon(void);
void flower_on(void);
void flower_off(void);

extern volatile int msec_count; // extern으로 되어있으면 초기화 뺄것!
int led_state = 0;

void (*funcs[FUNC_NUM])(void) =
{
	led_all_on,
	led_all_off,
	shift_left_ledon,		
	shift_right_ledon,
	shift_left_keep_ledon,	
	shift_right_keep_ledon,
	flower_on,				
	flower_off
};

int led_main(void)   // 정의 
{
	DDRA = 0xff;   // PORTA에 연결된 pin8개를 output mode로 

	funcs[led_state]();
		
}

void state_transaction(void)
{
	led_state++;
	led_state%=FUNC_NUM;
}

void flower_on(void)
{
	static int i = 0;
	
	if(msec_count >= DELAY){
		msec_count = 0;
		if(i >= 4){
			i = 0;
			PORTA = 0;
			state_transaction();
		}else{
			PORTA |= 0x10 << i | 0x08 >> i++;
		}
	}
}

void flower_off(void)
{
	unsigned char h=0xf0;
	unsigned char l=0x0f;
	static int i = 0;
	
	if(msec_count >= DELAY){
		msec_count = 0;
		if(i >= 4){
			i = 0;
			PORTA = 0;
			state_transaction();
		}else{
			PORTA = (h >> i) & 0xf0 | (l << i++) & 0x0f;
		}
	}
}

void shift_left_keep_ledon(void)
{
	static int i = 0;
	
	if(msec_count >= DELAY){
		msec_count = 0;
		if(i >= 8){
			i = 0;
			PORTA = 0;
			state_transaction();
		}else{
			PORTA |= 0b00000001 << i++;
		}
	}
}
void shift_right_keep_ledon(void)
{
	static int i = 0;
	if(msec_count >= DELAY){
		msec_count = 0;
		if(i >= 8){
			i = 0;
			PORTA = 0;
			state_transaction();
		}else{
			PORTA |= 0b10000000 >> i++;
		}
	}	
}
void shift_left_ledon(void)
{
	static int i = 0;
	
	if(msec_count >= DELAY){
		msec_count = 0;
		if(i >= 8){
			i = 0;
			PORTA = 0;
			state_transaction();
		}else{
			PORTA = 0b00000001 << i++;
		}
	}

}

void shift_right_ledon(void)
{
	static int i = 0;
	if(msec_count >= DELAY){
		msec_count = 0;
		if(i >= 8){
			i = 0;
			PORTA = 0;
			state_transaction();
		}else{
			PORTA = 0b10000000 >> i++;
		}
	}
}

void led_all_on(void)
{
	PORTA = 0xff;
}

void led_all_off(void)
{
	PORTA = 0x00;
}