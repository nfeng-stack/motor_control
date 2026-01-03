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
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStructure.Pin = ULTRASONIC_ECHO_PIN;
    GPIO_InitStructure.Pull = GPIO_PULLDOWN ;
    HAL_GPIO_Init(ULTRASONIC_PIN_GROUP, &GPIO_InitStructure);

    //外部中断初始化
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,0,1);

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

static void * (*g_ultarsound_callback)(bsp_gpio_in_enum);
void bsp_ultrasound_gpio_it_callback_register(void * (*gpio_it_callback)(bsp_gpio_in_enum))
{
    g_ultarsound_callback = gpio_it_callback;
}

void EXTI15_10_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_FLAG(ULTRASONIC_ECHO_PIN))
    {
        __HAL_GPIO_EXTI_CLEAR_FLAG(ULTRASONIC_ECHO_PIN);
        __HAL_GPIO_EXTI_CLEAR_IT(ULTRASONIC_ECHO_PIN);
        if (HAL_GPIO_ReadPin(ULTRASONIC_PIN_GROUP,ULTRASONIC_ECHO_PIN) == GPIO_PIN_SET) {
            /*echo被拉高所需执行的业务*/
            g_ultarsound_callback(BSP_GPIO_RISING);
        } else if (HAL_GPIO_ReadPin(ULTRASONIC_PIN_GROUP,ULTRASONIC_ECHO_PIN) == GPIO_PIN_RESET)
        {
            /*echo被拉低所需要执行的业务*/
            g_ultarsound_callback(BSP_GPIO_FAIL);
        }
        
    }
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

void bsp_led_gpio_reset(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
}

uint8_t bsp_led_gpio_read(void)
{
    return GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1);
}


void bsp_motor_gpio_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_name;
	GPIO_name.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_name.Pin = MOTOERA_CONTROL_IN1 | MOTOERA_CONTROL_IN2 | MOTOERB_CONTROL_IN1 | MOTOERB_CONTROL_IN2 ;
	GPIO_name.Pull = GPIO_PULLDOWN ;
	GPIO_name.Speed = GPIO_SPEED_FREQ_HIGH ;
	HAL_GPIO_Init(MOTOTER_CONTROL_GROUP,&GPIO_name);
	HAL_GPIO_WritePin(MOTOTER_CONTROL_GROUP,\
                    MOTOERA_CONTROL_IN1 | MOTOERA_CONTROL_IN2 | MOTOERB_CONTROL_IN1 | MOTOERB_CONTROL_IN2,\
                    GPIO_PIN_RESET);
}

void bsp_motor_gpio_set(uint16_t gpio_pin)
{
    HAL_GPIO_WritePin(MOTOTER_CONTROL_GROUP,gpio_pin,GPIO_PIN_SET);
}

void bsp_motor_gpio_reset(uint16_t gpio_pin)
{
    HAL_GPIO_WritePin(MOTOTER_CONTROL_GROUP,gpio_pin,GPIO_PIN_RESET);

}