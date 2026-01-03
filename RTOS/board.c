/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-24                  the first version
 */

#include <rthw.h>
#include <rtthread.h>

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 1024 means 1024 bytes
 */
#define RT_HEAP_SIZE (13 * 1024)
static rt_uint8_t rt_heap[RT_HEAP_SIZE] __attribute__((section("._user_heap_stack")));

RT_WEAK void *rt_heap_begin_get(void)
{
	return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
	return rt_heap + RT_HEAP_SIZE;
}
#endif
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_usart.h"
#include "bsp_init.h"
#include "stm32f1xx_ll_dma.h"
void SysTick_Handler(void)
{
	HAL_IncTick();
	rt_interrupt_enter();

	rt_tick_increase();

	rt_interrupt_leave();
}

/**
 * This function will initial your board.
 */

void rt_hw_board_init(void)
{
	bsp_init();
/* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
	rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
	rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

/********************* 定义板级标准输入输出流 *******************************/
#ifdef RT_USING_CONSOLE

void rt_hw_console_output(const char *str)
{
	rt_size_t i = 0, size = 0;
	char a = '\r';

	size = rt_strlen(str);

	for (i = 0; i < size; i++)
	{
		if (*(str + i) == '\n')
		{
			LL_USART_TransmitData8(USART3, a);
			while (!LL_USART_IsActiveFlag_TXE(USART3))
				;
		}
		LL_USART_TransmitData8(USART3, *(str + i));
		while (!LL_USART_IsActiveFlag_TXE(USART3))
			;
	}
	while (!LL_USART_IsActiveFlag_TC(USART3))
		;
}
#define RING_BUFFER_SIZE	256
typedef struct ringbuf
{
	uint8_t bufer[RING_BUFFER_SIZE] ;
	uint8_t write_position ;
	uint8_t read_position;
}ringbuf,ringbuf_t;
struct rt_semaphore static fish_semphoer = {0} ;
static rt_sem_t fish_sem_p = &fish_semphoer;
int fish_sem_init(void)
{
	rt_sem_init(fish_sem_p,"fish_sem",0,RT_IPC_FLAG_FIFO);
	return 0;
}
INIT_APP_EXPORT(fish_sem_init);
ringbuf fish_bufer = {{0},0,0};
char write_bufer(char *src ,uint8_t count)
{
	uint16_t len = RING_BUFFER_SIZE - fish_bufer.write_position + fish_bufer.read_position ;
	if (count > len ) return -1;
	while (count--) fish_bufer.bufer[fish_bufer.write_position++] = *src++;
	return 0;
}
char read_bufer()
{
	if (fish_bufer.read_position == fish_bufer.write_position) {
		rt_sem_take(fish_sem_p,RT_WAITING_FOREVER);	
	}
	return (char)fish_bufer.bufer[fish_bufer.read_position++];
}

char rt_hw_console_getchar(void) 
{
	return read_bufer();
}

/***********************************************************************/

/***************************** 板级中断函数 *******************************/

uint8_t rx_buff[64];
void USART3_IRQHandler(void)
{
    if (LL_USART_IsActiveFlag_IDLE(USART3))
    {
        LL_USART_ClearFlag_IDLE(USART3);
        /*接收数据处理*/
        uint8_t size = sizeof(rx_buff) - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_3);
		write_bufer((char *)&rx_buff,size);
		rt_sem_release(fish_sem_p);
        rt_memset(rx_buff, 0, size);
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
        LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t)rx_buff);
        LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, sizeof(rx_buff));
        LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
    }
}

/*********************************************************************/
#include "board.h"
/**
 * 初始化bsp运行线程
 * 该线程主要为了适配硬件中断，驱动接口等
 */
struct rt_thread board_bsp_thread ;
uint32_t board_bsp_thread_stack[BOARD_BSP_THREAD_STACK_SIZE];

struct rt_mailbox board_bsp_mail;
uint32_t board_bsp_mail_pool[BOARD_BSP_MAIL_SIZE];

