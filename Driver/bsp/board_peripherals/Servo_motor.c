#include "bsp_pwm.h"
#include "rtthread.h"
void servo_motor_set_angle(uint8_t angle)
{
    uint16_t width = (1000 + (angle / 180.0f) * 1000);
    pwm_tim3_set_wideth(width,TIM_PWM_CH_3);
}

void servo_motor_start(void)
{
    servo_motor_set_angle(90);
    pwm_tim3_start(TIM_PWM_CH_3);
}
#ifdef ENABLE_DEBUG
MSH_CMD_EXPORT(servo_motor_start,start_servo);
#endif

void servo_motor_init(void)
{
    pwm_tim3_init();
    servo_motor_start();
}