#include "main.h"
#include "rtthread.h"
int main(void)
{
    // bsp_init();
    // rt_kprintf("hello world\n");
    for (;;)
    {
    //    rt_kprintf("main");
        rt_thread_mdelay(10);      
    }
}
