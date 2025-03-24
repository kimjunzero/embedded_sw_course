/*
 * dht11.h
 *
 * Created: 2025-03-18 오전 10:52:27
 *  Author: microsoft
 */ 


#ifndef DHT11_H_
#define DHT11_H_

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>		// printf

#define DHT_DDR		DDRG
#define DHT_PORT	PORTG
#define DHT_PIN		PING
#define DHT_PIN_NUM	0

#define HIGH 1
#define LOW  0

uint8_t us_count = 0;
enum state_define {OK, TIMEOUT, VALUE_ERROR, TRANS_ERROR};
// #define 나열하는것보다 enum 쓰는게 더 깔끔할 수 있음

enum state_define state = OK;	// 앞으로 관리할 state변수 생성 (초기값 OK)




#endif /* DHT11_H_ */