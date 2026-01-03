#ifndef     __BSP_TIMER_H
#define     __BSP_TIMER_H
#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_tim.h"
void bsp_timer4_init(void);
void bsp_timer_register_itcallback(void * (*callback)(void *));
void bsp_timer4_set_counter(uint32_t count);
void bsp_timer4_set_autoreload(uint32_t count);
void bsp_timer4_start();
void bsp_timer4_stop();
uint32_t bsp_timer4_get_counter(void);
void bsp_timer4_disable_it(void);
void bsp_timer4_enable_it(void);
#endif