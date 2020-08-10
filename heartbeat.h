/*---------------------------------HEARTBEAT_H---------------------------------*/
#ifndef HEARTBEAT_H
#define HEARTBEAT_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <stdint.h>
#include "volatile.h"

void HEART_DETECTION_init(void);
uint8_t HEART_DETECTION_heart_beat(uint16_t signal);

extern int HEART_DETECTION_last_RR;

static inline float HEART_DETECTION_heart_RR(void){
	return (float)HEART_DETECTION_last_RR;
}

static inline float HEART_DETECTION_heart_HR(void){
	return 60000.0/(float)HEART_DETECTION_last_RR;
}

static inline float HEART_DETECTION_get_stable_clock(void){
	return (float)stable_clock;
}

static inline uint8_t HEART_DETECTION_disconnection()
{
	return GPIO_getInputPinValue(GPIO_PORT_P6,GPIO_PIN6)|GPIO_getInputPinValue(GPIO_PORT_P6,GPIO_PIN7);
}




#endif
