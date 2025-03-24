/*
 * led.c
 *
 * Created: 2025-03-05 오전 10:21:53
 *  Author: microsoft
 */ 

#include "led.h"
#include "button.h"

#define FUNC_NUMER 6

int state=0;

// 자동 포인터 배열
void (*fp1[]) () =
{
	shift_left_ledon1,
	shift_right_ledon1,
	shift_left_keep_ledon1,
	shift_right_keep_ledon1,
	flower_on1,
	flower_off1
};

// 수동 포인터 배열
void (*fp2[]) () = 
{
	shift_left_ledon2,
	shift_right_ledon2,
	shift_left_keep_ledon2,
	shift_right_keep_ledon2,
	flower_on2,
	flower_off2
};

// 0이면 자동, 1이면 수동
int mode = 0;
int current_state = 0;

int led_main(void)
{
	DDRA = 0xff;   // PORTA에 연결된 pin8개를 output mode로
	
	while(1)
	{
		if(get_button(BUTTON0, BUTTON0PIN))
		{
			mode = !mode;
		}
		
		if(mode == 0) // 자동
		{
			fp1[state]();
		}
		else // 수동
		{
			if (get_button(BUTTON1, BUTTON1PIN))
			{
				current_state++;
				current_state %= 6;
			}
			fp2[current_state] ();
			
		}
	}
}

// 수동
void flower_on1(void)
{
	static int i=0;

	if (msec_count >= 100)
	{
		msec_count=0;
		if (i >= 4)
		{
			i=PORTA=0x00;
			if(mode == 0)
				state_transition();   // state를 천이 to shift_right_keep_ledon
		}
		else
		{
			PORTA |= 0x10 << i | 0x08 >> i++;
		}
	}
}
// 자동
void flower_on2(void)
{
    // org 
	PORTA=0;
	for (int i=0; i < 4; i++)
	{
		PORTA |= 0x10 << i | 0x08 >> i;
		_delay_ms(30);
	}
}
// 수동
void flower_off1(void)
{
	unsigned char h=0xf0;
	unsigned char l=0x0f;
	
	static int i=0;

	if (msec_count >= 30)
	{
		msec_count=0;
		if (i >= 4)
		{
			i=PORTA=0x00;
			if(mode == 0)
				state_transition();   // state를 천이 to shift_right_keep_ledon
		}
		else
		{
			PORTA = ( ((h >> i) & 0xf0) | ((l << i) & 0x0f));
			i++;
		}
	}
}

// 자동
void flower_off2(void)
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

void shift_left_keep_ledon1(void)
{
	static int i=0;

	if (msec_count >= 30)
	{
		msec_count=0;
		if (i >= 8)
		{
			i = PORTA = 0x00;
			if(mode == 0)
				state_transition();   // state를 천이 to shift_right_keep_ledon
		}
		else
		{
			PORTA |= 0b00000001 << i++;
		}
	}
}
void shift_left_keep_ledon2(void)
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
void shift_right_keep_ledon1(void)
{
	static int i=0;

	if (msec_count >= 30)
	{
		msec_count=0;
		if (i >= 8)
		{
			i=PORTA=0x00;
			if(mode == 0)
				state_transition();   // state를 천이 to shift_right_keep_ledon
		}
		else
		{
			PORTA |= 0b10000000 >> i++;
		}
	}
}
void shift_right_keep_ledon2(void)
{
   static int i=0;
	   
	PORTA=0;
	for (int i=0; i < 8; i++)
	{
		PORTA |= 0b10000000 >> i;
		//        10000000 i=0;
		//        01000000 i=1;
		_delay_ms(30);
	}
}

void shift_left_ledon1(void)
{
   // (1)for문 제거  (2) _delay_ms(30) 제거
   static int i=0;
   
   if (msec_count >= 30)
   {
	   msec_count=0;
	   if (i >= 8)
	   {
		   i=PORTA=0;
		   if(mode == 0)
		   {
			   state_transition();   // state를 천이 to shift_right_keep_ledon
		   }
	   }
	   else
	   {
		   PORTA = 0b00000001 << i++;  // (1) PORTA = 0b00000001 << i  (2) i++
	   }
   }
}
void shift_left_ledon2(void)
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

void shift_right_ledon1(void)
{
	static int i=0;

	if (msec_count >= 30)
	{
		msec_count=0;
		if (i >= 8)
		{
			i=PORTA=0x00;
			if(mode == 0)
				state_transition();   // state를 천이 to shift_right_keep_ledon
		}
		else
		{
			PORTA = 0b10000000 >> i++;
		}
	}
}
void shift_right_ledon2(void)
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


void state_transition(void)
{
	state++;
	state %= FUNC_NUMER;
}


