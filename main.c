/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "sysinit.h"
#include "led.h"
#include "oled.h"
#include "utility.h"
#include "config.h"
#include "mqueue.h"
#include "hrv.h"
#include "uart0.h"
#include "filter.h"
#include "volatile.h"
#include "heartbeat.h"
#include "tick.h"
#include "uart2.h"
#include "userButton.h"
#include "json.h"

static char str_AT[][100]={
	"AT+ILOPRESET\r",
	"AT+ILOPSTOP\r",
	"AT+FACTORY\r",
    "AT+UARTE=OFF\r",
	"AT+ILOPDOMAIN=0\r", //设置 ilop 服务器站点
    "AT+ILOPSET=a11407CgHIo,zHgpYHHnS4aWgdWu,p6t0yLo47hOhrEDdetxBN6hhKj1LsumZ,06YJoczJDFksVXA2QxqZ\r", //设置 ilop 产品信息		//dave
	//"AT+ILOPSET=a11407CgHIo,zHgpYHHnS4aWgdWu,dWVN0iscUj0tRS49X1Kxk3HB49Va0L9C,0N68LNJs5ZCnabaQTIEr\r", //设置 ilop 产品信息	//diye
	//"AT+ILOPSET=a11407CgHIo,zHgpYHHnS4aWgdWu,h0hlwCxUiX8NOS3NqoZ6hXrUt2s7Z6Wk,0O9hVaYu0fVr9e35FbDP\r", //设置 ilop 产品信息	//jimmy
    "AT+ILOPSTART\r", //启动ilop服务
    "AT+ILOPAWSSTART\r" //启动awss路由配网模式
};

QUEUE(rr_cache_queue,2*RR_CACHE_SIZE)
QUEUE(HRV_RR_queue,HRV_CACHE_SIZE)

char json_buffer[2000];

char ai_buffer[500];

int connect=0;
int curconnnect=0;


int getRR = 0;

int main(void)
{
	MY_CLOCK_INIT();
	MY_GPIO_INIT();
	MY_UART_INIT();
	MY_TIMER_INIT();
	MY_ADC_INIT();
	MY_SYSTICK_INIT();
	MY_INTERRUPT_INIT();
	MY_SYSTEM_INIT();
	while(true)
	{
		UART2_receive();
		if(get_user_button())
    	{
			MAP_Timer32_haltTimer(TIMER32_0_BASE);
			MAP_Timer32_haltTimer(TIMER32_1_BASE);
			LED_CONTROL_setRGB(RGB_RED|RGB_BLUE,0);

			UART2_test_send(str_AT[0]);
			for(int i = 1;i<sizeof(str_AT)/sizeof(str_AT[0]);i++)
			{
				delay_ms(1500);
				UART2_test_receive();
				UART2_test_send(str_AT[i]);
			}
			delay_ms(1500);
			UART2_test_receive();
			
			LED_CONTROL_setRGB(RGB_OFF,0);
			MAP_Timer32_startTimer(TIMER32_0_BASE, false);
   	 	}

		if(curconnnect!=connect)
		{
			int len = JSON_create(json_buffer,JSON_data_status);
			sprintf(ai_buffer,"AT+ILOPSENDJSON=property,%d\r",len);
			UART2_test_send(ai_buffer);
			UART2_test_send(json_buffer);
			UART2_receive();
			connect=curconnnect;
		}

		if(getRR)
		{
			int len = JSON_create(json_buffer,JSON_data_base);
			sprintf(ai_buffer,"AT+ILOPSENDJSON=property,%d\r",len);
			UART2_test_send(ai_buffer);
			UART2_test_send(json_buffer);
			UART2_receive();
			getRR=0;
		}
		if(QUEUE_size(rr_cache_queue) >= RR_CACHE_SIZE)
		{
			
			for(int i=0;i<RR_CACHE_SIZE;i++)
			{
				while(QUEUE_size(HRV_RR_queue)>=QUEUE_MAX_SIZE(HRV_RR_queue))QUEUE_pop(HRV_RR_queue);
				QUEUE_push(HRV_RR_queue, QUEUE_front(rr_cache_queue) );
				QUEUE_pop(rr_cache_queue);
			}
			
			HRV_COMPUTE_init(QUEUE_HRV_RR_queue_arr,QUEUE_size(HRV_RR_queue));
			HRV_COMPUTE_compute(QUEUE_HRV_RR_queue_arr,QUEUE_size(HRV_RR_queue));
			
			/*
			/////////////////////////desc/////////////////////////
			
			UART0_send_str("\ndesc_results:\n");
			
			UART0_send_str("min:");
			UART0_send_float(HRV_COMPUTE_desc_min());
			
			UART0_send_str("max:");
			UART0_send_float(HRV_COMPUTE_desc_max());
			
			UART0_send_str("mean:");
			UART0_send_float(HRV_COMPUTE_desc_mean());
			
			UART0_send_str("var:");
			UART0_send_float(HRV_COMPUTE_desc_var());
			
			UART0_send_str("std:");
			UART0_send_float(HRV_COMPUTE_desc_std());
			
			UART0_send_str("median:");
			UART0_send_float(HRV_COMPUTE_desc_median());
			
			UART0_send_str("amplitude:");
			UART0_send_float(HRV_COMPUTE_desc_amplitude());
			
			/////////////////////////desc/////////////////////////
			
			/////////////////////////time_domain/////////////////////////
			
			UART0_send_str("\ntime_domain_results:\n");
			
			UART0_send_str("rmssd:");
			UART0_send_float(HRV_COMPUTE_time_domain_rmssd());
			
			UART0_send_str("sdnn:");
			UART0_send_float(HRV_COMPUTE_time_domain_sdnn());
			
			UART0_send_str("sdsd:");
			UART0_send_float(HRV_COMPUTE_time_domain_sdsd());
			
			UART0_send_str("nn50:");
			UART0_send_float(HRV_COMPUTE_time_domain_nn50());
			
			UART0_send_str("pnn50:");
			UART0_send_float(HRV_COMPUTE_time_domain_pnn50());
			
			UART0_send_str("mrri:");
			UART0_send_float(HRV_COMPUTE_time_domain_mrri());
			
			UART0_send_str("mhr:");
			UART0_send_float(HRV_COMPUTE_time_domain_mhr());
			
			/////////////////////////time_domain/////////////////////////
			
			/////////////////////////frequency_domain/////////////////////////
			
			UART0_send_str("\nfrequency_domain_results:\n");
			
			UART0_send_str("total_power:");
			UART0_send_float(HRV_COMPUTE_frequency_domain_total_power());
			
			UART0_send_str("vlf:");
			UART0_send_float(HRV_COMPUTE_frequency_domain_vlf());
			
			UART0_send_str("lf:");
			UART0_send_float(HRV_COMPUTE_frequency_domain_lf());
			
			UART0_send_str("hf:");
			UART0_send_float(HRV_COMPUTE_frequency_domain_hf());
			
			UART0_send_str("lf_hf:");
			UART0_send_float(HRV_COMPUTE_frequency_domain_lf_hf());
			
			UART0_send_str("lfnu:");
			UART0_send_float(HRV_COMPUTE_frequency_domain_lfnu());
			
			UART0_send_str("hfnu:");
			UART0_send_float(HRV_COMPUTE_frequency_domain_hfnu());
			
			/////////////////////////frequency_domain/////////////////////////
			
			/////////////////////////non_linear/////////////////////////
			
			UART0_send_str("\nnon_linear_results:\n");
			
			UART0_send_str("sd1:");
			UART0_send_float(HRV_COMPUTE_non_linear_sd1());
			
			UART0_send_str("sd2:");
			UART0_send_float(HRV_COMPUTE_non_linear_sd2());
			
			/////////////////////////non_linear/////////////////////////
			
			UART0_send_str("\n");
			*/

			int len = JSON_create(json_buffer,JSON_data_table);
			sprintf(ai_buffer,"AT+ILOPSENDJSON=property,%d\r",len);
			UART2_test_send(ai_buffer);
			UART2_test_send(json_buffer);
			UART2_receive();
		}
	}

}

