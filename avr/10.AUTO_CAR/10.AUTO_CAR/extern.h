#ifndef EXTERN_H_
#define EXTERN_H_

extern void init_button(void);
extern int get_button(int button_num, int button_pin);

extern void init_timer1(void);
extern void init_L298N(void);
extern void stop(void);
extern void forward(int speed);
extern void backward(int speed);
extern void turn_left(int speed);
extern void turn_right(int speed);

extern void init_uart0(void);
extern void UART0_transmit(uint8_t data);

extern void distance_ultrasonic(void);
extern void init_ultrasonic(void);

extern void init_led(void);
extern void auto_mode_check(void);
extern void init_uart1(void);

extern volatile uint8_t rx_buff[80];   // uart0로 부터 들어온 문자를 저장 하는 버퍼(변수)
extern volatile uint8_t rx_msg_received;
extern volatile uint8_t bt_data;
extern int func_index;

#endif /* EXTERN_H_ */