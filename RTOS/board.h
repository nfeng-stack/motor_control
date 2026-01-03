#ifndef __BOARD_H
#define __BOARD_H

#define BOARD_BSP_THREAD_STACK_SIZE		1024
#define BOARD_BSP_MAIL_SIZE		4
#define BOARD_BSP_MODULE_SIZE   7
typedef enum 
{
	BOARD_BSP_MODULE_ULTRALSOUND,
	BOARD_BSP_MODULE_TIMER,
	BOARD_BSP_MODULE_GPIO_IT,
	BOARD_BSP_MODULE_ELECTIC_MOTOR,
	BOARD_BSP_MODULE_OLED,
	BOARD_BSP_MODULE_LED,
	BOARD_BSP_MODULE_SERVO_MOTOR
}board_bsp_mb_enum;

typedef struct 
{
	board_bsp_mb_enum message_enum;
	void * message;
}mb_message;

int8_t board_bsp_send_message(board_bsp_mb_enum message_enum,void *message);
int8_t board_bsp_module_register(void (*module_fun)(void * message),board_bsp_mb_enum module_enum);

#endif