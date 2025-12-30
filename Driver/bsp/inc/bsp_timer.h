#ifndef     __BSP_TIMER_H
#define     __BSP_TIMER_H


extern void timer_init(void);
extern void set_pwm(uint16_t duty, uint16_t freq);
#endif