/*
 * uart1.c
 *
 * Created: 2025-03-20 오전 11:40:13
 *  Author: microsoft
 */ 
#include "uart1.h"
#include "extern.h"

void init_uart1(void);

/*
   BT comportmaster로 부터 1byte기 들어 올때 마다 이속(ISR(USART1_RX_vect)으로 들어 온다.
*/ 

volatile uint8_t bt_data = 0;
ISR(USART1_RX_vect) // 가능한 짧게
{
    bt_data = UDR1;  // BT로 부터 들어온 HW reg(UDR1)을 1byte 읽어 들인다.
                    // bt_data = UDR1;를 실행하면 UDR1의 내용이 빈다.(empty)
					
	UART0_transmit(bt_data);  // BT로 부터 들어온 문자가 어떤것인지 확인 하기 위해서 comport로 출력	
}
/*
  1. 전송속도: 9600bps : 총글자수 :9600/10bit ==> 960자 
     ( 1글자를 송.수신 하는데 소요 시간 : 약 1ms)
  2. 비동기 방식(start/stop부호 비트로 데이터를 구분)
  3. RX(수신) : 인터럽트 방식으로 구현 
*/

void init_uart1(void)
{
   // 1. 9600bps로 설정
   UBRR1H = 0x00;
   UBRR1L = 207;  // 9600bps P219 표9-9
   // 2. 2배속 통신  표9-1
   UCSR1A |= 1 << U2X1;  // 2배속 통신 
   UCSR1C |= 0x06;   // 비동기/data8bits/none parity
   // P215 표9-1
   // RXEN1  : UART1로 부터 수신이 가능 하도록 
   // TXEN1  : UART1로 부터 송신이 가능 하도록 
   // RXCIE1 : UART1로 부터 1byte가 들어오면(stop bit가 들어 오면)) rx interrupt를 발생 시켜라
   UCSR1B |= 1 << RXEN1 | 1 << TXEN1 | 1 << RXCIE1;
   
}