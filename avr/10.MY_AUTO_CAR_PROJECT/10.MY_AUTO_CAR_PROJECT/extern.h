#ifndef EXTERN_H_
#define EXTERN_H_

extern void init_button(void);
extern int get_button(int button_num, int button_pin);
extern void auto_mode_check(void);
extern void init_led(void);
extern volatile int button_state;

extern void init_timer1(void);
extern void init_L298N(void);
extern void stop(void);
extern void forward(int speed);
extern void backward(int speed);
extern void turn_left(int speed);
extern void turn_right(int speed);

extern int fnd_main(void);

extern void init_uart0(void);
extern void UART0_transmit(uint8_t data);
extern volatile uint8_t bt_data;
extern int func_index;

extern void init_ultrasonic(void);
extern void distance_ultrasonic(void);
extern void ultrasonic_distance_check(void);

extern void init_speaker(void);
extern void process_siren(void); // 자동모드용 노래
// extern void process_reverse_beep(mode, direction); // 수동 모드용 노래
extern void sound_on(uint8_t mode);

extern void fnd_display(void);
extern void init_fnd(void);
#endif /* EXTERN_H_ */