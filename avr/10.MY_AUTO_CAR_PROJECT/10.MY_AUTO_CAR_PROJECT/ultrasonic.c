#include "ultrasonic.h"

volatile char scm[50];

#define TIMER2_PRESCALER     128
#define TIME_PER_TICK_US     (1000000.0 * TIMER2_PRESCALER / F_CPU)


// 거리 계산 함수
static inline float calculate_distance(uint8_t tcnt_value) {
	return tcnt_value * TIME_PER_TICK_US;
}


// 거리 경고 상태 갱신
static inline void update_warning(float distance, float range, volatile uint8_t* warning_flag) {
	*warning_flag = (distance < range) ? 1 : 0;
}

// ISR: 초음파 센서 3개
ISR(INT4_vect) { // LEFT
	if (ECHO_PIN_LEFT & (1 << ECHO_LEFT)) 
	{
		TCNT2 = 0;
	} 
	else 
	{
		left_distance = calculate_distance(TCNT2);
		update_warning(left_distance, 220, &warning_left);
	}
}

ISR(INT5_vect) { // CENTER
	if (ECHO_PIN_CENTER & (1 << ECHO_CENTER)) 
	{
		TCNT2 = 0;
	} 
	else 
	{
		middle_distance = calculate_distance(TCNT2);
		update_warning(middle_distance, 250, &warning_middle);
	}
}

ISR(INT6_vect) { // RIGHT
	if (ECHO_PIN_RIGHT & (1 << ECHO_RIGHT)) 
	{
		TCNT2 = 0;
	} 
	else 
	{
		right_distance = calculate_distance(TCNT2);
		update_warning(right_distance, 220, &warning_right);
	}
}

// =========================
// 트리거 펄스 출력
// =========================
static inline void trigger_sensor(volatile uint8_t* port, uint8_t pin) 
{
	*port &= ~(1 << pin);
	_delay_us(10);
	*port |= (1 << pin);
	_delay_us(15);
	*port &= ~(1 << pin);
}

void trigger_ultrasonic(void) 
{
	trigger_sensor(&TRIG_PORT_LEFT, TRIG_LEFT);
	trigger_sensor(&TRIG_PORT_CENTER, TRIG_CENTER);
	trigger_sensor(&TRIG_PORT_RIGHT, TRIG_RIGHT);
}

// 초기화 함수
void init_ultrasonic(void) {
	// Timer2: 128분주 설정
	TCCR2 |= (1 << CS22) | (1 << CS20);  // 분주 128

	// LEFT
	TRIG_DDR_LEFT |= (1 << TRIG_LEFT);
	ECHO_DDR_LEFT &= ~(1 << ECHO_LEFT);
	EICRB |= (1 << ISC40);    // Any logical change
	EIMSK |= (1 << INT4);

	// CENTER
	TRIG_DDR_CENTER |= (1 << TRIG_CENTER);
	ECHO_DDR_CENTER &= ~(1 << ECHO_CENTER);
	EICRB |= (1 << ISC50);
	EIMSK |= (1 << INT5);

	// RIGHT
	TRIG_DDR_RIGHT |= (1 << TRIG_RIGHT);
	ECHO_DDR_RIGHT &= ~(1 << ECHO_RIGHT);
	EICRB |= (1 << ISC60);
	EIMSK |= (1 << INT6);
}

// 거리 측정 시작 함수
void ultrasonic_distance_check(void) {
	trigger_ultrasonic();
}
