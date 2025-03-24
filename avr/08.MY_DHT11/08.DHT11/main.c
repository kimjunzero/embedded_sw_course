#include "dht11.h"
#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>  // sei()등 
#include <stdio.h>  // printf scanf fgets puts gets 등이 들어 있다. 
extern void init_dht11(void);
extern void init_uart0(void);
extern void UART0_transmit(uint8_t data);
extern volatile uint8_t rx_buff[80];   // uart0로 부터 들어온 문자를 저장 하는 버퍼(변수)
volatile uint8_t rx_msg_received;
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	init_uart0();
	stdout = &OUTPUT;   // printf가 동작 될 수 있도록 stdout에 OUTPUT 화일 포인터 assign 
	sei();   // 전역(대문)으로 interrupt 허용
	
	uint8_t data[5] = {0,};
	int ret = 0;
	
	init_dht11();
	
	while(1)
	{
		if((ret = dht11_main(data)) == OK)
		{
			printf("temp: %d.%d\n", data[2], data[3]);   // data[2] 정수부분, data[3] 소수부분
			printf("humi: %d.%d\n", data[0], data[1]);   // data[0] 정수부분, data[1] 소수부분
		}
		else
			printf("error code : %d\n", ret);
			
		 _delay_ms(1500);   // 안정화 시간
	}
	return 0;
}

