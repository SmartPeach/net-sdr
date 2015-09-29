#include <rtthread.h>
#include "stm32f4xx.h"
#include "exit.h"
#include "stm32f4xx_exit.h"


/*  变量分配4字节对齐 */
ALIGN(RT_ALIGN_SIZE)

/*  静态线程的 线程堆栈*/
static rt_uint8_t led_stack[512];
static rt_uint8_t key_stack[512];
/* 静态线程的 线程控制块 */
static struct rt_thread led_thread;
static struct rt_thread key_thread;

/* 事件控制块 */
struct rt_event event;

rt_err_t demo_thread_creat(void)
{
    rt_err_t result;
	  rt_hw_exit_led_configure();

    /* 初始化事件对象 */
    result = rt_event_init(&event, "event", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init event failed.\r\n");
        return -1;
    }

    /* 创建led线程 ： 优先级 16 ，时间片 5个系统滴答 */
    result = rt_thread_init(&led_thread,
                            "led",
                            led_thread_entry, RT_NULL,
                            (rt_uint8_t*)&led_stack[0], sizeof(led_stack), 16, 5);

    if (result == RT_EOK)
    {
        rt_thread_startup(&led_thread);
    }

    /* 创建key线程 ： 优先级 15 ，时间片 5个系统滴答 */
    result = rt_thread_init(&key_thread,
                            "key",
                            key_thread_entry, RT_NULL,
                            (rt_uint8_t*)&key_stack[0], sizeof(key_stack), 15, 5);

    if (result == RT_EOK)
    {
        rt_thread_startup(&key_thread);
    }
    return 0;
}

