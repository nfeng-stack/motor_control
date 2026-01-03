#include "bsp_pwm.h"


void pwm_tim2_init(void)
{
	__HAL_RCC_TIM2_CLK_ENABLE(); //开启定时器时钟
	__HAL_RCC_GPIOA_CLK_ENABLE();
	TIM_HandleTypeDef htim2 = {0};
	GPIO_InitTypeDef tim_pwm_pin = {0};
	//pwm输出io口配置
	tim_pwm_pin.Pin = TIM2_PWM_OC2_PIN | TIM2_PWM_OC3_PIN ;
	tim_pwm_pin.Mode = GPIO_MODE_AF_PP ;
	tim_pwm_pin.Pull = GPIO_NOPULL ;
	tim_pwm_pin.Speed = GPIO_SPEED_FREQ_HIGH ;
	HAL_GPIO_Init(TIM2_PWM_OC_PIN_GROUP,&tim_pwm_pin);
	//时基单元配置
	htim2.Instance = TIM2 ;
	htim2.Init.Prescaler = 72 -1;/* 因为tim的时钟来自内部，apb1如果apb1分频为1，则timclk倍频1否则2*/
	htim2.Init.Period = 1000 -1;
	htim2.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_ENABLE ;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1 ;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP ;
	htim2.Init.RepetitionCounter = 0 ;
	HAL_TIM_PWM_Init(&htim2);
	//时钟源配置
	TIM_ClockConfigTypeDef tim_clk = {0};
	tim_clk.ClockSource = TIM_CLOCKSOURCE_INTERNAL ;
	HAL_TIM_ConfigClockSource(&htim2,&tim_clk);
	//主从配置
	//1、从模式配置
	TIM_SlaveConfigTypeDef tim_slave_cfg = {0};
	tim_slave_cfg.SlaveMode = TIM_SLAVEMODE_DISABLE ;
	HAL_TIM_SlaveConfigSynchro(&htim2,&tim_slave_cfg);
	//2、主模式配置
	TIM_MasterConfigTypeDef tim_master_cfg = {0};
	tim_master_cfg.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE ;
	HAL_TIMEx_MasterConfigSynchronization(&htim2,&tim_master_cfg);
	//pwm输出模式配置
	TIM_OC_InitTypeDef tim_oc_channel_config = {0};
	tim_oc_channel_config.OCFastMode = TIM_OCFAST_ENABLE ;
	tim_oc_channel_config.OCIdleState = TIM_OCIDLESTATE_SET ;
	tim_oc_channel_config.OCMode = TIM_OCMODE_PWM1 ;
	tim_oc_channel_config.OCNIdleState  = TIM_OCIDLESTATE_RESET;
	tim_oc_channel_config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	tim_oc_channel_config.OCPolarity = TIM_OCPOLARITY_HIGH ;
	tim_oc_channel_config.Pulse = 500;
	HAL_TIM_PWM_ConfigChannel(&htim2,&tim_oc_channel_config,TIM_PWM_CH_2);
	HAL_TIM_PWM_ConfigChannel(&htim2,&tim_oc_channel_config,TIM_PWM_CH_3);
}



