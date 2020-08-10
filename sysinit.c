/*---------------------------------SYSINIT_C---------------------------------*/
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "config.h"
#include "heartbeat.h"
#include "sht2x.h"
#include "oled.h"
#include "led.h"
#include "userButton.h"
#include "uart0.h"
#include "uart2.h"
#include "tick.h"

void MY_CLOCK_INIT()
{
	/* 停用开门狗 */
	MAP_WDT_A_holdTimer();
	// MAP_FPU_enableModule();/*启用FPU加快DCO频率计算，注：DCO是内部数字控制振荡器，默认是3M频率*/
	// MAP_CS_setDCOFrequency(4000000);/* 设置DCO频率为指定频率，此处DCO=4M*/
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
	CS_setExternalClockSourceFrequency(32768, 48000000);
	MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
	MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
	MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
	CS_startHFXT(false);
	MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_HSMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_4);
	MAP_CS_initClockSignal(CS_BCLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
}
void MY_UART_INIT()
{
	UART0_init();
	UART2_init();
}
void MY_GPIO_INIT()
{
	/* P5.1配置为模拟输入模式*/
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,GPIO_PIN1, GPIO_TERTIARY_MODULE_FUNCTION);
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6,GPIO_PIN6);/*P6.6上拉电阻 ---- L0- --R */
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6,GPIO_PIN7);/*P6.7上拉电阻 ---- L0+ --L */
}
void MY_TIMER_INIT()
{
	MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_16, TIMER32_32BIT,TIMER32_PERIODIC_MODE);
	/* 配置Timer32的第二个计数器为32位模式，周期模式，定时器频率=MCLK/定时器分频系数 此处第二个计数器频率=MCLK=1M */
  MAP_Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_16, TIMER32_32BIT,TIMER32_PERIODIC_MODE);
}
void MY_SYSTICK_INIT()
{
	MAP_SysTick_enableModule();
  MAP_SysTick_setPeriod(48000);
}
void MY_ADC_INIT()
{
	MAP_ADC14_enableModule();/*使能14位ADC模块*/
	MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_4,0); /*初始化14位ADC模块，选用MCLK时钟源，ADC预分频系数为1，ADC分频系数为4，ADC时钟=MCLK/1/4=1M*/
	MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);/*将ADC模块配置为使用单个ADC内存采样转换，内存位置为ADC_MEM0*/
	MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A4, 0); /*ADC模块内存位置选用ADC_MEM0，参考电压选择ADC_VREFPOS_AVCC_VREFNEG_VSS，通道选择A4，不选择差分模式*/   
	MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION); /*ADC模块采用手动采样模式*/
}
void MY_INTERRUPT_INIT()
{
	
	/* 使能Timer32第一个计数器中断开关*/
	MAP_Interrupt_enableInterrupt(INT_T32_INT1);
	MAP_Timer32_enableInterrupt(TIMER32_0_BASE);
	
	/* 使能Timer32第二个计数器中断开关*/
	MAP_Interrupt_enableInterrupt(INT_T32_INT2);    
	MAP_Timer32_enableInterrupt(TIMER32_1_BASE);
	
	MAP_ADC14_enableInterrupt(ADC_INT0); /*使能ADC_INT0中断*/
	MAP_Interrupt_enableInterrupt(INT_ADC14);/*使能INT_ADC14模块中断*/
	
	MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);

    MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA2);

	MAP_SysTick_enableInterrupt();
	
	MAP_Interrupt_enableMaster(); /*使能主中断*/
}

void MY_SYSTEM_INIT()
{
	LED_CONTROL_initRGB();	
	HEART_DETECTION_init();
	userButton_restore();
	SHT2x_Init();
	OLED_Init();
	
	LED_CONTROL_setRGB(RGB_BLUE,0);
	
	OLED_Clear();
	OLED_ShowString(OLED_DISPLAY_COLUMN_START,OLED_DISPLAY_ROW_1,"connect:off   ");
	OLED_ShowString(OLED_DISPLAY_COLUMN_START,OLED_DISPLAY_ROW_2,"hr:---   ");
	OLED_ShowString(OLED_DISPLAY_COLUMN_START,OLED_DISPLAY_ROW_3,"rr:---   ");
	OLED_ShowString(OLED_DISPLAY_COLUMN_START,OLED_DISPLAY_ROW_4,"time:--------  ");

	delay_ms(6000);
	UART2_test_receive();

	UART2_test_send("AT+ILOPSET=a11407CgHIo,zHgpYHHnS4aWgdWu,p6t0yLo47hOhrEDdetxBN6hhKj1LsumZ,06YJoczJDFksVXA2QxqZ\r");//DAVE
	delay_ms(500);
	UART2_test_receive();

	// UART2_test_send("AT+ILOPSET=a11407CgHIo,zHgpYHHnS4aWgdWu,dWVN0iscUj0tRS49X1Kxk3HB49Va0L9C,0N68LNJs5ZCnabaQTIEr\r");//diye
	// delay_ms(500);
	// UART2_test_receive();

	// UART2_test_send("AT+ILOPSET=a11407CgHIo,zHgpYHHnS4aWgdWu,h0hlwCxUiX8NOS3NqoZ6hXrUt2s7Z6Wk,0O9hVaYu0fVr9e35FbDP\r");//jimmy
	// delay_ms(500);
	// UART2_test_receive();

	UART2_test_send("AT+ILOPSET?\r");
	delay_ms(500);
	UART2_test_receive();

	UART2_test_send("AT+ILOPSENDJSON=property,45\r");
	delay_ms(100);
	UART2_test_receive();
	UART2_test_send("{\"model\":\"4028c65273316cb20173a438bad2000c\"}\r");
	delay_ms(500);
	UART2_test_receive();
	
	MAP_ADC14_enableConversion(); /*使能ADC模块转换*/
	MAP_ADC14_toggleConversionTrigger(); /*触发ADC模块开始采样*/
	MAP_Timer32_setCount(TIMER32_0_BASE,3000);/*设置Timer32第一个计数器计数周期，此处 4000/4M=1ms*/
	MAP_Timer32_startTimer(TIMER32_0_BASE, false);/*Timer32第一个计数器以周期性模式开始计数*/

	LED_CONTROL_setRGB(RGB_OFF,0);

}

