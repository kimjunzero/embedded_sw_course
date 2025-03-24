#ifndef LED_H_
#define LED_H_

#include <avr/io.h>  // PORTA PORTB등의 I/O 관련 register등이 있다. 

// 자동/수동 모드용 LED
#define AUTO_LED_PORT PORTG
#define AUTO_LED_PORT_DDR DDRG
#define AUTO_LED_PIN 3

// 자동 모드에서 모드 단계 확인용 LED
#define AUTO_MODE_PORT PORTF
#define AUTO_MODE_PORT_DDR DDRF
#define AUTO_MODE_PIN_D1 5
#define AUTO_MODE_PIN_D2 6
#define AUTO_MODE_PIN_D3 7

#endif /* LED_H_ */