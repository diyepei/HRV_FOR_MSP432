/*---------------------------------HEART_DETECTION_C---------------------------------*/
//心率检测器

#include <stdint.h>

#include "config.h"
#include "volatile.h"

int HEART_DETECTION_last_RR = 0;
static uint8_t heart_beat_first_time = HEART_BEAT_FIRST_TIME;
void HEART_DETECTION_init()
{
	heart_clock = 0;
	stable_clock = 0;
	heart_beat_first_time = 2;
	HEART_DETECTION_last_RR = 0;
}
uint8_t HEART_DETECTION_heart_beat(uint16_t signal)
{
	static uint16_t pre_signal = 0;
	static int d_avg = 0;
	static uint8_t status = 0;
	
	int d_signal = signal - pre_signal;
	pre_signal = signal;
	d_avg = AVG_BETA * (double)d_signal + (1.0 - AVG_BETA) * (double)d_avg;
	
	if(status)//状态1 已过斜率阈值
	{
		if(d_avg == 0)
		{
			status = 0;
		}
		return 0;
	}
	else//状态0 等待斜率超过阈值
	{
			if(d_avg < SLOPE_THRESHOLD)
			{
				if(heart_beat_first_time>0)
				{
					heart_beat_first_time --;
					heart_clock = 0;
					status = 1;
					return 0;
				}
				else
				{
					HEART_DETECTION_last_RR = heart_clock;
					heart_clock = 0;
					status = 1;
					return 1;
				}
			}
			else
			{
				return 0;
			}
	}
}




