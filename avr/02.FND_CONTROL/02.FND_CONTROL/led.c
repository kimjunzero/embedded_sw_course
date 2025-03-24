/*
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

int led_main(void)   // 정의 
{
	DDRA = 0xff;   // PORTA에 연결된 pin8개를 output mode로 
	
	while(1) 
	{
		led_all_on();
		_delay_ms(100);
		led_all_off();
		_delay_ms(100);
		shift_left_ledon();
		shift_right_ledon();
		shift_left_keep_ledon();  // _delay_ms(200)
		shift_right_keep_ledon();
		flower_on(); 
		flower_off();
	}	
}

void flower_on(void)
{
	PORTA=0;
	for (int i=0; i < 4; i++)
	{
		PORTA |= 0x10 << i | 0x08 >> i;
		_delay_ms(30);
	}
	
}

void flower_off(void)
{
	unsigned char h=0xf0;
	unsigned char l=0x0f;
	
	PORTA=0;
	for (int i=0; i < 4; i++)
	{
		PORTA = (h >> i) & 0xf0 | (l << i) & 0x0f;
		_delay_ms(30);
	}
	PORTA=0;
	_delay_ms(30);
}

void shift_left_keep_ledon(void)
{
	for (int i=0; i < 8; i++)
	{
		PORTA |= 0b00000001 << i;
		//               1 i=0;
		//        00000010 i=1;
		//        10000000 i=7;
		_delay_ms(30);
	}	
}
void shift_right_keep_ledon(void)
{
	PORTA=0;
	for (int i=0; i < 8; i++)
	{
		PORTA |= 0b10000000 >> i;
		//        10000000 i=0;
		//        01000000 i=1;
		_delay_ms(30);
	}	
}
void shift_left_ledon(void)
{
	for (int i=0; i < 8; i++)
	{
		PORTA = 0b00000001 << i;
		//               1 i=0;
	    //        00000010 i=1;
	    //        10000000 i=7;
		_delay_ms(30);
	}	
}

void shift_right_ledon(void)
{
	PORTA=0;
	for (int i=0; i < 8; i++)
	{
		PORTA = 0b10000000 >> i;
		//        10000000 i=0;
		//        01000000 i=1;
		_delay_ms(30);
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