#include "led.h"

void init_led(void);

void init_led(void)
{
	AUTO_LED_PORT_DDR |= 1 << AUTO_LED_PIN;
	AUTO_MODE_PORT_DDR |= 1 << AUTO_MODE_PIN_D1 | 1 << AUTO_MODE_PIN_D2 | AUTO_MODE_PIN_D3;
}