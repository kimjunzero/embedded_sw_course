/*
 * uart.c
 *
 * Created: 2025-03-10 오전 10:31:49
 *  Author: microsoft
 */ 
#include "uart0.h"
#include <avr/io.h>
#include <string.h>
#include <stdio.h>

void init_uart0(void);
void UART0_transmit(uint8_t data);
void pc_command_processing(void);


extern int led_mode = 0;
/*
   PC comportmaster로 부터 1byte가 들어 올 때마다 이속으로 들어온다. (RX INT)
   예) led_all_on\n ==> 11번 이곳으로 들어 온다.
       led_all_off\n 
*/
volatile uint8_t rx_msg_received=0;
ISR(USART0_RX_vect) //ST0P비트가 들어옴과 동시에 INTERRUPT
{
   volatile uint8_t rx_data;
   volatile static int i=0;
   
   //rx_data의 형은 안바꿔줘도됌 -> char로 가져와서 이차원배열에 저장하기 때문
   rx_data = UDR0; // uart0의 H/W register(UDR0)로 부터 1byte를 읽어 들인다.
               // rx_data = UDR0;를 실행하면 UDR0의 내용이 비워진다.(empty)
   if (rx_data == '\n')
   {
      rx_buff[rear++][i] = '\0';
      rear % COMMAND_NUMBER; // rear : 0~9
      i=0;
      // rx_buffer queue full check 하는 logic 추가
      
      
      /* rx_buffer가 1차원 배열일 때
      rx_msg_received = 1; // 완전한 message 수신 
      rx_buffer[i] = '\0'; // 문장의 끝을 NULL로 집어 넣는다.
      i=0;
      */
   }
   else
   {
      rx_buff[rear][i++] = rx_data;
      // COMMAND_LENGTH를 check하는 logic 추가
      
      
   }
}
/*
   1. 전송속도: 9600bps : 총글자수 : 9600/10bit ==>960자
      ( 1글자를 송,수신 하는데 소요 시간 : 약 1ms)
   2. 비동기 방식(start/stop부호 비트로 데이터를 구분)
   3. RX(수신) : 인터럽트 방식으로 구현
*/
void init_uart0(void)
{
   // 1. 9600bps로 설정
   UBRR0H = 0x00; //상위 4비트
   UBRR0L = 207; // 9600bps P219 표 9-9 하위 8비트
   //UBRR0L = 16; // 115200bps P219 표 9-9 하위 8비트
   // 2. 2배속 통신 표9-1
   UCSR0A |= 1 << U2X0; // 2배속 통신
   UCSR0C |= 0x06; // 비동기/data=8bit/none parity
   // P215 표9-1
   // RXEN0 : UART0로부터 수신이 가능하도록
   // TXEN0 : UART0로부터 송신이 가능하도록
   // RXCIE0 : UART0로부터 1byte가 들어오면(STOP BIT가 들어 오면) rx interrupt를 발생 시킨다.
   UCSR0B |= 1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0;
}

// UART0로 1byte를 전송 하는 함수 (polling방식)
void UART0_transmit(uint8_t data)
{
   // 데이터 전송 중이면 전송이 끝날 때 까지 기다린다.
   while (!(UCSR0A & 1 <<UDRE0)); //1이 될때 while 종료
   UDR0 = data; // data를 H/W전송 register에 쏜다.
}

char *fp_name[] = {
   "led_all_on",
   "led_all_off",   // 이 부분도 뒤에서 설명
   "shift_left_ledon",
   "shift_right_ledon",
   "shift_left_keep_ledon",
   "shift_right_keep_ledon",
   "flower_on",
   "flower_off"
};

void pc_command_processing(){
	if(front != rear)
	{
		printf("%s\n", (char*)rx_buff[front]);
		
		for (int i=0;i<8;i++){
			if(strncmp(rx_buff[front], fp_name[i],strlen(fp_name[i]))==NULL){
				printf(("find:%s\n", fp_name[i]));
				led_mode=i;
			}
		}
		front++;
		front %= COMMAND_NUMBER;
	}
}