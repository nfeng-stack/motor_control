#include "stm32f1xx_hal.h"                // Device header
#include "bsp_gpio.h"

void LED_Init(void)
{
	bsp_led_gpio_init();
}

void LED1_ON(void)
{
	bsp_led_gpio_set();
}

void LED1_OFF(void)
{
	bsp_led_gpio_reset();
}

void LED1_turn(void)
{
	if(!bsp_led_gpio_read())
	{
		LED1_ON();
	}
	else
	{
		LED1_OFF();
	}
}
