/*
 * pwm.c
 *
 * Created: 2025-03-13 오후 12:49:07
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

/*
	16비트 timer 1번 활용
	PWM 출력 신호(2EA)
	=============
	PE5: 0C1A 왼쪽 바퀴.
	PE6: OC1B 오른쪽 바퀴.
	BTN0: auto/manual
	
	방향설정
	=======
	1. LEFT MOTOR
	   PF0 : IN1(motor driver)
	   PF1 : IN2(
	2. RIGHT MOTOR
	   PF2 : IN3(motor driver)
	   PF3 : IN4
	   
	   IN1/IN3    IN2/IN4
	   =======    =======
	      0          1   : 역회전
		  1          0   : 정회전
		  1          1   : STOP
*/
void init_timer1(void)
{
	// 분주비 : 64.  16000000 / 64 ---> 2500000Hz
	// T = 1/f = 1/2500000Hz ==> 0.000004sec(4us)
	// 2500000Hz에서 256개의 펄스를 count하면 소요시간 1.02ms
	//               127개                           0.5ms
	// 표318 표 14-1
	TCCR1B |= 1 << CS11 | 1 << CS10; // 분주비 : 64
	
	// 16 timer1 모드 14번의 고속 PWM(P327P 표 14-5)
	TCCR1A |= 1 << WGM11; // TOP을 ICR1에 지정을 할 수 있다.
	TCCR1B |= 1 << WGM13 | 1 << WGM12; //
	
	// 비반전 모드 TOP : ICR1비교일치값(PWM)지정 : OCR1A, OCR1B 표 15-7
	// 사용자가 설정한 PWM값에 일치시 OC3C의 출력핀이 LOW로 바뀌고 BOTTOM에서 HIGH로 바뀐다.
	// P350 표15-7
	TCCR1A |= 1 << COM1A1 | 1 << COM1B1;
	
	ICR1 = 0x3ff; // 1023 =?
}

/*
방향설정
=======
1. LEFT MOTOR
PF0 : IN1(motor driver)
PF1 : IN2(
2. RIGHT MOTOR
PF2 : IN3(motor driver)
PF3 : IN4

IN1/IN3    IN2/IN4
=======    =======
0          1   : 역회전
1          0   : 정회전
1          1   : STOP
*/
void init_L298N(void)
{
	MOTOR_PWM_DDR |= 1 << MOTOR_LEFT_PORT_DDR | 1 << MOTOR_RIGHT_PROT_DDR; // 출력 모드로 설정
	MOTOR_DRIVER_DIRECTION_DDR |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;  // IN1 IN2 IN3 IN4
	
	// 값이 뭐가 들어있는지 모르니까 전진 모드로 설정
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 2 | 1 << 0; // 자동차를 전진 모드로 설정
}

void stop(void)
{
	// 값이 뭐가 들어있는지 모르니까 전진 모드로 설정. 스탑 모드로 설정
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3; // 자동차를 전진 모드로 설정
}

void forward(int speed)
{
	// 값이 뭐가 들어있는지 모르니까 전진 모드로 설정. 스탑 모드로 설정
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 0 | 1 << 2;
	
	OCR1A = OCR1B = speed;  // PB5(OCR1A): 왼쪽, PB6(OCR1B) : 오른쪽
}

void backward(int speed)
{
	// 값이 뭐가 들어있는지 모르니까 전진 모드로 설정. 스탑 모드로 설정
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 3 | 1 << 1; // 후진 모드 1010 IN1, IN30으로 해서 후진시킨다.
	
	OCR1A = OCR1B = speed;  // PB5(OCR1A): 왼쪽, PB6(OCR1B) : 오른쪽
}

void turn_left(int speed)
{
	// 값이 뭐가 들어있는지 모르니까 전진 모드로 설정. 스탑 모드로 설정
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 0 | 1 << 2; // 전진 모드
	
	OCR1A = 0;    // PB5(OCR1A): 왼쪽
	OCR1B = speed; // PB6(OCR1B) : 오른쪽
}

void turn_right(int speed)
{
	// 값이 뭐가 들어있는지 모르니까 전진 모드로 설정. 스탑 모드로 설정
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 0 | 1 << 2; // 전진 모드
	
	OCR1A = speed;    // PB5(OCR1A): 왼쪽
	OCR1B = 0; // PB6(OCR1B) : 오른쪽
}