#include "fnd.h"
#include "button.h"
#include "led.h"
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h> // sei()

// ms_count1은 초를 처리하고, ms_count3은 스톱워치의 초를 처리
volatile int ms_count1 = 0;  
volatile int ms_count3 = 0;
int sec_count1 = 0;  
int sec_count2 = 0;
int sec_count3 = 0;

// led 제어(초)
uint32_t led0_cnt = 0; // 500ms
uint32_t led1_cnt = 0; // 1000ms
uint32_t led2_cnt = 0; // 100ms

#define MIN_SEC 0
#define CIRCLE 1
#define STOPWATCH 2  // STOPWATCH 모드 번호 변경

void min_sec_mode(void);
void circle_mode(void);
void stopwatch_mode(void);
void check_led_timers(void);
void led_toggle(int led_num);
void init_timer0(void);

int state = 0; // 모드 변경 인덱스
int stop_flag = 0; // 스톱 워치 실행 정지 flag

// 함수 포인터 배열
void (*fp[])(void) =
{	min_sec_mode,
	circle_mode, 
	stopwatch_mode
 };

int fnd_main(void)
{
	init_fnd();
	init_timer0();  // 타이머 인터럽트 초기화
	
	while (1)
	{
		if (get_button(BUTTON0, BUTTON0PIN))
		{
			state_transition();
			fp[state]();  // 현재 상태에 해당하는 함수 실행
		}
		
		led0_cnt++;
		led1_cnt++;
		led2_cnt++;
		
		button_reset(); // 버튼 초기화 함수
		check_led_timers(); // LED 타이머 체크
		
		fnd_display();  // FND 디스플레이 업데이트
		_delay_ms(1);  // 1ms 지연
	}
	
	return 0;
}


void min_sec_mode(void)
{
    sec_count2 = 0;
}

void circle_mode(void)
{
    sec_count2++;
}

void stopwatch_mode(void)
{
    sec_count3++;
}

void fnd_display(void)
{
    int operand_index2[] = {int operand_index2[] = {-1, -2, -2, -2, -2, -2, -10, -14, -16, -1, -3, -7, -15, -15, -15, -15, -15, -16 };
}; // 3번째 fnd
    int operand_index3[] = {-1, -1, -2, -33, -49, -57, -57, -57, -57, -1, -1, -1, -9, -24, -56, -57, -57}; // 4번째 fnd

};

    static int digit_select = 0;
	
	// FND를 모두 끄는 처리 (매번 자리 표시 전에 끄기)
    FND_DATA_PORT = ~0x00;  // 모든 FND 끄기

    // 각 자리에 대해 값을 설정
    switch(digit_select)
    {
        case 0:
            FND_DIGIT_PORT = 0x80;   // 첫 번째 자리를 켬
        
            if(state == CIRCLE)
                FND_DATA_PORT = fnd_font[sec_count2 % 10];   // CIRCLE 모드에서 초의 1의 자리
            else if(state == STOPWATCH)
                FND_DATA_PORT = fnd_font[sec_count3 % 10];   // STOPWATCH 모드에서 초의 1의 자리
            else
                FND_DATA_PORT = fnd_font[sec_count1 % 10];   // MIN_SEC 모드에서 초의 1의 자리
            break;

        case 1:
            FND_DIGIT_PORT = 0x40;   // 두 번째 자리를 켬
        
            if(state == CIRCLE)
                FND_DATA_PORT = fnd_font[sec_count2 / 10 % 6];   // CIRCLE 모드에서 초의 10의 자리
            else if(state == STOPWATCH)
                FND_DATA_PORT = fnd_font[sec_count3 / 10 % 6];   // STOPWATCH 모드에서 초의 10의 자리
            else
                FND_DATA_PORT = fnd_font[sec_count1 / 10 % 6];  // MIN_SEC 모드에서 초의 10의 자리
            break;

        case 2:
            FND_DIGIT_PORT = 0x20;   // 세 번째 자리를 켬
        
            if(state == CIRCLE)
                FND_DATA_PORT = operand_index2[sec_count2 % 18]; // CIRCLE 모드에서 써클 표시
            else if(state == STOPWATCH)
                FND_DATA_PORT = fnd_font[sec_count3 / 60 % 10];   // STOPWATCH 모드에서 초의 1의 자리
            else
			{
				if(sec_count1 % 2 == 0)
					FND_DATA_PORT = fnd_font[sec_count1 / 60 % 10] & 0x7f;   // MIN_SEC 모드에서 분의 1의 자리
				FND_DATA_PORT = fnd_font[sec_count1 / 60 % 10];  // MIN_SEC 모드에서 분의 1의 자리
			}
            break;

        case 3:
            FND_DIGIT_PORT = 0x10;   // 네 번째 자리를 켬
            
            if(state == CIRCLE)
                FND_DATA_PORT = operand_index3[sec_count2 % 18]; // CIRCLE 모드에서 써클 표시
            else if(state == STOPWATCH)
                FND_DATA_PORT = fnd_font[sec_count3 / 600 % 6];   // STOPWATCH 모드에서 분의 10의 자리
            else
                FND_DATA_PORT = fnd_font[sec_count1 / 600 % 6];   // MIN_SEC 모드에서 분의 10의 자리
            break;
    }

    // 자리를 순차적으로 변경
    digit_select++;
    digit_select %= 4;   // 다음 표시할 자리를 선택
}

