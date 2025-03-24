/*
 * dht11.c
 *
 * Created: 2025-03-18 오전 10:52:55
 *  Author: microsoft
 */ 

#include "dht11.h"

void init_dht11();
void dht11_main();

void init_dht11()
{
   DHT_DDR |= 1 << DHT_PIN_NUM;
   DHT_PORT |= 1 << DHT_PIN_NUM;
}

void dht11_main()
{
   uint8_t data[5] = {0,};   // 5bytes(humi(2) + temp(2) + checksum(1)
   
   init_dht11();
   
   while(1)
   {
      memset(data,0,sizeof(data));
      dht11_state=OK;
      //=========== step1 : start signal  ==========
	  
      init_dht11();
      _delay_ms(100);
	  
      DHT_PORT &= ~(1 << DHT_PIN_NUM);   // DATA LOW
      _delay_ms(20);
      
      DHT_PORT |= 1 << DHT_PIN_NUM;   // DATA HIGH
      DHT_DDR &= ~(1 << DHT_PIN_NUM);   // input mode
      _delay_us(1);
	  
      // response check
      us_count=0;
      while( (DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM )   // 아직도 HIGH인지 check
      {
         _delay_us(2);
         us_count += 2;
         if(us_count > 50)   // 50us만큼 기다렸는데도 HIGH이면 ERROR
         {
            dht11_state=TIMEOUT;
            break;
         }
      }
      //============ step2 : response signal check ============
      if (dht11_state ==OK)   // 정상 상태면 DATA pin이 LOWA이다.
      {
         // response check
         us_count=0;
         while( !((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM ))   // LOW일 동안 반복
         {
            _delay_us(2);
            us_count += 2;
            if(us_count > 100)   // (spec에서는 80us인데 여유 생각해서 100us) 100us만큼 기다렸는데도 LOW이면 ERROR(dht11_state=TIMEOUT)
            {
               dht11_state=TIMEOUT;
               break;
            }
         }
      }
      // response HIGH check
      if (dht11_state == OK)   // 정상 상태면 DATA pin이 LOWA이다.
      {
         // response check
         us_count = 0;
         while( (DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM )   // HIGH일 동안 반복
         {
            _delay_us(2);
            us_count += 2;
            if(us_count > 100)   // (spec에서는 80us인데 여유 생각해서 100us) 100us만큼 기다렸는데도 HIGH이면 ERROR
            {
               dht11_state=TIMEOUT;
               break;
            }
         }
      }
      // dht11_state=OK이면 DATA LINE(PORTG.0)은 LOW상태이다.
      // start signal request & response는 정상적으로 끝났다.
      
      
      //========== step3 : data bit 수신 from DHT11 ===========
      // 이제부터 40개의 pulse를 count한다. (습도H, 습도L, 온도H, 온도L, checksum) 8 8 8 8bit를 묶는 작업
      // '0' --> 50us동안 LOW, 26~28us동안 HIGH
      // '1' --> 50us동안 LOW, 70us동안 HIGH
      if(dht11_state==OK)
      { 
         for(int i=0; i < 5; i++)   // 5bytes 이기 때문
         {
             uint8_t pulse[8] = {0,};   // 1개의 pulse를 담는 그릇(변수)
			for (int j=7; j >=0; j--)   // byte당 8bit 이다
            {
               // LOW 확인 50us check
               // response check
               us_count=0;
               while( !((DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM ))   // LOW일 동안 반복
               {
                  _delay_us(2);
                  us_count += 2;
                  if(us_count > 70)   // (spec에서는 50us인데 여유 생각해서 70us) 70us만큼 기다렸는데도 LOW이면 ERROR(dht11_state=TIMEOUT)
                  {
                     dht11_state=TIMEOUT;
                     i=5;
                     j=-1;   // for문 전체 탈출
                     break;
                  }
               }
               
               // 이시점에 정상이면 dht11_state=OK이고 전압은 HIGH(PG.0)
               if (dht11_state==OK)
               {
                  // response check
                  us_count=0;
                  while( (DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM )   // HIGH일 동안 반복
                  {
                     _delay_us(2);
                     us_count += 2;
                     if(us_count > 90)   // '0': HIGH길이: 26~28us, '1': HIGH길이: 70us인데 이것보다 길게 90us
                     {
                        dht11_state=TIMEOUT;
                        i=5;
                        j=-1;   // for문 전체 탈출
                        break;
                     }
                  }
                  if(dht11_state ==OK)
                  {
                     if (us_count < 40)   // '0'
                        pulse[j] =0;
                     else if (us_count >= 40)   // '1'
                        pulse[j] =1;
                  }
               }
            }
            if (dht11_state == OK)   // pulse를 8개를 정상 처리했으면
            {
               data[i] = pulse[0] << 0 | pulse[1] << 1 | pulse[2] << 2 | pulse[3] << 3 |
                       pulse[4] << 4 | pulse[5] << 5 | pulse[6] << 6 | pulse[7] << 7;
            }
         }   // for문 끝
         // check-sum check
         if (dht11_state == OK)
         {
            if (data[4] != data[0]+data[1]+data[2]+data[3])
				dht11_state = VALUE_ERROR;
         }
         _delay_us(50);   // SPEC에는 50us인데 여유 생각해서 60us
         // HIGH가 유지 되면 정상 전압: HIGH
         // response check
         us_count=0;
         while( (DHT_PIN & 1 << DHT_PIN_NUM) >> DHT_PIN_NUM )   // HIGH일 동안 반복
         {
            _delay_us(2);
            us_count += 2;
            if(us_count > 90)   // '0': HIGH길이: 26~28us, '1': HIGH길이: 70us인데 이것보다 길게 90us
            {

			   break;
            }
         }
         if (us_count < 90)   // 무언가 data가 계속 발생
         {
            dht11_state = TRANS_ERROR;
         }
      }
      // 값을 출력--------------
      if(dht11_state == OK)
      {
         printf("temp: %d.%d\n", data[2], data[3]);   // data[2] 정수부분, data[3] 소수부분
         printf("humi: %d.%d\n", data[0], data[1]);   // data[0] 정수부분, data[1] 소수부분
      }
      if(dht11_state != OK)
      {
         printf("error code : %d\n", dht11_state);
      }
      _delay_ms(1500);   // 안정화 시간
      
   }
}