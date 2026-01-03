#include "bsp_timer.h"

void bsp_timer4_init(void)
{
    __HAL_RCC_TIM4_CLK_ENABLE();
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
    __HAL_TIM_ENABLE_IT(&htim4,TIM_IT_UPDATE);
    __HAL_TIM_CLEAR_IT(&htim4,TIM_IT_UPDATE);
    __HAL_TIM_DISABLE(&htim4);
    HAL_NVIC_ClearPendingIRQ(TIM4_IRQn);
    HAL_NVIC_SetPriority(TIM4_IRQn,3,2);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);

}

static void * (*g_timer4_it_callback)(void *);
void TIM4_IRQHandler(void)
{
    if(LL_TIM_IsActiveFlag_UPDATE(TIM4))
    {
        /*注册具体业务*/
        if(g_timer4_it_callback != NULL)
        {
            g_timer4_it_callback(NULL);
        } 
        LL_TIM_ClearFlag_UPDATE(TIM4);
    }
}

void bsp_timer_register_itcallback(void * (*callback)(void *))
{
    g_timer4_it_callback = callback;
}

void bsp_timer4_set_counter(uint32_t count)
{
    if(LL_TIM_IsEnabledCounter(TIM4))
    {
        LL_TIM_DisableCounter(TIM4);
    }
    LL_TIM_SetCounter(TIM4,count);
}

void bsp_timer4_set_autoreload(uint32_t count)
{
    LL_TIM_SetAutoReload(TIM4,count);
}

void bsp_timer4_start()
{
    LL_TIM_EnableCounter(TIM4);
}

void bsp_timer4_stop()
{
    LL_TIM_DisableCounter(TIM4);
}

uint32_t bsp_timer4_get_counter(void)
{
    return LL_TIM_GetCounter(TIM4);
}

void bsp_timer4_disable_it(void)
{
    LL_TIM_DisableIT_UPDATE(TIM4);
}

void bsp_timer4_enable_it(void)
{
    LL_TIM_EnableIT_UPDATE(TIM4);
}