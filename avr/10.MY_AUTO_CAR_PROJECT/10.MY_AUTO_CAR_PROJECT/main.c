#include "button.h"
#include "uart0.h"
#include "extern.h"
#include "def.h"
#include "ultrasonic.h"

#define OK 1
#define ERROR 0
#define DISTANCE 13

// ====================
// 함수 선언
// ====================
void init_timer0(void);
void distance_check(void);
void manual_mode(void);
void auto_mode(void);

volatile int msec_count = 0;
volatile int ultrasonic_count = 0;

// 위험 거리 판단
int warning_left = 1;
int warning_right = 1;
int warning_middle = 1;

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

int func_index = MANUAL_MODE; // 초기 수동 모드
void (*pfunc[])() = {
	manual_mode,		// 수동모드
	distance_check,		// 초음파 거리 측정
	auto_mode_check,	// 버튼 체크
	auto_mode			// 자율주행
};

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;  // 1ms 주기
	msec_count++;
	ultrasonic_count++;
	fnd_display();
}

int main(void)
{
	init_timer0();
	init_timer1();
	init_uart0();
	init_uart1();
	init_button();
	init_L298N();
	init_led();
	init_ultrasonic();
	init_fnd();
	init_speaker();
	I2C_LCD_init();
	stdout = &OUTPUT;  // printf 사용 설정

	sei();  // 전역 인터럽트 허용

	while (1)
	{
		process_siren(); // 자동모드용 사운드
		// process_reverse_beep(mode, direction); // 수동후진용 사운드
		auto_mode_check();
		
		if (button_state)
			auto_mode();
		else 
		{
			stop();
			manual_mode();
		}
	}
}

void init_timer0(void)
{
	TCNT0 = 6;
	TCCR0 |= (1 << CS02);  // 분주 64
	TIMSK |= (1 << TOIE0); // 오버플로 인터럽트 허용
}

void manual_mode(void)
{
	switch (bt_data)
	{
		case 'F': case 'f':
			forward(1000);
			display_status(0, 0); // 모드 0이면 수동 모드
		break;
		
		case 'B': case 'b':
			backward(1000);
			display_status(0, 1);
		break;
		
		case 'L': case 'l':
			turn_left(1000);
			display_status(0, 2);
		break;
		
		case 'R': case 'r':
			turn_right(1000);
			display_status(0, 3);
		break;
		
		case 'S': case 's':
			stop();
			display_status(0, 4);
		break;
		
		default:
			display_status(0, 5); // 초기 모드
		break;
	}

	func_index = DISTANCE_CHECK;
}

void distance_check(void)
{
	func_index = MODE_CHECK;
}

void auto_mode(void)
{
	if (ultrasonic_count >= 90) // 거리 측정 시작
	{
		trigger_ultrasonic();
		ultrasonic_count = 0;
	}
    
	// 장애물이 가까운 경우
	if (middle_distance <= DISTANCE)
	{
		stop(); // 정지
		_delay_us(100);
		
		backward(700); // 약간 후진
		
		if (left_distance >= right_distance)
		{ // 더 거리가 넓은쪽으로 회전
			turn_left(1000);
			display_status(1, 2);
		}
		else
		{
			turn_right(1000);
			display_status(1, 3);
		}
		return;
	}
	
	
	// 정면 거리가 충분히 넓은 경우
	if (middle_distance > 220)
	{
		int side_diff = abs(left_distance - right_distance);
		
		// 좌우 거리가 충분히 넓다면 직진
		if (left_distance > 220 && right_distance > 220)
		{
			forward(450);
			display_status(1, 0);
		} // 좌우 거리가 차이가 없어도 직진
		else if (side_diff < 60)
		{
			forward(400);
			display_status(1, 0);
		}// 한쪽이 넓다면 넓은 방향으로 회전
		else
		{
			if (left_distance > right_distance)
			{
				turn_left(1000);
				display_status(1, 2);
			}
			else
			{
				turn_right(1000);
				display_status(1, 3);
			}
		}
		return;
	}
    // 정면 거리가 좁은 경우
	if (middle_distance <= 250)
	{
		// 양쪽 다 거리가 좁은 경우 후진
		if (left_distance <= 220 && right_distance <= 220)
		{
			backward(700);
			_delay_us(100);
			
			// 후진 후 좌우 거리 비교 -> 더 넓은 방향으로 회전
			if (left_distance >= right_distance)
			{
				turn_left(1000);
				display_status(1, 2);
			}
			else
			{
				turn_right(1000);
				display_status(1, 3);
			}
			return;
		}
		// 좌우 한쪽만 넓은 경우, 한쪽은 열려있는 경우
		if (left_distance > right_distance)
		{
			turn_left(1000); // 왼쪽으로 회전
			display_status(1, 2);
		}
		else
		{
			turn_right(1000); // 오른쪽으로 회전
			display_status(1, 3);
		}
		return;
	}
}