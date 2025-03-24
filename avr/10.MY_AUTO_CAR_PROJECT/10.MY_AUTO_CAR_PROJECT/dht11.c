/*
 * dht11.c
 *
 * Created: 2025-03-18 오전 10:52:34
 *  Author: microsoft
 */ 

#include "dht11.h"

void init_dht11(void);
int dht11_main2(uint8_t *data);

char * error_log = " ";

void init_dht11(void)
{
	DHT_DDR |= 1 << DHT_PIN_NUM;
	DHT_PORT |= 1 << DHT_PIN_NUM; // idle상태가 HIGH임
}

void signal_us_check(int duration, int binary_choice)
{
	us_count = 0;
	while(((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM) == binary_choice)
	{
		_delay_us(2);
		us_count += 2;
		if(us_count > duration)
		{
			state = TIMEOUT;
			return;
		}
	}
}

int dht11_get_bit(void)
{
	us_count = 0;
	while((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM)
	{
		_delay_us(2);
		us_count += 2;
		if(us_count > 90)
		{
			state = TIMEOUT;
			return -1;
		}
	}
	
	if(us_count < 40) return 0;
	else if(us_count >= 40) return 1;
}

int dht11_main2(uint8_t *data)
{
	
	init_dht11();
	
	while(1)
	{
		_delay_ms(1000);
		
		memset(data, 0, sizeof(data)); // data 초기화
		state = OK;
		// ================ step 1 : start signal ======================
		// idel상태인 HIGH를 100ms 유지 -> 이 후 LOW로 바꾸어 준다.
		init_dht11();
		_delay_ms(100);
		
		DHT_PORT &= ~(1 << DHT_PIN_NUM);
		_delay_ms(20); // LOW상태를 최소 18ms 유지해야함
		
		DHT_PORT |= 1 << DHT_PIN_NUM; // start signal을 보낸 다음에는 HIGH로 유지해야함
		DHT_DDR &= ~(1 << DHT_PIN_NUM); // input모드로 변경
		_delay_us(1);
		
		// response signal check
		signal_us_check(50, HIGH);
		
		if(state != OK)
		{
			error_log = "response signal check fail";
			continue;
		}
		// ============= step 2 : response signal check =============
		// step 2 실행 (정상적으로 DATA pin에 LOW가 감지된것)
		signal_us_check(100, LOW);
		
		if(state != OK)
		{
			error_log = "step 2, response signal low check fail";
			continue;
		}
		
		// response high 체크
		// LOW가 80us 정도 유지되고 (그니까 100us이내에) 잘 HIGH를 유지했는지 확인
		signal_us_check(100, HIGH);
		
		if(state != OK)
		{
			error_log = "step2, response signal high fail";
			continue;
		}
		// 여기서 state가 OK면 hand shaking이 잘 마무리된것!!
		// == DTAT LINE은 지금 LOW 상태임
		
		
		// ============= step 3 : data bit receive from DHT11 =============
		// 40개의 pulse를 count 한다
		// -> 8비트식 한 바이트를 이루어 의미를 띄므로, 8비트씩 묶어야함
		// '0' : LOW 50us, HIGH 26~28us
		// '1' : LOW 50us, HIGH 70us

		for(int i = 0; i<5; i++)
		{
			uint8_t pulse[8] = {0,}; // 1개의 pulse를 저장할 변수 (8비트 저장)
			for(int j = 7; j>=0; j--)
			{
				// LOW 50us 확인
				signal_us_check(70, LOW);
				if(state == TIMEOUT)
				{
					i = 5;
					j = -1;
					error_log = "get bit, 50us low check fail";
				}
					
				// state 확인 필수!
				// HIGH가 몇 us인지 확인
				int dht11_bit = dht11_get_bit();
				if(state == OK) pulse[j] = dht11_bit;
				else error_log = "get bit, high fail";
			}	
			// pulse를 가지고 data로 넣어주기!
			if(state == OK)
			{
				data[i] = pulse[0] << 0 | pulse[1] << 1 | pulse[2] << 2 | pulse[3] << 3 |
							pulse[4] << 4 | pulse[5] << 5 | pulse[6] << 6 | pulse[7] << 7;
			}else{
				error_log = "get bit fail";
			}
		}
			
		// 5바이트 다 받아옴
		// check sum 확인!!
		if(state == OK)
		{
			if(data[4] != data[0] + data[1] + data[2] + data[3])
			{
				// check sum이 맞지 않는 상황
				state = VALUE_ERROR;
			}
		}else{
			error_log = "get bit fail 2";
		}
		_delay_us(60); // 제일 마지막 단계!! 50us LOW상태 유지
			
		
		// 값 출력
		if(state == OK)
		{
			return 1;
		}
		else
		{
			return 0;
		}
		
		_delay_ms(2000); // 2초정도 안정화 시간이 필요하당
	}
}

void dht11_main(void)
{
	uint8_t data[5] = {0,}; // 온습도 데이터 5바이트를 저장할 공간
	init_dht11();
	
	while(1)
	{
		memset(data, 0, sizeof(data)); // data 초기화
		state = OK;
		
		// ================ step 1 : start signal ======================
		// idel상태인 HIGH를 100ms 유지 -> 이 후 LOW로 바꾸어 준다.
		init_dht11();
		_delay_ms(100);
		
		DHT_PORT &= ~(1 << DHT_PIN_NUM);
		_delay_ms(20); // LOW상태를 최소 18ms 유지해야함
		
		DHT_PORT |= 1 << DHT_PIN_NUM; // start signal을 보낸 다음에는 HIGH로 유지해야함
		DHT_DDR &= ~(1 << DHT_PIN_NUM); // input모드로 변경
		_delay_us(1);
		
		// response signal check
		us_count = 0;
		while((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM)
		{
			// HIGH상태인지 check한다
			_delay_us(2);
			us_count += 2;
			if(us_count > 50)
			{
				// HIGH상태가 50us가 지나도 안떨어진다
				// == response가 안들어 온거니까 문제가 생긴것
				state = TIMEOUT;
				break;
			}
		}
		
		// ============= step 2 : response signal check =============
		if(state == OK)
		{
			// step 2 실행 (정상적으로 DATA pin에 LOW가 감지된것)
			us_count = 0;
			while(!((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM))
			{
				// LOW상태가 얼마나 지속되는지 check한다
				_delay_us(2);
				us_count += 2;
				if(us_count > 100)
				{
					// LOW상태가 100us가 지나도 HIGH로 안올라가면
					// 뭔가 정상적인 반응이 되돌아온게 아닌것!!
					state = TIMEOUT;
					break;
				}
			}
		}
		
		// response high 체크
		if(state == OK)
		{
			// LOW가 80us 정도 유지되고 (그니까 100us이내에) 잘 HIGH를 유지했는지 확인
			us_count = 0;
			while((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM)
			{
				// HIGH상태가 얼마나 지속되는지 check한다
				_delay_us(2);
				us_count += 2;
				if(us_count > 100)
				{
					// HIGH상태가 100us가 지나도 안떨어지면 뭔가 문제!
					state = TIMEOUT;
					break;
				}
			}
		}
		
		// 여기서 state가 OK면 hand shaking이 잘 마무리된것!!
		// == DTAT LINE은 지금 LOW 상태임
		
		
		// ============= step 3 : data bit receive from DHT11 =============
		// 40개의 pulse를 count 한다
		// -> 8비트식 한 바이트를 이루어 의미를 띄므로, 8비트씩 묶어야함
		// '0' : LOW 50us, HIGH 26~28us
		// '1' : LOW 50us, HIGH 70us
		if(state == OK)
		{
			
			for(int i = 0; i<5; i++)
			{
				uint8_t pulse[8] = {0,}; // 1개의 pulse를 저장할 변수 (8비트 저장)
				for(int j = 7; j>=0; j--)
				{
					// LOW 50us 확인
					us_count = 0;
					while(!((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM))
					{
						// LOW상태가 얼마나 지속되는지 check한다
						_delay_us(2);
						us_count += 2;
						if(us_count > 70) // 여유둬서 70으로 check
						{
							state = TIMEOUT;
							j = -1;
							i = 5; // for문 두개를 한번에 빠져나가기 위해
							break;
						}
					}
					
					// state 확인 필수!
					// HIGH가 몇 us인지 확인
					if(state == OK)
					{
						us_count = 0;
						while((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM)
						{
							// HIGH상태가 얼마나 지속되는지 check한다
							_delay_us(2);
							us_count += 2;
							if(us_count > 90) // 70인데, 약간 여유주는것!
							{
								// HIGH상태가 90us가 지나도 안떨어지면 뭔가 문제!
								state = TIMEOUT;
								j = -1;
								i = 5; // for문 두개를 한번에 빠져나가기 위해
								break;
							}
						}
					}
					
					// '0' : HIGH 26~28us
					// '1' : HIGH 70us
					if(state == OK)
					{
						if(us_count < 40) // '0'
						{
							pulse[j] = 0;
						}else if(us_count >= 40) // '1'
						{
							pulse[j] = 1;
						}
					}
				}	
				// pulse를 가지고 data로 넣어주기!
				if(state == OK)
				{
					data[i] = pulse[0] << 0 | pulse[1] << 1 | pulse[2] << 2 | pulse[3] << 3 |
							  pulse[4] << 4 | pulse[5] << 5 | pulse[6] << 6 | pulse[7] << 7;
				}
			}
			
			// 5바이트 다 받아옴
			// check sum 확인!!
			if(state == OK)
			{
				if(data[4] != data[0] + data[1] + data[2] + data[3])
				{
					// check sum이 맞지 않는 상황
					state = VALUE_ERROR;
				}
			}
			_delay_us(60); // 제일 마지막 단계!! 50us LOW상태 유지
			
			// 이 다음 HIGH를 유지해줘야함
			/*
			us_count = 0;
			while((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM)
			{
				// HIGH상태가 얼마나 지속되는지 check한다
				_delay_us(2);
				us_count += 2;
				if(us_count > 90)
				{
					// HIGH상태가 100us가 지나도 안떨어지면 뭔가 문제!
					//printf("timeout\n");
					state = TIMEOUT;
					break;
				}
			}
			
			if(us_count < 90)
			{
				// 데이터시트에는 안나와있는데
				// HIGH를 90us동안 유지하지 않으면
				// 뭔가 data가 계속 발생하고 있는것
				state = TRANS_ERROR;
			}*/
		}
		// 값 출력
		if(state == OK)
		{
			return state;
		}
		else
		{
			return state;
		}
		
		_delay_ms(2000); // 2초정도 안정화 시간이 필요하당
	}
}