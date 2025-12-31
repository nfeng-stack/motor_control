#include "stm32f1xx_hal.h"               // Device header
#include "stm32f1xx_ll_tim.h"

void PWM_Init(void)
{
	__HAL_RCC_TIM2_CLK_ENABLE(); //开启定时器时钟
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_TIM3_CLK_ENABLE();	//舵机	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	TIM_HandleTypeDef htim2 = {0};
	TIM_HandleTypeDef htim3 = {0};
	GPIO_InitTypeDef GPIO_name = {0};
	GPIO_name.Pin = GPIO_PIN_2 | GPIO_PIN_1 ;
	GPIO_name.Mode = GPIO_MODE_AF_PP ;
	GPIO_name.Pull = GPIO_NOPULL ;
	GPIO_name.Speed = GPIO_SPEED_FREQ_HIGH ;
	HAL_GPIO_Init(GPIOA,&GPIO_name);

	htim2.Instance = TIM2 ;
	htim2.Init.Prescaler = 72 -1;/* 因为tim的时钟来自内部，apb1如果apb1分频为1，则timclk倍频1否则2*/
	htim2.Init.Period = 1000 -1;
	htim2.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_ENABLE ;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1 ;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP ;
	htim2.Init.RepetitionCounter = 0 ;
	HAL_TIM_PWM_Init(&htim2);

	TIM_ClockConfigTypeDef tim_clk = {0};
	tim_clk.ClockSource = TIM_CLOCKSOURCE_INTERNAL ;
	HAL_TIM_ConfigClockSource(&htim2,&tim_clk);

	TIM_SlaveConfigTypeDef tim_slave_cfg = {0};
	tim_slave_cfg.SlaveMode = TIM_SLAVEMODE_DISABLE ;
	HAL_TIM_SlaveConfigSynchro(&htim2,&tim_slave_cfg);

	TIM_MasterConfigTypeDef tim_master_cfg = {0};
	tim_master_cfg.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE ;
	HAL_TIMEx_MasterConfigSynchronization(&htim2,&tim_master_cfg);

	TIM_OC_InitTypeDef tim_oc_channel_config = {0};
	tim_oc_channel_config.OCFastMode = TIM_OCFAST_ENABLE ;
	tim_oc_channel_config.OCIdleState = TIM_OCIDLESTATE_SET ;
	tim_oc_channel_config.OCMode = TIM_OCMODE_PWM1 ;
	tim_oc_channel_config.OCNIdleState  = TIM_OCIDLESTATE_RESET;
	tim_oc_channel_config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	tim_oc_channel_config.OCPolarity = TIM_OCPOLARITY_HIGH ;
	tim_oc_channel_config.Pulse = 500;
	HAL_TIM_PWM_ConfigChannel(&htim2,&tim_oc_channel_config,TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&htim2,&tim_oc_channel_config,TIM_CHANNEL_3);
	
	GPIO_name.Pin = GPIO_PIN_0;
	GPIO_name.Mode = GPIO_MODE_AF_PP ;
	GPIO_name.Pull = GPIO_NOPULL ;
	GPIO_name.Speed = GPIO_SPEED_FREQ_HIGH ;
	HAL_GPIO_Init(GPIOB,&GPIO_name);

	htim3.Instance = TIM3 ;
	htim3.Init.Prescaler = 72 -1;
	htim3.Init.Period = 20000 -1;
	htim3.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_DISABLE ;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1 ;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP ;
	htim3.Init.RepetitionCounter = 0 ;
	HAL_TIM_PWM_Init(&htim3);

	HAL_TIM_ConfigClockSource(&htim3,&tim_clk);

	tim_oc_channel_config.OCFastMode = TIM_OCFAST_ENABLE ;
	tim_oc_channel_config.OCIdleState = TIM_OCIDLESTATE_SET ;
	tim_oc_channel_config.OCMode = TIM_OCMODE_PWM1 ;
	tim_oc_channel_config.OCNIdleState  = TIM_OCIDLESTATE_RESET;
	tim_oc_channel_config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	tim_oc_channel_config.OCPolarity = TIM_OCPOLARITY_HIGH ;
	tim_oc_channel_config.Pulse = 0;
	HAL_TIM_PWM_ConfigChannel(&htim3,&tim_oc_channel_config,TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
}

void PWM_Setcompare3(uint16_t compare)
{
	LL_TIM_OC_SetCompareCH3(TIM2,compare);
}

void PWM_Setcompare2(uint16_t compare)
{
	LL_TIM_OC_SetCompareCH2(TIM2,compare);
}

void PWM_SetDJcompare3(uint16_t compare)
{
	LL_TIM_OC_SetCompareCH3(TIM3,compare);
}