void pwm_tim3_init(void)
{
	__HAL_RCC_TIM3_CLK_ENABLE();	//舵机	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	TIM_HandleTypeDef htim3 = {0};
	GPIO_InitTypeDef tim_pwm_pin = {0};
	//输出pwm io口配置
	tim_pwm_pin.Pin = TIM3_PWM_OC3_PIN_GROUPB;
	tim_pwm_pin.Mode = GPIO_MODE_AF_PP ;
	tim_pwm_pin.Pull = GPIO_NOPULL ;
	tim_pwm_pin.Speed = GPIO_SPEED_FREQ_HIGH ;
	HAL_GPIO_Init(TIM3_PWM_OC_PIN_GROUPB,&tim_pwm_pin);
	//时基单元配置
	htim3.Instance = TIM2 ;
	htim3.Init.Prescaler = 72 -1;/* 因为tim的时钟来自内部，apb1如果apb1分频为1，则timclk倍频1否则2*/
	htim3.Init.Period = 1000 -1;
	htim3.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_DISABLE ;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1 ;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP ;
	htim3.Init.RepetitionCounter = 0 ;
	HAL_TIM_PWM_Init(&htim3);
	//时钟源配置
	TIM_ClockConfigTypeDef tim_clk = {0};
	tim_clk.ClockSource = TIM_CLOCKSOURCE_INTERNAL ;
	HAL_TIM_ConfigClockSource(&htim3,&tim_clk);
	//主从配置
	//1、从模式配置
	TIM_SlaveConfigTypeDef tim_slave_cfg = {0};
	tim_slave_cfg.SlaveMode = TIM_SLAVEMODE_DISABLE ;
	HAL_TIM_SlaveConfigSynchro(&htim3,&tim_slave_cfg);
	//2、主模式配置
	TIM_MasterConfigTypeDef tim_master_cfg = {0};
	tim_master_cfg.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE ;
	HAL_TIMEx_MasterConfigSynchronization(&htim3,&tim_master_cfg);
	//pwm输出模式配置
	TIM_OC_InitTypeDef tim_oc_channel_config = {0};
	tim_oc_channel_config.OCFastMode = TIM_OCFAST_ENABLE ;
	tim_oc_channel_config.OCIdleState = TIM_OCIDLESTATE_SET ;
	tim_oc_channel_config.OCMode = TIM_OCMODE_PWM1 ;
	tim_oc_channel_config.OCNIdleState  = TIM_OCIDLESTATE_RESET;
	tim_oc_channel_config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	tim_oc_channel_config.OCPolarity = TIM_OCPOLARITY_HIGH ;
	tim_oc_channel_config.Pulse = 500;
	HAL_TIM_PWM_ConfigChannel(&htim3,&tim_oc_channel_config,TIM_PWM_CH_3);
}

void pwm_tim2_start(uint32_t pwm_ch)
{
	LL_TIM_CC_EnableChannel(TIM2,pwm_ch);
	if (!LL_TIM_IsEnabledCounter(TIM2)) {
		LL_TIM_EnableCounter(TIM2);
	}
}

void pwm_tim2_close(uint32_t pwm_ch)
{
	LL_TIM_CC_DisableChannel(TIM2,pwm_ch);
}

void pwn_tim2_set_wideth(uint32_t wideth ,uint32_t pwm_ch)
{
	switch(pwm_ch)
	{
		case TIM_PWM_CH_1:
			LL_TIM_OC_SetCompareCH1(TIM2,wideth);
			break;
		case TIM_PWM_CH_2:
			LL_TIM_OC_SetCompareCH2(TIM2,wideth);
			break;
		case TIM_PWM_CH_3:
			LL_TIM_OC_SetCompareCH3(TIM2,wideth);
			break;
		case TIM_PWM_CH_4:
			LL_TIM_OC_SetCompareCH4(TIM2,wideth);
			break;
		default :
			break;
	}
}

void pwm_tim3_start(uint32_t pwm_ch)
{
	LL_TIM_CC_EnableChannel(TIM3,pwm_ch);
	if (!LL_TIM_IsEnabledCounter(TIM3)) {
		LL_TIM_EnableCounter(TIM3);
	}
}

void pwm_tim3_close(uint32_t pwm_ch)
{
	LL_TIM_CC_DisableChannel(TIM3,pwm_ch);
	if (LL_TIM_IsEnabledCounter(TIM3)) {
		LL_TIM_DisableCounter(TIM3);
	}
}

void pwn_tim3_set_wideth(uint32_t wideth ,uint32_t pwm_ch)
{
	switch(pwm_ch)
	{
		case TIM_PWM_CH_1:
			LL_TIM_OC_SetCompareCH1(TIM3,wideth);
			break;
		case TIM_PWM_CH_2:
			LL_TIM_OC_SetCompareCH2(TIM3,wideth);
			break;
		case TIM_PWM_CH_3:
			LL_TIM_OC_SetCompareCH3(TIM3,wideth);
			break;
		case TIM_PWM_CH_4:
			LL_TIM_OC_SetCompareCH4(TIM3,wideth);
			break;
		default :
			break;
	}
}
