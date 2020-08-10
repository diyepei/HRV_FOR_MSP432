/*---------------------------------LED_C---------------------------------*/
//RGB控制器

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <stdint.h>

#include "led.h"

void LED_CONTROL_initRGB(void)
{
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);/* 配置P2.0为输出模式, P2.0 --- LED2 红 */
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);/* 配置P2.1为输出模式, P2.1 --- LED2 绿 */
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);/* 配置P2.2为输出模式, P2.2 --- LED2 蓝 */
	LED_CONTROL_setRGB(RGB_OFF,0);
}

void LED_CONTROL_setRGB(uint8_t x  , uint32_t duration)
{
	if(x&RGB_RED)MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
	else MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
	if(x&RGB_GREEN)MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
	else MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
	if(x&RGB_BLUE)MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
	else MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);
	if(duration)
	{
		MAP_Timer32_haltTimer(TIMER32_1_BASE);
		MAP_Timer32_setCount(TIMER32_1_BASE,duration*4);
		MAP_Timer32_startTimer(TIMER32_1_BASE, true);
	}
}

void T32_INT2_IRQHandler(void)//计时器2中断
{
	MAP_Timer32_clearInterruptFlag(TIMER32_1_BASE); /* 清除Timer32第二个计数器中断标志*/  
	LED_CONTROL_setRGB(RGB_OFF,0);
}
