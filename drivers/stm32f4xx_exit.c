#include <rthw.h>
#include <rtthread.h>
#include <board.h>

#include "stm32f4xx_exit.h"


/****************************
KEY_UP    PE12
KEY_DOWN  PE13
KEY_LEFT  PE14
KEY_RIGHT PE15
****************************/
/*
typedef enum KeyCode_t {
	KEY_NULL  = 0,
	KEY_UP    = 1,
	KEY_DOWN  = 2,
	KEY_LEFT  = 3,
	KEY_RIGHT = 4
}KeyCode;
*/

extern struct rt_event event;

#define LED_PORT				GPIOE
#define LED1_PIN				GPIO_Pin_8
#define LED2_PIN				GPIO_Pin_9
#define LED3_PIN				GPIO_Pin_10
#define LED4_PIN				GPIO_Pin_11


#define KEY_PORT				GPIOE
#define KEY_UP_PIN	    GPIO_Pin_12
#define KEY_DOWN_PIN	  GPIO_Pin_13
#define KEY_LEFT_PIN	  GPIO_Pin_14
#define KEY_RIGHT_PIN	  GPIO_Pin_15
#define KEY_ALL  (0x0000 | KEY_UP_PIN | KEY_DOWN_PIN | KEY_LEFT_PIN | KEY_RIGHT_PIN)

static void LED_EXIT_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* init gpio configuration */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin   = KEY_UP_PIN | KEY_DOWN_PIN | KEY_LEFT_PIN | KEY_RIGHT_PIN ;
    GPIO_Init(KEY_PORT, &GPIO_InitStructure);
		GPIO_SetBits(GPIOE, KEY_UP_PIN | KEY_DOWN_PIN | KEY_LEFT_PIN | KEY_RIGHT_PIN);
	
    /* GPIO_LED*/
    GPIO_InitStructure.GPIO_Pin   = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT; 		  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL; 
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
}


static void EXTI_Configuration(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;

    /* Connect EXTI Line to PE pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource12);
    /* Configure EXTI Line12 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource13);
    /* Configure EXTI Line13 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource14);
    /* Configure EXTI Line14*/
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource15);
    /* Configure EXTI Line15 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the EXTI Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void key_scan(void)
{
    static vu16 s_KeyCode;//内部检查按键使用
    static vu8 s_key_debounce_count, s_key_long_count;
    vu16	t_key_code;

	
    t_key_code = GPIO_ReadInputData(KEY_PORT) & KEY_ALL;
//		rt_kprintf("key : %x \r\n",t_key_code);

    if((t_key_code == KEY_ALL)||(t_key_code != s_KeyCode))
    {
        s_key_debounce_count = 0;	//第一次
        s_key_long_count = 0;
    }
    else
    {
        if(++s_key_debounce_count == DEBOUNCE_SHORT_TIME)
        {   //短按键
            switch(s_KeyCode)
            {
            case   KEY_ALL & (~KEY_UP_PIN):	   //up
                rt_event_send(&event, (1 << 0));
                rt_kprintf("key: send event0\r\n");
                break;

            case   KEY_ALL & (~KEY_DOWN_PIN):	  //down
                rt_event_send(&event, (1 << 1));
                rt_kprintf("key: send event1\r\n");
                break;

            case  KEY_ALL & (~KEY_LEFT_PIN):		  //left
                rt_event_send(&event, (1 << 2));
                rt_kprintf("key: send event2\r\n");
                break;

            case   KEY_ALL & (~KEY_RIGHT_PIN):	//right
                rt_event_send(&event, (1 << 3));
                rt_kprintf("key: send event3\r\n");
                break;

            default:				  //其他组合不做处理
                break;
            }
        }
        if(s_key_debounce_count == DEBOUNCE_COUT_FIRST + DEBOUNCE_COUT_INTERVAL)
        {   //连按键
            s_key_debounce_count = DEBOUNCE_COUT_FIRST;
            ++s_key_long_count;
            /*
            switch(s_KeyCode)
            {
            case   KEY_ALL & (~KEY_UP_PIN):	   //up
            	break;
            case   KEY_ALL & (~KEY_DOWN_PIN):	  //down
            	break;
            }	*/
        }

        if(s_key_long_count == DEBOUNCE_LONG_TIME)
        {   //长按键

            s_key_long_count = DEBOUNCE_LONG_TIME;
        }
    }
    s_KeyCode = t_key_code;				// 保存本次键值

}

void rt_hw_exit_led_configure(void)
{
	  LED_EXIT_GPIO_Configuration();
    EXTI_Configuration();
    NVIC_Configuration();
}

void key_thread_entry(void *parameter)
{
    rt_time_t next_delay;
    while (1)
    {
			key_scan();
      next_delay = 10;
      /* wait next key press */
      rt_thread_delay(next_delay);
    }
}



void led_thread_entry(void* paramete)
{
    /* 保存事件结果 */
    rt_uint32_t evt;
	
    /* 无限循环*/
    while (1)
    {

        if (rt_event_recv(&event, (1 << 0), \
                           RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,\
                          10, &evt) == RT_EOK)
        {
            rt_kprintf("led1: AND recv event 0x%x\r\n", evt);

            /* 点亮第1个LED*/
						GPIO_ToggleBits(GPIOE, GPIO_Pin_8);
        }


        if (rt_event_recv(&event, (1 << 1), \
                          RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,\
                          10, &evt) == RT_EOK)
        {
            rt_kprintf("led2:OR recv event 0x%x\r\n", evt);
            /* 点亮第2个LED*/
            GPIO_ToggleBits(GPIOE,GPIO_Pin_9);
        }

        if (rt_event_recv(&event, (1 << 2), \
                           RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,\
                          10, &evt) == RT_EOK)
        {
            rt_kprintf("led3:AND recv event 0x%x\r\n", evt);
            /* 点亮第3个LED*/
            GPIO_ToggleBits(GPIOE,GPIO_Pin_10);
        }
				
        if (rt_event_recv(&event, (1 << 3), \
                           RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,\
                          10, &evt) == RT_EOK)
        {
					  rt_kprintf("led3:AND recv event 0x%x\r\n", evt);
            /* 点亮第3个LED*/
            GPIO_ToggleBits(GPIOE,GPIO_Pin_11);
        }
    }
}





