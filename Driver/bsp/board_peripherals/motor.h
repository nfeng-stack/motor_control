#ifndef __MOTOR_H
#define __MOTOR_H

#define MOTOERA_PWM_CH		TIM_PWM_CH_3
#define MOTOERB_PWM_CH		TIM_PWM_CH_2
#include "stdint.h"
typedef enum
{
	MOTOER_CONTROL_STOP,
	MOTOER_CONTROL_POSITION_RUN,
	MOTOER_CONTROL_NEGATIVE_RUN,
	MOTER_CONTROL_RUN_TO_STOP
}motoer_control;


void Motor_Astate(motoer_control control);
void MotorA_set_speed(uint16_t speed);
void Motor_Bstate(motoer_control control);
void MotoerB_set_speed(uint16_t speed);
void motoerA_start_positive(void);
void motoerB_start_positive(void);
void motoerA_start_negative(void);
void motoerB_start_negatice(void);
void motoerA_stop(void);
void motoerB_stop(void);
void Motor_Init(void);

#endif
