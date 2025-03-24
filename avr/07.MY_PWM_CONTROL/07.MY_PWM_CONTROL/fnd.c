/*
 * fnd.c
 *
 * Created: 2025-03-06 오후 12:23:41
 *  Author: microsoft
 */ 
#include "fnd.h"
void init_fnd(void);
int fnd_main(void);
void fnd_display1(void);

extern volatile int msec_count;
extern uint32_t sec_count;  // 초를 재는 count변수 unsigned int --> uint32_t

void fnd_display1(void)
{
	// 0   1    2    3     4    5   6     7   8    9    .
	uint8_t fnd_font[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xd8,0x80,0x90,0x7f};
		
	static int digit_select=0;  // static를 쓰면 전역 변수 처럼 함수가 빠져 나갔다가 다시 들어 오더라도 값을 유지
	
	switch(digit_select)
	{
		case 0:
			FND_DIGIT_PORT = 0x80;   //PORTB 7
			FND_DATA_PORT = fnd_font[sec_count % 10];   // 0~9초
			break;
		case 1:
			FND_DIGIT_PORT = 0x40;  //PORTB 6
			FND_DATA_PORT = fnd_font[sec_count / 10 % 6];   // 10단위초
			break;
		case 2:
			FND_DIGIT_PORT = 0x20; //PORTB 5
			FND_DATA_PORT = fnd_font[sec_count / 60 % 10];   // 1단위 분
			break;
		case 3:
			FND_DIGIT_PORT = 0x10;  //PORTB4
			FND_DATA_PORT = fnd_font[sec_count / 600 % 6];   // 10단위 분
			break;
	}
	digit_select++;
	digit_select %= 4;   // 다음 표시할 자리수 선택
}

void fnd_display2(void)
{
	// D4
	uint8_t fnd_font_D4[] = {0xFD,0xF9,0xF1,0xF1,0xF1,0xF1,0xF1,0xF1,0xF1,0xF1,0xF1,0xF0,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xF6,0xF2,0xF0};
	// D3
	uint8_t fnd_font_D3[] = {0xFF,0xFF,0xFF,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF6,0xF6,0xFF,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xF6,0xF6,0xF6,0xF6};
	// D2
	uint8_t fnd_font_D2[] = {0xFF,0xFF,0xFF,0xFF,0xF7,0xF7,0xF7,0xF7,0xF7,0xF6,0xF6,0xF6,0xFF,0xFF,0xFE,0xFE,0xFE,0xFE,0xFE,0xF6,0xF6,0xF6,0xF6,0xF6};
	// D1
	uint8_t fnd_font_D1[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xF7,0xE7,0xC7,0xC6,0xC6,0xC6,0xC6,0xFF,0xFF,0xFF,0xFE,0xDE,0xCE,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6};

	static int digit_select1=0;  // static를 쓰면 전역 변수 처럼 함수가 빠져 나갔다가 다시 들어 오더라도 값을 유지
	
	switch(digit_select1)
	{
		case 0:
			FND_DIGIT_PORT = 0x08;   //PORTB 4
			FND_DATA_PORT = fnd_font_D4[sec_count % 24];   // 0~9초
		break;
		case 1:
			FND_DIGIT_PORT = 0x04;  //PORTB 3
			FND_DATA_PORT = fnd_font_D3[sec_count % 24];   // 10단위초
		break;
		case 2:
			FND_DIGIT_PORT = 0x02; //PORTB 2
			FND_DATA_PORT = fnd_font_D2[sec_count % 24];   // 1단위 분
		break;
		case 3:
			FND_DIGIT_PORT = 0x01;  //PORTB 1
			FND_DATA_PORT = fnd_font_D1[sec_count % 24];   // 10단위 분
		break;
	}
	digit_select1++;
	digit_select1 %= 4;   // 다음 표시할 자리수 선택
}

void init_fnd(void)
{
	FND_DATA_DDR = 0xff;  // 출력 모드로 설정
	// FND_DIGIT_DDR |= 0xf0;   // 자릿수 선택 7654 
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 
					 | 1 << FND_DIGIT_D4 | 1 << FND_DIGIT_D5 | 1 << FND_DIGIT_D6 | 1 << FND_DIGIT_D7 | 1 << FND_DIGIT_D8; 
	// fnd를 all off 
#if 0  // common 애노우드 
	FND_DATA_PORT = ~0x00;   // fnd를 all off  0xff;
#else  // common 캐소우드
	FND_DATA_PORT = 0x00;   // fnd를 all off   
#endif 
}