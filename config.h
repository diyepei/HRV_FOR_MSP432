/*---------------------------------CONFIG_H---------------------------------*/
/* System Config */
#ifndef CONFIG_H
#define CONFIG_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define FILTER_SIZE 20
#define HRV_CACHE_SIZE 64
#define RR_CACHE_SIZE 10
#define AVG_BETA 0.01
#define SLOPE_THRESHOLD -40
#define STABLE_TIME 300000

#define HEART_BEAT_FIRST_TIME 2

#endif
