/*
 * fnd.c
 *
 * Created: 2025-03-06 오후 12:23:44
 *  Author: microsoft
 */ 
#include "fnd.h"
#include "button.h"

static uint32_t ms1_count = 0;  // ms를 재는 count변수 unsigned int --> uint32_t
static uint32_t sec1_count = 0; // 초를 재는 count변수 unsigned int --> uint32_t
static uint32_t ms2_count = 0;  // ms를 재는 count변수 unsigned int --> uint32_t
static uint32_t sec2_count = 0; // 초를 재는 count변수 unsigned int --> uint32_t

int fnd_mode1(void)
{
	while(1)
	{
		fnd_display1();
		_delay_ms(1);
		ms1_count++;
		if(ms1_count >= 1000)  // 1000ms --> 1sec
		{
			ms1_count = 0;
			sec1_count++;
		}
		
		// 1분 33초가 되었을 때 리셋
		if (sec1_count == 93) {
			sec1_count = 0;  // 리셋
			ms1_count = 0;   // 밀리초 카운터도 리셋
		}
		
		// 버튼이 눌리면 모드 변경을 위해 탈출
		if(get_button(BUTTON0, BUTTON0PIN)) return 0;
	}
	
	return 0;
}

int fnd_mode2(void)
{
	while(1)
	{
		fnd_display2();
		_delay_ms(1);
		ms2_count++;
		if(ms2_count >= 1000)  // 1000ms --> 1sec
		{
			ms2_count = 0;
			sec2_count++;
		}
		
		// 버튼이 눌리면 모드 변경을 위해 탈출
		if(get_button(BUTTON0, BUTTON0PIN)) return 0;
		
	}
	
	return 0;
}

void fnd_display2(void)
{
	#if 1  // common 애노드
						 // 0     1     2     3     4    5     6     7     8     9     .
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90, 0x7f};
	#else  // common 캐소드
						//  0      1      2      3      4     5      6      7      8      9      .
	uint8_t fnd_font[] = {~0xc0, ~0xf9, ~0xa4, ~0xb0, ~0x99, ~0x92, ~0x82, ~0xd8, ~0x80, ~0x90, ~0x7f};
	#endif

	static int digit_select2 = 0; // static를 쓰면 전역 변수처럼 함수가 빠져 나갔다가 다시 들어오더라도 값을 유지
	// 
	switch(digit_select2)
	{
		case 0:
		#if 1 // common 애노드
			FND_DIGIT_PORT = 0x80; // 10000000
		#else // common 캐소드
			FND_DIGIT_PORT = ~0x80 // 01111111
		#endif
			FND_DATA_PORT = fnd_font[sec2_count % 10]; // 0~9초
		break;
		
		case 1:
		#if 1 // common 애노드
			FND_DIGIT_PORT = 0x40; // 10000000
		#else // common 캐소드
			FND_DIGIT_PORT = ~0x40; // 01111111
		#endif
			FND_DATA_PORT = fnd_font[sec2_count / 10 % 6]; // 10단위초
		break;
		
		case 2:
		#if 1 // common 애노드
			FND_DIGIT_PORT = 0x20; // 10000000
		#else // common 캐소드
			FND_DIGIT_PORT = ~0x20; // 01111111
		#endif
			FND_DATA_PORT = 0;
		break;
		
		case 3:
		#if 1 // common 애노드
			FND_DIGIT_PORT = 0x10; // 10000000
		#else // common 캐소드
			FND_DIGIT_PORT = ~0x10; // 01111111
		#endif
			FND_DATA_PORT = 0;
		break;
		
	}
	digit_select2++;
	digit_select2 %= 4; // 다음 표시할 자리수 선택
}
void fnd_display1(void)
{
#if 1  // common 애노드
                         // 0     1     2     3     4    5     6     7     8     9     .
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90, 0x7f};
#else  // common 캐소드
	                     //  0      1      2      3      4     5      6      7      8      9      .
	uint8_t fnd_font[] = {~0xc0, ~0xf9, ~0xa4, ~0xb0, ~0x99, ~0x92, ~0x82, ~0xd8, ~0x80, ~0x90, ~0x7f};
#endif 

	static int digit_select1 = 0; // static를 쓰면 전역 변수처럼 함수가 빠져 나갔다가 다시 들어오더라도 값을 유지
	
	switch(digit_select1)
	{
		case 0:
#if 1 // common 애노드
			FND_DIGIT_PORT = 0x80; // 10000000
#else // common 캐소드
			FND_DIGIT_PORT = ~0x80; // 01111111
#endif
			FND_DATA_PORT = fnd_font[sec1_count % 10]; // 0~9초
			break;
		case 1:
#if 1 // common 애노드
		FND_DIGIT_PORT = 0x40; // 10000000
#else // common 캐소드
		FND_DIGIT_PORT = ~0x40; // 01111111
#endif
			FND_DATA_PORT = fnd_font[sec1_count / 10 % 6]; // 10단위초 
			break;
		case 2:
#if 1 // common 애노드
		FND_DIGIT_PORT = 0x20; // 10000000
#else // common 캐소드
		FND_DIGIT_PORT = ~0x20; // 01111111
#endif
			if(sec1_count % 2 == 0)
				FND_DATA_PORT = fnd_font[sec1_count / 60 % 10] & 0x7f;// 1단위 분
			else
				FND_DATA_PORT = fnd_font[sec1_count / 60 % 10];
			break;
		case 3:
#if 1 // common 애노드
		FND_DIGIT_PORT = 0x10; // 10000000
#else // common 캐소드
		FND_DIGIT_PORT = ~0x10; // 01111111
#endif
			FND_DATA_PORT = fnd_font[sec1_count / 600 % 6]; // 10단위 분
			break;
	}
	digit_select1++;
	digit_select1 %= 4; // 다음 표시할 자리수 선택
}

void init_fnd(void)
{
	FND_DATA_DDR = 0xff; // 출력 모드로 설정
	// FND_DIGIT_DDR |= 0xf0; // 자리수 선택 7654
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 | 1 << FND_DIGIT_D4;
	
	// fnd를 all off
#if 1 // common 애노드
	FND_DATA_PORT = ~0x00; // fnd를 all off 0xff;
#else // common 캐소드
	FND_DATA_PORT = 0x00;  // fnd all off
#endif
}