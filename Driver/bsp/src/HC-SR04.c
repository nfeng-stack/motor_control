#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_tim.h"
#include "motor.h"
#include "OLED.h"
#include "rtthread.h"
float a;
float right = 0, left = 0;
uint16_t Cnt;
uint16_t OverCnt;

void Ultrasound_Init()
{
    __HAL_RCC_TIM4_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure;
    // Trig引脚配置（PB12）
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Pull = GPIO_PULLDOWN ;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // Echo引脚配置（PB13）
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pin = GPIO_PIN_13;
    GPIO_InitStructure.Pull = GPIO_NOPULL ;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 初始化Trig引脚为低电平
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_RESET);
    TIM_HandleTypeDef htim4 = {0};
    htim4.Instance = TIM4 ;
	htim4.Init.Prescaler = 72 -1;
	htim4.Init.Period = 0xFFFF - 1;
	htim4.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_DISABLE ;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1 ;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP ;
	htim4.Init.RepetitionCounter = 0 ;
	HAL_TIM_Base_Init(&htim4);
    
    TIM_ClockConfigTypeDef tim_clk = {0};
	tim_clk.ClockSource = TIM_CLOCKSOURCE_INTERNAL ;
	HAL_TIM_ConfigClockSource(&htim4,&tim_clk);

	TIM_SlaveConfigTypeDef tim_slave_cfg = {0};
	tim_slave_cfg.SlaveMode = TIM_SLAVEMODE_DISABLE ;
	HAL_TIM_SlaveConfigSynchro(&htim4,&tim_slave_cfg);

	TIM_MasterConfigTypeDef tim_master_cfg = {0};
	tim_master_cfg.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE ;
	HAL_TIMEx_MasterConfigSynchronization(&htim4,&tim_master_cfg);
    // __HAL_TIM_ENABLE_IT(&htim4,TIM_IT_UPDATE);
    // HAL_NVIC_SetPriority(TIM4_IRQn,3,2);
    // HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

float Test_Distance()
{
    // 发送10us触发脉冲
    rt_kprintf("start detect distance\n %d %d",HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12),HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12));
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_SET);
    LL_TIM_SetCounter(TIM4,0);
    LL_TIM_EnableCounter(TIM4);
    while(LL_TIM_GetCounter(TIM4) < 10);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);

    LL_TIM_DisableCounter(TIM4);
    while(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13));
    LL_TIM_SetCounter(TIM4,0);
    LL_TIM_EnableCounter(TIM4);
    while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13));
    rt_kprintf("detece distance over\n");
    uint32_t tim = LL_TIM_GetCounter(TIM4);
    LL_TIM_DisableCounter(TIM4);
    uint32_t distance = tim / 1000.0 * 34.0;  // 修正计算公式
    rt_kprintf("ditance is %dcm\n,tim:%d",distance,tim);
    return distance;
}

void Auto_Run(void)
{
	if(10 < a && a < 20)
	{
		Motor_Astate(0);Motor_Bstate(0);
//		PWM_SetDJcompare3(2300);//左90度
//		left = Test_Distance();
//		OLED_ShowNum(4,10,left,4);
//		if(left > 10)
//		{
//			PWM_SetDJcompare3(1300);//正中
//		}
//		else{
//			PWM_SetDJcompare3(500);//右90度
//			right = Test_Distance();
//			OLED_ShowNum(4,10,right,4);
//			if(right > 10)
//			{
//				PWM_SetDJcompare3(1300);//正中
//			}
//		}
//		
	}
	if(a > 20 )
	{
		Motor_Astate(1);Motor_Bstate(1);
	}
	if(a < 10)
	{
		Motor_Astate(2);Motor_Bstate(2);
	}
	// Delay_ms(100);
}
