#include <avr/io.h>
#include <util/delay.h>

// 모터 제어 핀 정의
#define IN1 PF0
#define IN2 PF1
#define IN3 PF2
#define IN4 PF3
#define ENA PB5
#define ENB PB6

void init_motor_pins() {
	// 모터 제어 핀을 출력으로 설정
	DDRF |= (1 << IN1) | (1 << IN2) | (1 << IN3) | (1 << IN4);
	DDRB |= (1 << ENA) | (1 << ENB);

	// 모터 정지 (초기 상태)
	PORTF &= ~((1 << IN1) | (1 << IN2) | (1 << IN3) | (1 << IN4));
	PORTB &= ~((1 << ENA) | (1 << ENB));
}

void motor_forward() {
	// 모터 정지 후 방향 설정 (안정성 확보)
	PORTB &= ~((1 << ENA) | (1 << ENB));
	PORTF &= ~((1 << IN1) | (1 << IN2) | (1 << IN3) | (1 << IN4));
	_delay_ms(50);

	// 정방향 회전
	PORTF |= (1 << IN1);
	PORTF &= ~(1 << IN2);
	PORTF |= (1 << IN3);
	PORTF &= ~(1 << IN4);

	// 모터 드라이버 활성화
	PORTB |= (1 << ENA) | (1 << ENB);
}

void motor_stop() {
	// 모든 모터 신호 LOW로 설정
	PORTF &= ~((1 << IN1) | (1 << IN2) | (1 << IN3) | (1 << IN4));
	PORTB &= ~((1 << ENA) | (1 << ENB));
}

int main() {
	init_motor_pins(); // 모터 핀 초기화

	while (1) {
		motor_forward();  // 전진
		_delay_ms(500);
	}
}
