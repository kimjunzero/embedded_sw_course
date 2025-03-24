/*
 * uart0.h
 *
 * Created: 2025-03-10 오전 10:32:07
 *  Author: microsoft
 */ 


#ifndef UART0_H_
#define UART0_H_
#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>  // sei()등
#include "def.h"
volatile uint8_t rx_buff[COMMAND_NUMBER][COMMAND_LENGTH];   // uart0로 부터 들어온 문자를 저장 하는 버퍼(변수)
volatile int rear = 0;  // input index: USART0_RX_vect에서 집어 넣어 주는 index
volatile int front = 0; // output index 
void init_uart0(void);

#endif /* UART0_H_ */