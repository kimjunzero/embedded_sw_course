/*
 * dht11.h
 *
 * Created: 2025-03-18 오전 10:52:39
 *  Author: microsoft
 */ 
#ifndef DHT11_H_
#define DHT11_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>

// 이건 컴파일러가 알아서 해줌.
#define DHT_DDR  DDRG
#define DHT_PORT PORTG
#define DHT_PIN  PING
#define DHT_PIN_NUM 0

int us_count;
enum state_define dht11_state;
enum state_define {OK, TIMEOUT, VALUE_ERROR, TRANS_ERROR}; // 자동적으로 0부터 시작, OK = 10으로 하면 10, 11, 12.. 이렇게 됨. 중간에 넣고 싶으면 중간에 값 넣어주면 됨.
int dht11_main(uint8_t *data);
void init_dht11(void); // 함수 선언
void send_start_signal(void);
uint8_t check_signal(uint8_t expected_state, uint16_t timeout_us);
void process_pulse(uint8_t *pulse, uint8_t *data);

#endif /* DHT11_H_ */