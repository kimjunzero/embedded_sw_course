#include "fnd.h"
#include "extern.h"
extern int sound_state;

void fnd_display(void)
{
	static int digit_select = 0;
	                     // 0     1     2     3    A     U     H     n     d     t     _
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x88, 0xc1, 0x89, 0xab, 0xa1, 0x4e, 0xf7};

	enum { // 0       1      2       3        4      5       6       7       8       9        10        
		   FONT_0, FONT_1, FONT_2, FONT_3, FONT_A, FONT_U, FONT_H, FONT_n, FONT_d, FONT_t, FONT_UNDER
	};

	uint8_t auto_text[4];
	uint8_t hand_text[4] = {FONT_d, FONT_n, FONT_A, FONT_H};  // HAnd 표시(수동 모드)

	if(button_state) // 자동 모드이면 ot_1 , ot_2, ot_3 표시
	{
		auto_text[0] = (sound_state == 1) ? FONT_1 : (sound_state == 2) ? FONT_2 : FONT_3; // 삼항 연산자로 숫자 다르게 표시
		auto_text[1] = FONT_UNDER;
		auto_text[2] = FONT_U;
		auto_text[3] = FONT_0;
	}

	FND_DIGIT_PORT = 0x00; // 모든 자릿수 끄고 시작(잔상방지)

	if(button_state) // 자동 모드
		FND_DATA_PORT = fnd_font[auto_text[digit_select]];
	else             // 수동 모드
		FND_DATA_PORT = fnd_font[hand_text[digit_select]];

	switch(digit_select)
	{
		case 0: FND_DIGIT_PORT = 0x08; break; // 첫째 자리
		case 1: FND_DIGIT_PORT = 0x04; break; // 둘째 자리
		case 2: FND_DIGIT_PORT = 0x02; break; // 셋째 자리
		case 3: FND_DIGIT_PORT = 0x01; break; // 넷째 자리
	}

	digit_select = (digit_select + 1) % 4;
}
void init_fnd(void)
{
	FND_DATA_DDR = 0xff;  // 출력 모드로 설정
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3| 1 << FND_DIGIT_D4; // PB0, PB1, PB2, PB3
	
	FND_DATA_PORT = ~0x00;   // fnd를 all off = 0xff;
}