#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_tim.h"
#include "motor.h"
#include "OLED.h"

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
    GPIO_InitStructure.Pull = GPIO_NOPULL ;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // Echo引脚配置（PB13）
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pin = GPIO_PIN_13;
    GPIO_InitStructure.Pull = GPIO_PULLUP ;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 初始化Trig引脚为低电平
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_RESET);
    TIM_HandleTypeDef htim4 = {0};
    htim4.Instance = TIM4 ;
	htim4.Init.Prescaler = 36 -1;
	htim4.Init.Period = 0xFFFF - 1;
	htim4.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_DISABLE ;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1 ;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP ;
	htim4.Init.RepetitionCounter = 0 ;
	HAL_TIM_Base_Init(&htim4);
}

float Test_Distance()
{
    // 发送10us触发脉冲
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_SET);
    // Delay_us(20);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,GPIO_PIN_RESET);

    // 等待回波信号变高（增加超时检测）
    uint32_t timeout = 0;
    while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13) == GPIO_PIN_RESET)
    {
        if (++timeout > 10000) // 约10ms超时（根据实际情况调整）
        {
            return -1; // 返回错误值
        }
        // Delay_us(1);
    }

    // 启动定时器测量高电平持续时间
    LL_TIM_SetCounter(TIM4,0);
    LL_TIM_EnableCounter(TIM4);
    // 等待回波信号变低（增加超时检测）
    timeout = 0;
    while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13) == GPIO_PIN_RESET)
    {
        if(LL_TIM_GetCounter(TIM4) > 58000)// 约58ms超时（4m距离）
        {
            LL_TIM_DisableCounter(TIM4);
            LL_TIM_SetCounter(TIM4,0);
            TIM4->CNT = 0;
            return -1;
        }
    }
    LL_TIM_DisableCounter(TIM4);
    Cnt = LL_TIM_GetCounter(TIM4);
    LL_TIM_SetCounter(TIM4,0);
    // 计算距离（单位：厘米）
    float distance = (Cnt * 0.034) / 2; // 修正计算公式
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
