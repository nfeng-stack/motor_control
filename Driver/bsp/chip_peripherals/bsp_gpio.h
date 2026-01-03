#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#define ULTRASONIC_TRIG_PIN     GPIO_PIN_12
#define ULTRASONIC_ECHO_PIN     GPIO_PIN_13
#define ULTRASONIC_PIN_GROUP    GPIOB

#define OLED_SCL_PIN    GPIO_PIN_8
#define OLED_SDA_PIN    GPIO_PIN_9
#define OLED_PIN_GROUP  GPIOB

#define OLED_W_SCL(x)		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,(GPIO_PinState)(x))
#define OLED_W_SDA(x)		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,(GPIO_PinState)(x))


void bsp_ultrasound_gpio_init(void);
void bsp_gpio_set(uint16_t gpio_pin);
void bsp_gpio_reset(uint16_t gpio_pin);
void bsp_oled_gpio_init(void);
void bsp_led_gpio_init(void);
void bsp_led_gpio_set(void);
void bsp_led_gpio_rest(void);
uint8_t bsp_led_gpio_read(void)
#endif