typedef void (*module_fun)(void * message);
module_fun g_module_fun[BOARD_BSP_MODULE_SIZE];
int8_t board_bsp_module_register(void (*module_fun)(void * message),board_bsp_mb_enum module_enum)
{
	g_module_fun[module_enum] = module_fun;
	return 0;
}

int8_t board_bsp_send_message(board_bsp_mb_enum message_enum,void *message)
{
	mb_message * mb_message_p = (mb_message *)rt_malloc(sizeof(mb_message));
	if(mb_message_p == NULL){
		return -1;
	}
	mb_message_p->message = message;
	mb_message_p->message_enum = message_enum;
	rt_mb_send(&board_bsp_mail,(rt_ubase_t)mb_message_p);
	return 0;
}

void board_bsp_thread_enrty(void *par)
{
	while(1)
	{
		rt_thread_delay(10);
		rt_kprintf("this bsp_thread_runing\n");
		rt_ubase_t rev = 0;
		if(rt_mb_recv(&board_bsp_mail,&rev,RT_WAITING_FOREVER) != RT_EOK){
			rt_kprintf("this recv mb error\n");
		}
		board_bsp_mb_enum message_enum = ((mb_message *)rev)->message_enum;
		switch (message_enum)
		{
		case BOARD_BSP_MODULE_ULTRALSOUND:
			if(g_module_fun[BOARD_BSP_MODULE_ULTRALSOUND] != NULL){
				g_module_fun[BOARD_BSP_MODULE_ULTRALSOUND](((mb_message *)rev)->message);
			}
			break;
		case BOARD_BSP_MODULE_TIMER:
			if(g_module_fun[BOARD_BSP_MODULE_TIMER] != NULL){
				g_module_fun[BOARD_BSP_MODULE_TIMER](((mb_message *)rev)->message);
			}
			break;
		case BOARD_BSP_MODULE_GPIO_IT:
			if(g_module_fun[BOARD_BSP_MODULE_GPIO_IT] != NULL){
				g_module_fun[BOARD_BSP_MODULE_GPIO_IT](((mb_message *)rev)->message);
			}
			break;
		case BOARD_BSP_MODULE_ELECTIC_MOTOR:
			if(g_module_fun[BOARD_BSP_MODULE_ELECTIC_MOTOR] != NULL){
				g_module_fun[BOARD_BSP_MODULE_ELECTIC_MOTOR](((mb_message *)rev)->message);
			}
			break;
		case BOARD_BSP_MODULE_OLED:
			if(g_module_fun[BOARD_BSP_MODULE_OLED] != NULL){
				g_module_fun[BOARD_BSP_MODULE_OLED](((mb_message *)rev)->message);
			}
			break;
		case BOARD_BSP_MODULE_LED:
			if(g_module_fun[BOARD_BSP_MODULE_LED] != NULL){
				g_module_fun[BOARD_BSP_MODULE_LED](((mb_message *)rev)->message);
			}
			break;
		case BOARD_BSP_MODULE_SERVO_MOTOR:
			if(g_module_fun[BOARD_BSP_MODULE_SERVO_MOTOR] != NULL){
				g_module_fun[BOARD_BSP_MODULE_SERVO_MOTOR](((mb_message *)rev)->message);
			}
			break;
		default:
			break;
		}
		rt_free(((void *)rev));
	}
}

 long board_bsp_thread_init(void)
 {
	rt_err_t error = rt_thread_init(&board_bsp_thread,\
									"bsp_thread",\
									board_bsp_thread_enrty,\
									NULL,\
									&board_bsp_thread_stack,\
									BOARD_BSP_THREAD_STACK_SIZE,\
									2,10);
	if(error != RT_EOK) {
		return -1;
	}
	error = rt_mb_init(&board_bsp_mail,\
						"board_bsp_mail",\
						&board_bsp_mail_pool,\
						BOARD_BSP_MAIL_SIZE,\
						RT_IPC_FLAG_FIFO);
	if(error != RT_EOK){
		return -1;
	}

    rt_thread_startup(&board_bsp_thread);
	return 0;
 }
MSH_CMD_EXPORT(board_bsp_thread_init,start_up_board_bsp_thread);


#endif
