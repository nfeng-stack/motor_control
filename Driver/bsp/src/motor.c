#include "stm32f1xx_hal.h"                // Device header
#include "PWM.h"

unsigned char Pin_value1 = 0;
unsigned char Pin_value2 = 0;

void Motor_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_name;
	GPIO_name.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_name.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 ;
	GPIO_name.Pull = GPIO_NOPULL ;
	GPIO_name.Speed = GPIO_SPEED_FREQ_HIGH ;
	HAL_GPIO_Init(GPIOA,&GPIO_name);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1 | GPIO_PIN_2,GPIO_PIN_SET);		
	PWM_Init();
}	

void Motor_Astate(uint8_t state)
{
	if(state == 1)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
	}
	else if(state == 0)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	}
	else if(state == 2)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	}
}

void Motor_Aspeed(int8_t speed)
{
	PWM_Setcompare3(speed);
}

void Motor_Bstate(uint8_t state)
{
	if(state == 1)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
	}
	else if(state == 0)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
	}
	else if(state == 2)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
	}
}

void Motor_Bspeed(int8_t speed)
{
	PWM_Setcompare2(speed);
}



unsigned char show1(void)
{
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4) == GPIO_PIN_SET)
	{
		Pin_value1 = 1;
	}
	else
	{Pin_value1 = 0;}
	return Pin_value1;
}

unsigned char show2(void)
{
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5) == GPIO_PIN_SET)
	{
		Pin_value2 = 1;
	}
	else
	{Pin_value2 = 0;}
	return Pin_value2;
}
