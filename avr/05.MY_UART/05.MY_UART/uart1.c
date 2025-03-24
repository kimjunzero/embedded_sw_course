/*
 * uart1.c
 *
 * Created: 2025-03-11 오후 4:35:42
 *  Author: microsoft
 */ 

#include "uart1.h"
#include <avr/io.h>
#include <string.h>
#include <stdio.h>

extern int led_mode;

ISR(USART1_RX_vect) //ST0P비트가 들어옴과 동시에 INTERRUPT
{
	volatile uint8_t rx1_data;
	volatile static int i=0;
	
	rx1_data = UDR1; // uart0의 H/W register(UDR0)로 부터 1byte를 읽어 들인다.
	// rx_data = UDR0;를 실행하면 UDR0의 내용이 비워진다.(empty)
	if (rx1_data == '\n') {
		rx1_buff[rear1++][i] = '\0'; ////////2차원 배열일 때
		rear1 % COMMAND_NUMBER; // 0~9
		i=0;
		
	}
	else {
		rx1_buff[rear1][i++] = rx1_data;
		// COMMAND_LENGTH를 check하는 logic 추가
	}
}

void init_uart1(void){
   // 1. 9600bps로 설정
   UBRR1H = 0x00;
   UBRR1L = 207; //9600bps. (p219 표9-9)

   
   // 2. 2배속 통신 (표9-1)
   UCSR1A |= 1<< U2X1; // 2배속 통신
   
   //UCSRnC   -       0     0      0 _      0      1    1    0
   //             비동기     parity(none)   1stop    databit 수
   UCSR1C |= 0x06; //(비동기, data8bit, none parity)
   
   //UCSRnB 표9-2)7,4,3
   // p215 표 9-1
   // RXEN0: UART1로부터 수신이 가능하도록 4
   // TXEN0: UART1로부터 송신이 가능하도록 3
   // RXCIE0 : UART1로부터 1byte가 들어오면 (stop bit가 들어오면) rx interrupt를 발생시켜라 7
   UCSR1B |= 1 << RXEN1 | 1 << TXEN1 | 1 << RXCIE1;
}

//void UART1_transmit(uint8_t data){
//   // 데이터 전송 중이면 전송이 끝날때까지 기다린다 p215 UDERn (전송중:0),
//   while (!(UCSR1A & 1<<UDRE1))
//   ; // no operation
//   UDR1 = data; // data를 H/W전송 register에 쏜다.
//}




/*
void bt_command_processing(void){
   
   if(front1 != rear1){ // rx_buffer에 data가 존재하면
      printf("%s\n", rx1_buff[front1]); 
      if (strncmp(rx1_buff[front1], "led_all_on", strlen("led_all_on")) == NULL) {
         printf("find: led_all_on\n");
      }
      front1++;
      front1 %= COMMAND_NUMBER;
   }
}
*/

////

char *fp1_name[] = {
   "led_all_on",
   "led_all_off",
   "shift_left_ledon",
   "shift_right_ledon",
   "shift_left_keep_ledon",
   "shift_right_keep_ledon",
   "flower_on",
   "flower_off"
};


void bt_command_processing(){
   if(front1 != rear1)
   {
      printf("%s\n", (char*)rx1_buff[front1]);
      
      for (int i=0;i<8;i++){
         if(strncmp(rx1_buff[front1], fp1_name[i],strlen(fp1_name[i]))==NULL){
            printf(("find:%s\n", fp1_name[i]));
            led_mode=i;
         }
      }
      front1++;
      front1 %= COMMAND_NUMBER;
   }
}

//void rx1_msg_received(void){
//   while(!(UCSR1A&(1<<RXC1)));
//   return UDR1;
//}