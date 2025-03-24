/*
 * pwm.c
 *
 * Created: 2025-03-13 오후 12:49:01
 *  Author: microsoft
 */ 

#include "pwm.h"
#include "extern.h"

void init_timer1(void);
void init_L298N(void);
void stop(void);
void forward(int speed);
void backward(int speed);
void turn_left(int speed);
void turn_right(int speed);
extern volatile int msec_count;

/*
	16bit timer 1번 활용
	PWM 출력 신호(2EA)
	=================
	PE5  : OC1A 왼쪽 바퀴
	PE6  : OC1B 오른쪽 바퀴
	BTN0 : AUTO/MANUAL
	
	방향설정
	========
	1. LEFT MOTOR
		PF0 : IN1 (motor driver)
		PF1 : IN2
	2. RIGHT MOTOR
		PF2 : IN3 (motor driver)
		PF3 : IN4
		
		IN1/IN3		IN2/IN4
		=======     =======
		   0           1   : 역회전
		   1           0   : 정회전
		   1           1   :  STOP    
*/
void init_timer1(void) //Timer1
{
	// 분주비 : 64 16000000 / 64 ---> 250000Hz(250Khz)
	// T=1/f = 1/250000Hz ==> 0.000004sec (4us)
	// 250000Hz 에서 256개의 펄스를 count하면 소요시간 1.02ms
	//				 127개							 0.5ms
	// P318 표 14-1
	TCCR1B |= 1 << CS11 | 1 << CS10;	// 분주비 : 64
	
	// timer1 모드14 고속 PWM (P327 표 14-5)
	TCCR1A |= 1 << WGM11;	// TOP을 ICR1에 지정을 할 수 있다.
	TCCR1B |= 1 << WGM13 | 1 << WGM12;
	
	// 비반전 모드 TOP : ICR1 비교일치값(PWM) 지정 : OCR1A, OCR1B
	// 비반전모드 : 비교일치 발생시 OCn의 출력핀이 LOW로 바뀌고 BOTTOM 에서 HIGH 로 바뀐다.
	// P350 표 15-7
	TCCR1A |= 1 << COM1A1 | 1 << COM1B1;
	
	ICR1 = 0x3ff; // 1023 ==> 4ms TOP : PWM 값
}

/*
	방향설정
	========
	1. LEFT MOTOR
	PF0 : IN1 (motor driver)
	PF1 : IN2
	2. RIGHT MOTOR
	PF2 : IN3 (motor driver)
	PF3 : IN4
	
	IN1/IN3		IN2/IN4
	=======     =======
	0           1   : 역회전
	1           0   : 정회전
	1           1   :  STOP
*/
void init_L298N(void)
{
	MOTOR_PWM_DDR |= 1 << MOTOR_LEFT_PORT_DDR | 1 << MOTOR_RIGHT_PORT_DDR; // 출력모드
	MOTOR_DRIVER_DIRECTION_DDR |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;	   // IN1 IN2 IN3 IN4 출력
	
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 0 | 1 << 2;	// 자동차를 전진 모드로 설정
}

void stop(void)
{	
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;	// 자동차를 정지 모드로 설정
}

void forward(int speed)
{
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 0 | 1 << 2;	// 자동차를 전진 모드로 설정
	
	OCR1A = OCR1B = speed;		// PB5 : 왼쪽, PB6 : 오른쪽
}

void backward(int speed)
{	
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 1 | 1 << 3;	// 자동차를 후진 모드로 설정, 1010
	
	OCR1A = OCR1B = speed;		// PB5 : 왼쪽, PB6 : 오른쪽
}

void turn_left(int speed)
{	
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 0 | 1 << 2;	// 자동차를 전진 모드로 설정
	
	OCR1A = 0;			// PB5(OCR1A) : 왼쪽
	OCR1B = speed;		// PB6(OCR1B) : 오른쪽
}

void turn_right(int speed)
{	
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 0 | 1 << 2;	// 자동차를 전진 모드로 설정
	
	OCR1A = speed;		// PB5(OCR1A) : 왼쪽
	OCR1B = 0;			// PB6(OCR1B) : 오른쪽
}
