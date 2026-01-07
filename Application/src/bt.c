#include "main.h"
#include "rtthread.h"
#include "bt.h"
#include "car.h"
#include "stdint.h"


struct rt_thread bt_thread;
uint8_t bt_thread_stack[BT_THREAD_STACK_SIZE];

struct rt_semaphore static bt_sem = {0} ;


void bt_rev_callback(void)
{
    rt_sem_release(&bt_sem);
}

void bt_thread_entry(void * parm)
{
    while (1)
    {
       rt_err_t error = rt_sem_take(&bt_sem,RT_WAITING_FOREVER);
       if(error != RT_EOK)
       {
            return;
       }
       uint8_t is_success;
       uint8_t read_date = usart1_read(&is_success) - '0';
       car_control((car_control_direction)read_date);
       rt_thread_delay(100);
    }
    

}

void bt_thread_init(void)
{
    rt_err_t error = rt_thread_init(&bt_thread,\
                                    "bt_thread",\
                                    &bt_thread_entry,\
                                    NULL,bt_thread_stack,\
                                    BT_THREAD_STACK_SIZE,10,10);
    if(error != RT_EOK) {
        return ;
    }
	rt_sem_init(&bt_sem,"bt_sem",0,RT_IPC_FLAG_FIFO);
    bsp_usart1_register_rx_it_callback(bt_rev_callback);
    rt_thread_startup(&bt_thread);
}