#include "stm32f1xx_hal.h"
#include "bsp_gpio.h"



void bsp_ultrasound_gpio_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure;
    // Trig引脚配置（PB12）
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pin = ULTRASONIC_TRIG_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Pull = GPIO_PULLDOWN ;
    HAL_GPIO_Init(ULTRASONIC_PIN_GROUP, &GPIO_InitStructure);
    
    // Echo引脚配置（PB13）
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pin = ULTRASONIC_ECHO_PIN;
    GPIO_InitStructure.Pull = GPIO_NOPULL ;
    HAL_GPIO_Init(ULTRASONIC_PIN_GROUP, &GPIO_InitStructure);
    
    // 初始化Trig引脚为低电平
    HAL_GPIO_WritePin(ULTRASONIC_PIN_GROUP, ULTRASONIC_TRIG_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ULTRASONIC_PIN_GROUP, ULTRASONIC_ECHO_PIN,GPIO_PIN_RESET);
}

void bsp_ultrasound_gpio_set(uint16_t gpio_pin)
{
    HAL_GPIO_WritePin(ULTRASONIC_PIN_GROUP,gpio_pin,GPIO_PIN_SET);
}

void bsp_ultrasound_gpio_reset(uint16_t gpio_pin)
{
    HAL_GPIO_WritePin(ULTRASONIC_PIN_GROUP,gpio_pin,GPIO_PIN_RESET);
}



/*引脚初始化*/
void bsp_oled_gpio_init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Pin = OLED_SCL_PIN ;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN ;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH ;
	HAL_GPIO_Init(OLED_PIN_GROUP,&GPIO_InitStructure);
	GPIO_InitStructure.Pin = OLED_SDA_PIN ;
	HAL_GPIO_Init(OLED_PIN_GROUP,&GPIO_InitStructure);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}



void bsp_led_gpio_init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_name;
	GPIO_name.Pin = GPIO_PIN_1 | GPIO_PIN_2;
	GPIO_name.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_name.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_name.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&GPIO_name);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1 | GPIO_PIN_2 ,GPIO_PIN_SET);
}

void bsp_led_gpio_set(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
}

void bsp_led_gpio_rest(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
}

uint8_t bsp_led_gpio_read(void)
{
    return GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1);
}