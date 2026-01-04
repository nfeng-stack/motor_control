#include "bsp_gpio.h"
#include "bsp_timer.h"
#include "board.h"
#include "HC-SR04.h"
#include "rtthread.h"

void (*g_detect_done_callback)(uint32_t distance);

void ultarsound_module_detect_done_callbacke_register(void (*fun)(uint32_t distance))
{
    g_detect_done_callback = fun;
}

void ultrasound_module_handler(void *message)
{
    __attribute__((used)) uint32_t tim = *(uint32_t *)message;
    uint32_t distance = 170 * tim ;
    if(g_detect_done_callback != NULL)
    {
        g_detect_done_callback(distance);
    }
    /*计算出障碍物距离当前小车的距离*/
}

static uint32_t tim ;
void * ultrasound_receive_echo(bsp_gpio_in_enum gpio_status)
{
    if (gpio_status == BSP_GPIO_FAIL) {
        tim = bsp_timer4_get_counter();
        bsp_timer4_stop();
        bsp_ultrasound_gpio_diableit();
        board_bsp_send_message(BOARD_BSP_MODULE_ULTRALSOUND,&tim);
    } else if (gpio_status == BSP_GPIO_RISING) {
        bsp_timer4_stop();
        bsp_timer4_set_counter(0);
        bsp_timer4_set_autoreload(0xffff);
        bsp_timer4_disable_it();
        bsp_timer4_start();
    }
    return NULL;
}

static void* ultrasound_triger_done(void * p)
{
    bsp_ultrasound_gpio_reset(ULTRASONIC_TRIG_PIN);
    bsp_timer4_disable_it();
    bsp_timer4_stop();
    return NULL;
}

void ultrasound_detect_distance(void)
{
    bsp_timer4_set_counter(0);
    bsp_timer4_set_autoreload(20);
    bsp_timer4_enable_it();
    bsp_ultarsound_gpio_enableit();
    bsp_ultrasound_gpio_set(ULTRASONIC_TRIG_PIN);
    bsp_timer4_start();
}
#ifdef ENABLE_DEBUG
MSH_CMD_EXPORT(ultrasound_detect_distance,start ultrasound detect);
#endif
void Ultrasound_Init(void)
{
    bsp_ultrasound_gpio_init();
    bsp_timer4_init();
    board_bsp_module_register(ultrasound_module_handler,BOARD_BSP_MODULE_ULTRALSOUND);
    bsp_ultrasound_gpio_it_callback_register(ultrasound_receive_echo);
    bsp_timer_register_itcallback(ultrasound_triger_done);
}

