#include "stm32f1xx_hal.h"                // Device header


void LED_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_name;
	GPIO_name.Pin = GPIO_PIN_1 | GPIO_PIN_2;
	GPIO_name.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_name.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_name.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&GPIO_name);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1 | GPIO_PIN_2 ,GPIO_PIN_SET);
}

void LED1_ON(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
}

void LED1_OFF(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
}

void LED1_turn(void)
{
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1) == GPIO_PIN_1)
	{
		LED1_ON();
	}
	else
	{
		LED1_OFF();
	}
}
