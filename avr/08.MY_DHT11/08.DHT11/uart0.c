/*
 * uart0.c
 *
 * Created: 2025-03-10 오전 10:32:31
 *  Author: microsoft
 */ 
#include "uart0.h"
#include "led.h"
#include <string.h> // strcpy strcmp strcmp...

/*
   PC comportmaster로 부터 1byte기 들어 올때 마다 이속(ISR(USART0_RX_vect)으로 들어 온다. (RX INT)
   예)  led_all_on\n ==> 11번 이곳으로 들어 온다 
        led_all_off\n
*/

int mode = 0;
volatile uint8_t rx_msg_received=0;
ISR(USART0_RX_vect) // 가능한 짧게
{
   volatile uint8_t rx_data;
   volatile static int i=0;
   
   rx_data = UDR0;  // uart0의 H/W register(UDR0)로 부터 1byte를 읽어 들인다. 
                    // rx_data = UDR0;를 실행하면 UDR0의 내용이 빈다.(empty)
   if (rx_data == '\n')
   {
      rx_buff[rear++][i] = '\0';
      rear %= COMMAND_NUMBER; // rear : 0 ~ 9
      i = 0; // 다음 string을 저장하기 위한 1차원 index값을 0으로
      // !!!! rx_buff queue full check 하는 logic 추가 
   }
   else
   {
      rx_buff[rear][i++] = rx_data;
      // COMMAND_LENGTH를 check하는 logic 추가
   }
}
/*
  1. 전송속도: 9600bps : 총글자수 :9600/10bit ==> 960자 
     ( 1글자를 송.수신 하는데 소요 시간 : 약 1ms)
  2. 비동기 방식(start/stop부호 비트로 데이터를 구분)
  3. RX(수신) : 인터럽트 방식으로 구현 
*/

void init_uart0(void)
{
   // 1. 9600bps로 설정
   UBRR0H = 0x00;
   UBRR0L = 207;  // 9600bps P219 표9-9
   // 2. 2배속 통신  표9-1
   UCSR0A |= 1 << U2X0;  // 2배속 통신 
   UCSR0C |= 0x06;   // 비동기/data8bits/none parity
   // P215 표9-1
   // RXEN0 : UART0로 부터 수신이 가능 하도록 
   // TXEN0 : UART0로 부터 송신이 가능 하도록 
   // RXCIE0 : UART0로 부터 1byte가 들어오면(stop bit가 들어 오면)) rx interrupt를 발생 시켜라
   UCSR0B |= 1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0;
   
}

// UART0로 1byte를 전송 하는 함수 (polling방식)
void UART0_transmit(uint8_t data)
{
   // 데이터 전송 중이면 전송이 끝날떄 까지 기다린다. 
   while ( !(UCSR0A & 1 << UDRE0))
      ;   // no operation
   UDR0 = data;  // data를 H/W전송 register에 쏜다. 
}

void pc_command_processing(void)
{
   if(front != rear) // rx_buff에 data가 존재. 초기상태는 font == rear이 같음
   {
      printf("%s\n", rx_buff[front]);  // &rx_buff[front][0]
      if(strncmp(rx_buff[front], "led_all_off", strlen("led_all_off")) == NULL)
      {
         printf("find: led_all_off\n");
		 mode = 0;
      }
	  else if(strncmp(rx_buff[front], "led_all_on", strlen("led_all_on")) == NULL)
	  {
		  printf("find: led_all_on\n");
		  mode = 1;
	  }
	  else if(strncmp(rx_buff[front], "shift_left_ledon", strlen("shift_left_ledon")) == NULL)
	  {
		  printf("find: led_left_ledon\n");
		  mode = 2;
	  }
	  else if(strncmp(rx_buff[front], "shift_right_ledon", strlen("shift_right_ledon")) == NULL)
	  {
		  printf("find: led_right_ledon\n");
		  mode = 3;
	  }
	  else if(strncmp(rx_buff[front], "shift_left_keep_ledon", strlen("shift_left_keep_ledon")) == NULL)
	  {
		  printf("find: shift_left_keep_ledon\n");
		  mode = 4;
	  }
	  else if(strncmp(rx_buff[front], "shift_right_keep_ledon", strlen("shift_right_keep_ledon")) == NULL)
	  {
		  printf("find: shift_right_keep_ledon\n");
		  mode = 5;
	  }
	  else if(strncmp(rx_buff[front], "flower_on", strlen("flower_on")) == NULL)
	  {
		  printf("find: flower_on\n");
		  mode = 6;
	  }
	  else if(strncmp(rx_buff[front], "flower_off", strlen("flower_off")) == NULL)
	  {
		  printf("find: flower_off\n");
		  mode = 7;
	  }
	  
      front++;
      front %= COMMAND_NUMBER;
      // !!!!!! queue full check하는 logic들어 가야 한다. !!!!!
   }
}