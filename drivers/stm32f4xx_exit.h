#ifndef __STM32F4xx_EXIT_H
#define __STM32F4xx_EXIT_H


#define DEBOUNCE_SHORT_TIME     2   		// �ᴥ��������ʱ��5����λ��50���룩
#define DEBOUNCE_LONG_TIME      3//10 // ������ʱ��DEBOUNCE_COUT_FIRST+DEBOUNCE_COUT_INTERVAL*DEBOUNCE_LONG_TIME����λ��50���룩
#define DEBOUNCE_COUT_FIRST     20 	// ���������ʱ��100����λ��50���룩
#define DEBOUNCE_COUT_INTERVAL  3   // ���������ʱ��20����λ��50���룩


void key_thread_entry(void* parameter);
void led_thread_entry(void* paramete);
void rt_hw_exit_led_configure(void);












#endif 

