/*
 * uart1.c
 *
 * Created: 2025-03-20 오전 11:39:58
 *  Author: microsoft
 */ 

#include "uart1.h"
#include "extern.h"

void init_uart1(void);

/*
PC comportmaster로 부터 1byte가 들어오면 (즉, stop bit가 감지되면) RX complete인터럽트가 호출된다
== 한 바이트마다 인터럽트 처리를 해야하는것!
*/
volatile uint8_t bt_data=0;
ISR(USART1_RX_vect)
{
	bt_data = UDR1; // BT로부터 들어온 HW reg(UDR1)을 1byte 읽어들인다.
					// bt_data = UDR1;를 실행하면 UDR1의 내용이 빈다.(empty)
					
	UART0_transmit(bt_data);	// BT로부터 들어온 문자가 어떤것인지 확인하기 위해서 comportmaster로 출력
}

/*
[init에 고려해야할것]

전송속도 : 9600bps -> 1초에 960자를 받음
				1글자 송수신에 약 1ms가 걸린다 (1글자 == 10비트)

비동기 방식 : start, stop 비트의 활용

RX : interrupt (받는것)
TX : polling (보내는것)

*/
void init_uart1(void)
{
	// 전송속도 설정
	UBRR1H = 0x00;
	UBRR1L = 207; // 2배속 모드임
	//UBRR0L = 16;
	//UBRR0L = 832;
	
	UCSR1A |= 1 << U2X1; // 2배속 통신
	
	UCSR1C |= 0x06; // 비동기, none 8bit, data 8 bit
	
	// 인터럽트, uart 기능 설정
	//UCSR1B |= 1 << RXEN1; // 수신 허용
	//UCSR1B |= 1 << TXEN1; // 송신 허용
	//UCSR1B |= 1 << RXCIE1; // 수신 인터럽트 허용
	UCSR1B |= 1 << RXEN1 | 1 << TXEN1 | 1 << RXCIE1;
}
