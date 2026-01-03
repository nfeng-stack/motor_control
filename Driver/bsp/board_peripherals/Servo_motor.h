#ifndef     __SERVO_MOTOR_H
#define     __SERVO_MOTOR_H
#include "stdint.h"
void servo_motor_init(void);
void servo_motor_start(void);
void servo_motor_set_angle(uint8_t angle);

#endif