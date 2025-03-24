/*
 * fnd.c
 *
 * Created: 2025-03-06 오후 12:23:41
 *  Author: microsoft
 */ 
#include "fnd.h"
void init_fnd(void);
int fnd_main(void);
void fnd_display(void);

uint32_t ms_count=0;  // ms를 재는 count변수 unsigned int --> uint32_t
uint32_t sec_count=0;  // 초를 재는 count변수 unsigned int --> uint32_t

int fnd_main(void)
{
	init_fnd();
	
	while (1)
	{
		fnd_display();
		_delay_ms(1);
		ms_count++;
		if (ms_count >= 1000)   // 1000ms --> 1sec
		{
			ms_count=0;
			sec_count++;
		}
	}
	
	return 0;
}

void fnd_display(void)
{
#if 0  // common 애노우드
                         // 0   1    2    3     4    5   6     7   8    9    .
	uint8_t fnd_font[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xd8,0x80,0x90,0x7f};
#else  // common 캐소우드 
						 // 0   1    2    3     4    5   6     7   8    9    .
    uint8_t fnd_font[] = {~0xc0,~0xf9,~0xa4,~0xb0,~0x99,~0x92,~0x82,~0xd8,~0x80,~0x90,~0x7f};
#endif 

	static int digit_select=0;  // static를 쓰면 전역 변수 처럼 함수가 빠져 나갔다가 다시 들어 오더라도 값을 유지
	
	switch(digit_select)
	{
		case 0:
#if 0  // common 애노우드 
			FND_DIGIT_PORT = 0x80;   // 10000000 FND_DIGIT_PORT = 0x80
#else   // common 캐소우드 
			FND_DIGIT_PORT = ~0x80;   // 011111111 FND_DIGIT_PORT = ~0x80
#endif 
			FND_DATA_PORT = fnd_font[sec_count % 10];   // 0~9초 
			break;
		case 1:
#if 0  // common 애노우드
			FND_DIGIT_PORT = 0x40;   // 10000000  
#else   // common 캐소우드
			FND_DIGIT_PORT = ~0x40;  
#endif 
			FND_DATA_PORT = fnd_font[sec_count / 10 % 6];   // 10단위초
			break;
		case 2:
#if 0  // common 애노우드
			FND_DIGIT_PORT =  0x20;      
#else   // common 캐소우
			FND_DIGIT_PORT =  ~0x20; // 011111111
#endif
			FND_DATA_PORT = fnd_font[sec_count / 60 % 10];   // 1단위 분 
			break;
		case 3:
#if 0  // common 애노우드
			FND_DIGIT_PORT = 0x10;   // 10000000
#else   // common 캐소우드
			FND_DIGIT_PORT = ~0x10;   // 011111111
#endif
			FND_DATA_PORT = fnd_font[sec_count / 600 % 6];   // 10단위 분 
			break;
	}
	digit_select++;
	digit_select %= 4;   // 다음 표시할 자리수 선택
}

void init_fnd(void)
{
	FND_DATA_DDR = 0xff;  // 출력 모드로 설정
	// FND_DIGIT_DDR |= 0xf0;   // 자릿수 선택 7654 
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 
					 | 1 << FND_DIGIT_D4; 
	// fnd를 all off 
#if 0  // common 애노우드 
	FND_DATA_PORT = ~0x00;   // fnd를 all off  0xff;
#else  // common 캐소우드
	FND_DATA_PORT = 0x00;   // fnd를 all off   
#endif 
}