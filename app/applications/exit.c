#include <rtthread.h>
#include "stm32f4xx.h"
#include "exit.h"
#include "stm32f4xx_exit.h"


/*  ��������4�ֽڶ��� */
ALIGN(RT_ALIGN_SIZE)

/*  ��̬�̵߳� �̶߳�ջ*/
static rt_uint8_t led_stack[512];
static rt_uint8_t key_stack[512];
/* ��̬�̵߳� �߳̿��ƿ� */
static struct rt_thread led_thread;
static struct rt_thread key_thread;

/* �¼����ƿ� */
struct rt_event event;

rt_err_t demo_thread_creat(void)
{
    rt_err_t result;
	  rt_hw_exit_led_configure();

    /* ��ʼ���¼����� */
    result = rt_event_init(&event, "event", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init event failed.\r\n");
        return -1;
    }

    /* ����led�߳� �� ���ȼ� 16 ��ʱ��Ƭ 5��ϵͳ�δ� */
    result = rt_thread_init(&led_thread,
                            "led",
                            led_thread_entry, RT_NULL,
                            (rt_uint8_t*)&led_stack[0], sizeof(led_stack), 16, 5);

    if (result == RT_EOK)
    {
        rt_thread_startup(&led_thread);
    }

    /* ����key�߳� �� ���ȼ� 15 ��ʱ��Ƭ 5��ϵͳ�δ� */
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

