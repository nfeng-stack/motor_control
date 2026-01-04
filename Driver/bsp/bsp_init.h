#ifndef         __BSP_INIT_H
#define         __BSP_INIT_H
#include "bsp_uart.h"
#include "bsp_pwm.h"
#include "HC-SR04.h"
#include "LED.h"
#include "motor.h"
#include "OLED.h"
#include "Servo_motor.h"
void bsp_init(void);
long board_bsp_thread_init(void);

#endif