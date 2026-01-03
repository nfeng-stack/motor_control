#ifndef __BSP_PWM_H
#define __BSP_PWM_H
#include "stdint.h"
#include "stm32f1xx_hal.h"               // Device header
#include "stm32f1xx_ll_tim.h"

#define TIM2_PWM_OC1_PIN	GPIO_PIN_0	
#define TIM2_PWM_OC2_PIN	GPIO_PIN_1	
#define TIM2_PWM_OC3_PIN	GPIO_PIN_2
#define TIM2_PWM_OC4_PIN	GPIO_PIN_3	
#define TIM2_PWM_OC_PIN_GROUP	GPIOA

#define TIM_PWM_CH_1		TIM_CHANNEL_1
#define TIM_PWM_CH_2		TIM_CHANNEL_2	
#define TIM_PWM_CH_3		TIM_CHANNEL_3	
#define TIM_PWM_CH_4		TIM_CHANNEL_4	

#define TIM3_PWM_OC1_PIN_GROUPA	GPIO_PIN_6
#define TIM3_PWM_OC2_PIN_GROUPA	GPIO_PIN_7	
#define TIM3_PWM_OC3_PIN_GROUPB	GPIO_PIN_0
#define TIM3_PWM_OC4_PIN_GROUPB	GPIO_PIN_1	
#define TIM3_PWM_OC_PIN_GROUPA	GPIOA
#define TIM3_PWM_OC_PIN_GROUPB	GPIOB

void pwm_tim2_init(void);
void pwm_tim3_init(void);
void pwm_tim2_start(uint32_t pwm_ch);
void pwm_tim2_close(uint32_t pwm_ch);
void pwn_tim2_set_wideth(uint32_t wideth ,uint32_t pwm_ch);
void pwm_tim3_start(uint32_t pwm_ch);
void pwm_tim3_close(uint32_t pwm_ch);
void pwm_tim3_set_wideth(uint32_t wideth ,uint32_t pwm_ch);


#endif
