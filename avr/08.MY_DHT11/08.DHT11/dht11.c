#include "dht11.h"
// expected_state 0: low 신호 보내기 기다리는 상태
//                1: high 신호 보내기를 기다리는 상태
void init_dht11(void)
{
	DHT_DDR |= 1 << DHT_PIN_NUM;  // 데이터 핀을 출력 모드로 설정
	DHT_PORT |= 1 << DHT_PIN_NUM; // 데이터 핀을 high 상태로 유지(센서 대기 상태)
}

void send_start_signal(void)
{
	init_dht11(); // 초기화
	_delay_ms(100); // 대기
	
	DHT_PORT &= ~(1 << DHT_PIN_NUM); // LOW로 변경
	_delay_ms(20); // 대기
	
	DHT_PORT |= 1 << DHT_PIN_NUM; // 다시 high
	DHT_DDR &= ~(1 << DHT_PIN_NUM); // 데이터 핀을 입력 모드로 전환
	_delay_us(1);
}

// 신호를 보낼 준비가 되었는지 확인
uint8_t check_signal(uint8_t expected_state, uint16_t timeout_us)
{
	uint16_t us_count = 0;
	while (((DHT_PIN & (1 << DHT_PIN_NUM)) >> DHT_PIN_NUM) != expected_state)
	{
		_delay_us(2);
		us_count += 2;
		if(us_count > timeout_us) // 시간 초과 하면 TIMEOUR
		{
			return TIMEOUT;
		}		
	}
	return OK;
}

void process_pulse (uint8_t *pulse, uint8_t *data)
{
	for(int i = 0; i<5; i++)
	{
		for(int j=7; j>=0; j--)
		{
			if(check_signal(0,70) == TIMEOUT || check_signal(1, 90) == TIMEOUT)
			{
				return;
			}
			uint16_t high_duration = 0;
			while ((DHT_PIN & (1<< DHT_PIN_NUM)) >> DHT_PIN_NUM)
			{
				_delay_us(2);
				high_duration += 2;
				if(high_duration > 90) return;
			}
			pulse[j] = (high_duration < 40) ? 0 : 1;
		}
		data[i] = pulse[0] << 0 | pulse[1] << 1 | pulse[2] << 2 | pulse[3] << 3 | 
				  pulse[4] << 4 | pulse[5] << 5 | pulse[6] << 6 | pulse[7] << 7;
	}
}

int dht11_main(uint8_t *data)
{
	memset(data,0,sizeof(data)); // 메모리 초기화
	dht11_state =OK;
		
	send_start_signal();
		
	if(check_signal(0, 100)==TIMEOUT || check_signal(1, 100) == TIMEOUT)
	{
		return TIMEOUT;
	}
		
	uint8_t pulse[8] = {0};
	process_pulse(pulse, data);
			
	if(dht11_state == OK && data[4] != data[0]+data[1]+data[2]+data[3])
	{
		dht11_state = VALUE_ERROR;
	}
		
	return dht11_state;	
		
}


