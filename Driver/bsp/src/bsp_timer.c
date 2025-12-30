#include "stm32f1xx_hal.h"
#include "bsp_timer.h"
#include "stm32f1xx_ll_tim.h"
/**
 *
 *
 *   定时器3，PA6 通道1，PA7通道2，控制直流电机 ,tim2在apb1，最大36Mhz
 */
void timer_init(void)
{
    TIM_HandleTypeDef htim3;
    htim3.Instance = TIM3;
    htim3.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_ENABLE;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.Prescaler = 36 - 1; /*输入时钟为1Mhz*/
    htim3.Init.Period = 5 -1;
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    HAL_TIM_PWM_Init(&htim3);

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pin = GPIO_PIN_6 ;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.Pin =  GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
    TIM_ClockConfigTypeDef tim_clk_cfg = {0};
    tim_clk_cfg.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&htim3, &tim_clk_cfg);

    TIM_SlaveConfigTypeDef tim_slave_cfg = {0};
    tim_slave_cfg.SlaveMode = TIM_SLAVEMODE_DISABLE;
    HAL_TIM_SlaveConfigSynchro(&htim3, &tim_slave_cfg);

    TIM_MasterConfigTypeDef tim_master_cfg = {0};
    tim_master_cfg.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
    tim_master_cfg.MasterOutputTrigger = TIM_TRGO_UPDATE ;
    HAL_TIMEx_MasterConfigSynchronization(&htim3, &tim_master_cfg);

    TIM_OC_InitTypeDef tim_oc_cfg = {0};
    tim_oc_cfg.OCMode = TIM_OCMODE_PWM1;
    tim_oc_cfg.Pulse = 500;
    tim_oc_cfg.OCPolarity = TIM_OCPOLARITY_HIGH;
    tim_oc_cfg.OCFastMode = TIM_OCFAST_ENABLE;
    HAL_TIM_PWM_ConfigChannel(&htim3, &tim_oc_cfg, TIM_CHANNEL_1);
    tim_oc_cfg.OCMode = TIM_OCMODE_PWM2;
    HAL_TIM_PWM_ConfigChannel(&htim3, &tim_oc_cfg, TIM_CHANNEL_2);

    HAL_NVIC_SetPriority(TIM3_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);

    //__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
    HAL_TIM_Base_Start(&htim3);
    // HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    // HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}
void TIM3_IRQHandler(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(TIM3))
    {
        LL_TIM_ClearFlag_UPDATE(TIM3);
        /*更改频率具体业务流程可以*/
    }
}


/**
  * @brief   设置pwm通道一的频率占空比
  * @param   duty  占空比，这个值必须在[0 100]之间
  * @param   freq  频率
  * @retval 无
 */

void set_pwm(uint16_t duty, uint16_t freq)
{
    freq = 1000000 / freq;
    duty = duty * freq / 100;
    LL_TIM_SetAutoReload(TIM3, freq);
    LL_TIM_OC_SetCompareCH1(TIM3, duty);
    LL_TIM_OC_SetCompareCH2(TIM3,duty);
}