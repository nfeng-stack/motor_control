#include "bsp_gpio.h"
#include "bsp_pwm.h"
#include "motor.h"


void Motor_Astate(motoer_control control)
{
	switch (control)
	{
	case MOTOER_CONTROL_STOP:
		bsp_motor_gpio_reset(MOTOERA_CONTROL_IN1);
		bsp_motor_gpio_reset(MOTOERA_CONTROL_IN2);
		break;
	case MOTOER_CONTROL_POSITION_RUN:
		bsp_motor_gpio_set(MOTOERA_CONTROL_IN1);
		bsp_motor_gpio_reset(MOTOERA_CONTROL_IN2);
		break;
	case MOTOER_CONTROL_NEGATIVE_RUN:
		bsp_motor_gpio_reset(MOTOERA_CONTROL_IN1);
		bsp_motor_gpio_set(MOTOERA_CONTROL_IN2);
		break;
	case MOTER_CONTROL_RUN_TO_STOP:
		bsp_motor_gpio_set(MOTOERA_CONTROL_IN1);
		bsp_motor_gpio_set(MOTOERA_CONTROL_IN2);
		break;
	default:
		break;
	}
}

void MotorA_set_speed(uint16_t speed)
{
	pwn_tim2_set_wideth(speed,MOTOERA_PWM_CH);
}

void Motor_Bstate(motoer_control control)
{
	switch (control)
	{
	case MOTOER_CONTROL_STOP:
		bsp_motor_gpio_reset(MOTOERB_CONTROL_IN1);
		bsp_motor_gpio_reset(MOTOERB_CONTROL_IN2);
		break;
	case MOTOER_CONTROL_POSITION_RUN:
		bsp_motor_gpio_set(MOTOERB_CONTROL_IN1);
		bsp_motor_gpio_reset(MOTOERB_CONTROL_IN2);
		break;
	case MOTOER_CONTROL_NEGATIVE_RUN:
		bsp_motor_gpio_reset(MOTOERB_CONTROL_IN1);
		bsp_motor_gpio_set(MOTOERB_CONTROL_IN2);
		break;
	case MOTER_CONTROL_RUN_TO_STOP:
		bsp_motor_gpio_set(MOTOERB_CONTROL_IN1);
		bsp_motor_gpio_set(MOTOERB_CONTROL_IN2);
		break;
	default:
		break;
	}
}

void MotoerB_set_speed(uint16_t speed)
{
	pwn_tim2_set_wideth(speed,MOTOERB_PWM_CH);
}

void motoerA_start_positive(void)
{
	Motor_Astate(MOTOER_CONTROL_POSITION_RUN);
	pwm_tim2_start(MOTOERA_PWM_CH);
}

void motoerB_start_positive(void)
{
	Motor_Bstate(MOTOER_CONTROL_POSITION_RUN);
	pwm_tim2_start(MOTOERB_PWM_CH);
}

void motoerA_start_negative(void)
{
	Motor_Astate(MOTOER_CONTROL_NEGATIVE_RUN);
	pwm_tim2_start(MOTOERA_PWM_CH);
}

void motoerB_start_negatice(void)
{
	Motor_Bstate(MOTOER_CONTROL_NEGATIVE_RUN);
	pwm_tim2_start(MOTOERB_PWM_CH);
}

void motoerA_stop(void)
{
	Motor_Astate(MOTER_CONTROL_RUN_TO_STOP);
	pwm_tim2_close(MOTOERA_PWM_CH);
}

void motoerB_stop(void)
{
	Motor_Bstate(MOTER_CONTROL_RUN_TO_STOP);
	pwm_tim2_close(MOTOERB_PWM_CH);
}

void Motor_Init(void)
{
	bsp_motor_gpio_init();
	pwm_tim2_init();
}	