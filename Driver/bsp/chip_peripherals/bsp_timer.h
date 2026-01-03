#ifndef     __BSP_TIMER_H
#define     __BSP_TIMER_H

void bsp_timer4_init(void);
void bsp_timer_register_itcallback(void * (*callback)(void *));
void bsp_timer4_set_counter(uint32_t count);
void bsp_timer4_start();
void bsp_timer4_stop();

#endif