void init_fnd(void)
{
	FND_DATA_DDR = 0xff;  // FND 데이터 핀 출력 모드로 설정

	// 자릿수 선택 핀을 출력 모드로 설정
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3
	| 1 << FND_DIGIT_D4;

	// FND를 모두 끄는 처리
	#if 1  // common 애노우드
	FND_DATA_PORT = ~0x00;  // 모든 FND 끄기
	#else  // common 캐소우드
	FND_DATA_PORT = 0x00;   // 모든 FND 끄기
	#endif
}

void state_transition(void)
{
    state++;
    state %= 3;  // 3가지 모드를 순환
}

void button_reset(void)
{
    if(get_button(BUTTON1, BUTTON1PIN))
    {
        // Reset depending on the mode
        if(state == MIN_SEC)
        {
            ms_count1 = 0;
            sec_count1 = 0;  // 통합된 sec_count 사용
        }
        else if(state == CIRCLE)
        {
            sec_count2 = 0;  // 동일하게 통합된 sec_count 사용
        }
        else if(state == STOPWATCH)
        {
            ms_count1 = 0;
            sec_count3 = 0;  // 스톱워치 모드에서 리셋
        }
    }
	else if(get_button(BUTTON2, BUTTON2PIN))
	{
		if(state == STOPWATCH)
			stop_flag = !stop_flag;
	}
}

void check_led_timers(void)
{
	// 각 모드에 맞는 주기에서 LED 깜빡이기
	if (state == STOPWATCH && led2_cnt >= 100)  // 100ms
	{
		led2_cnt = 0;  // 타이머 초기화
		led_toggle(2);  // LED2 토글
	}
	else if (state == CIRCLE && led1_cnt >= 1000)  // 1000ms
	{
		led1_cnt = 0;  // 타이머 초기화
		led_toggle(1);  // LED1 토글
	}
	else if (state == MIN_SEC && led0_cnt >= 500)  // 500ms
	{
		led0_cnt = 0;  // 타이머 초기화
		led_toggle(0);  // LED0 토글
	}
	else
	{
		led_all_off();  // 다른 모드일 경우 모든 LED 끄기
	}
}

// LED 토글 함수
void led_toggle(int led_num)
{
	static int led_state[3] = {0, 0, 0};  // LED 상태를 저장하는 배열 (0: OFF, 1: ON)

	if (led_state[led_num] == 0)
	{
		// 해당 LED ON
		if (led_num == 0) led0_on();
		else if (led_num == 1) led1_on();
		else if (led_num == 2) led2_on();

		led_state[led_num] = 1;  // 상태 업데이트 (ON)
	}
	else
	{
		led_all_off();

		led_state[led_num] = 0;  // 상태 업데이트 (OFF)
	}
}


void led2_toggle(void)
{
	static int led2_state = 0;
	if (led2_state == 0)
	{
		led2_on();  // LED2 ON
		led2_state = 1;
	}
	else
	{
		led_all_off();  // LED2 OFF
		led2_state = 0;
	}
}

ISR(TIMER0_OVF_vect)
{
	ms_count1++;  // 1ms마다 증가
	ms_count3++;  // 10ms마다 증가

	if (ms_count1 >= 1000)   // 1000ms --> 1초
	{
		ms_count1 = 0;
		sec_count1++;  // 1초 카운트
		if (state == CIRCLE)
		sec_count2++;  // CIRCLE 모드에서 초 카운트
	}

	if (ms_count3 >= 10)  // 10ms마다 카운트
	{
		ms_count3 = 0;
		if (state == STOPWATCH && stop_flag)
		sec_count3++;  // STOPWATCH 모드에서 초 카운트
	}
}

void init_timer0(void)
{
	// 16MHZ/ 64 분주(down) 분주 : divider/prescale
	// ------ 분주비 계산 ------
	// (1) 16000000HZ/64 ==> 250,000HZ
	// (2) T(주기) 1clock의 소요시간 : 1/f = 1/250,000 ==> 0.0000004sec(4us) : 0.004ms
	// (3) 8bit timer 0V(OVflow) : 0.004ms x 256 = 0.001024sec --> 1.024ms
	// 1ms마다 정확하게 INT를 띄우고 싶으면  0.004ms x 250개를 count = 0.001sec ==> 1ms 마다 오버플로우
	TCNT0 = 6;  // TCNT : 0~256 1마다 TIMER0_OVF_vect로 진입 한다.
	// TCNT0 = 6으로 설정을 한 이유 : 6-->256 : 250개의 펄스를 count하기 때문에 정확히 1ms가 된다.
	
	// (4) 분주비 설정 64분주 (250,000HZ -->250KHZ) P296 표13-1
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;  // TCCR0 |= 0xf4 보다 좌측의 code 권장
	// (5) Timer0 overflow INT를 허용(endable)
	TIMSK |= 1 << TOIE0;  // TIMSK |= 0x01;
	sei(); // 전역(대문)으로 interruput 허용
}