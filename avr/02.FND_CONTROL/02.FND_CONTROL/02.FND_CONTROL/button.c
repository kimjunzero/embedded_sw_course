﻿/*
 * button.c
 *
 * Created: 2025-03-05 오후 2:30:26
 *  Author: microsoft
 */ 

#include "button.h"

// 버튼 초기화(방향설정 : 입력)
// PORT 76543210
//       1010  초기값
//      10000111 & 
// DDRD &= 0x87     // 3, 4, 5, 6을 input으로 설정을 한다. (1) 가독성이 떨어진다. (code 해석이 어렵다)
//                                                        (2) port를 변경시 프로그램 수정이 어렵다.

void init_button(void)
{
	BUTTON_DDR &= ~(1 << BUTTON0PIN | 1 << BUTTON1PIN | 1 << BUTTON2PIN | 1 << BUTTON3PIN);
}

// 예) 
// BUTTON : S/W 번호 --> button_num
// BUTTON0PIN : button_pin
// 리턴값 : 1 : 버튼을 눌렀다 떼면 1을 return
//          0 : ide

int get_button(int button_num, int button_pin)
{
	static unsigned char button_status[] =
	{BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE};
	// 지역 변수에 static을 쓰면 전역 변수처럼 함수를 빠져 나갔다 다시 들어 와도 값을 유지 한다.
	int currtn_state;
	
	currtn_state = BUTTON_PIN & (1 << button_pin); // 버튼을 읽는다.
	if(currtn_state && button_status[button_num] == BUTTON_RELEASE) // 버튼이 처음 눌려진 noise 상태. high
	{
		_delay_ms(60); // noise가 지나가기를 기다린다.
		button_status[button_num] = BUTTON_PRESS; // noise가 지나간 상태의 High 상태
		return 0; // 아직은 완전히 눌렸다 떼어진 상태가 아니다.
	}
	else if(currtn_state == BUTTON_RELEASE && button_status[button_num] == BUTTON_PRESS)
	{
		_delay_ms(60);
		button_status[button_num] = BUTTON_RELEASE; // 다음 버튼 체크를 위해 초기화
		return 1; // 완전히 1번 눌렸다 떼어진 상태로 인정
	}
	return 0;
}
