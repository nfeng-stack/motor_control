#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H
#include "stdint.h"
#include "stm32f1xx_hal.h"

#define ULTRASONIC_TRIG_PIN     GPIO_PIN_12
#define ULTRASONIC_ECHO_PIN     GPIO_PIN_13
#define ULTRASONIC_PIN_GROUP    GPIOB

#define OLED_SCL_PIN    GPIO_PIN_8
#define OLED_SDA_PIN    GPIO_PIN_9
#define OLED_PIN_GROUP  GPIOB

#define MOTOERA_CONTROL_IN1     GPIO_PIN_4
#define MOTOERA_CONTROL_IN2     GPIO_PIN_5
#define MOTOERB_CONTROL_IN1     GPIO_PIN_6
#define MOTOERB_CONTROL_IN2     GPIO_PIN_7
#define MOTOTER_CONTROL_GROUP   GPIOA

#define OLED_W_SCL(x)		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,(GPIO_PinState)(x))
#define OLED_W_SDA(x)		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,(GPIO_PinState)(x))
typedef enum
{
    BSP_GPIO_FAIL,
    BSP_GPIO_RISING
}bsp_gpio_in_enum;

void bsp_ultrasound_gpio_init(void);
void bsp_ultrasound_gpio_set(uint16_t gpio_pin);
void bsp_ultrasound_gpio_reset(uint16_t gpio_pin);
void bsp_ultrasound_gpio_diableit(void);
void bsp_ultarsound_gpio_enableit(void);
void bsp_oled_gpio_init(void);
void bsp_led_gpio_init(void);
void bsp_led_gpio_set(void);
void bsp_led_gpio_reset(void);
uint8_t bsp_led_gpio_read(void);
void bsp_ultrasound_gpio_it_callback_register(void * (*gpio_it_callback)(bsp_gpio_in_enum));
void bsp_motor_gpio_init(void);
void bsp_motor_gpio_set(uint16_t gpio_pin);
void bsp_motor_gpio_reset(uint16_t gpio_pin);
#endif