void ADC14_IRQHandler(void) //当转换完成并放置到ADC_MEM0时，此中断发生。
{
	uint64_t status;    
	status = ADC14_getEnabledInterruptStatus();
	MAP_ADC14_clearInterruptFlag(status);

	if(status & ADC_INT0)
	{
		adcResult = FILTER_doFilter(MAP_ADC14_getResult(ADC_MEM0));
		MAP_ADC14_toggleConversionTrigger();/*触发ADC模块开始采样*/
	}
}

int connnect = 0; 

void T32_INT1_IRQHandler(void)//计时器1中断
{
	MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE); /*清除Timer32第一个计数器中断标志*/
	if(HEART_DETECTION_disconnection())
	{
		HEART_DETECTION_init();
		LED_CONTROL_setRGB(RGB_RED,0);
		if(curconnnect!=connect)
		{
			OLED_ShowString(8*8,OLED_DISPLAY_ROW_1,"off   ");
			OLED_ShowString(3*8,OLED_DISPLAY_ROW_2,"---   ");
			OLED_ShowString(3*8,OLED_DISPLAY_ROW_3,"---   ");
			OLED_ShowString(5*8,OLED_DISPLAY_ROW_4,"--------  ");
		}
		curconnnect=0;
	}
	else
	{
		curconnnect=1;
		heart_clock++;
		stable_clock++;
		if(HEART_DETECTION_heart_beat(adcResult))
		{
			if(stable_clock<STABLE_TIME)
			{
				LED_CONTROL_setRGB(RGB_GREEN,100);
				OLED_ShowString(8*8,OLED_DISPLAY_ROW_1,"on   ");
			}
			else
			{
				LED_CONTROL_setRGB(RGB_RED|RGB_GREEN,100);
				OLED_ShowString(OLED_DISPLAY_COLUMN_START,OLED_DISPLAY_ROW_1,"stable");
			}
			char str[100];

			sprintf(str,"%3d   ",(int)HEART_DETECTION_heart_HR());
			OLED_ShowString(3*8,OLED_DISPLAY_ROW_2,str);
			sprintf(str,"%3d   ",(int)HEART_DETECTION_heart_RR());
			OLED_ShowString(3*8,OLED_DISPLAY_ROW_3,str);
			int ttime = (int)(HEART_DETECTION_get_stable_clock()/1000.0);
			sprintf(str,"%2dmin%2ds  ",ttime/60,ttime%60);
			OLED_ShowString(5*8,OLED_DISPLAY_ROW_4,str);

			QUEUE_push(rr_cache_queue , HEART_DETECTION_heart_RR());
			getRR=1;
		}
	}
}