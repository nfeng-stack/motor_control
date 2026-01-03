#include "bsp_gpio.h"
#include "bsp_timer.h"
#include "board.h"
void Ultrasound_Init()
{
   bsp_ultrasound_gpio_init();
   bsp_timer4_init();
}

float ultrasound_detect_distance()
{

}

void Auto_Run(void)
{
	
}
