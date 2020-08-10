#ifndef _JSON_H_
#define _JSON_H_

#include <stdint.h>

#include "hrv.h"
#include "heartbeat.h"

#define JSON_MAX_KEYLEN 50
#define JSON_DATA_TABLE_NUM 23
#define JSON_DATA_BASE_NUM 4
#define JSON_DATA_STATUS_NUM 1

typedef float (*float_fun_void)(void);
typedef int (*int_fun_void)(void);
typedef char *(*str_fun_void)(void);
typedef void * fun_p;

enum VAL_TYPE{INT,FLOAT,STR};

static const struct{
    char * name;
    int num;
    char key[JSON_DATA_TABLE_NUM][JSON_MAX_KEYLEN];
    enum VAL_TYPE val_type[JSON_DATA_TABLE_NUM];
    fun_p val_func[JSON_DATA_TABLE_NUM];
} JSON_data_table={
    "table",
    JSON_DATA_TABLE_NUM,
    {
        "min",
        "max",
        "mean",
        "var",
        "std",
        "median",
        "amplitude",
        "rmssd",
        "sdnn",
        "sdsd",
        "nn50",
        "pnn50",
        "mrri",
        "mhr",
        "power",
        "vlf",
        "lf",
        "hf",
        "lf_hf",
        "lfnu",
        "hfnu",
        "sd1",
        "sd2"
    },
    {
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT,
        FLOAT
    },
    {
        HRV_COMPUTE_desc_min,
        HRV_COMPUTE_desc_max,
        HRV_COMPUTE_desc_mean,
        HRV_COMPUTE_desc_var,
        HRV_COMPUTE_desc_std,
        HRV_COMPUTE_desc_median,
        HRV_COMPUTE_desc_amplitude,
        HRV_COMPUTE_time_domain_rmssd,
        HRV_COMPUTE_time_domain_sdnn,
        HRV_COMPUTE_time_domain_sdsd,
        HRV_COMPUTE_time_domain_nn50,
        HRV_COMPUTE_time_domain_pnn50,
        HRV_COMPUTE_time_domain_mrri,
        HRV_COMPUTE_time_domain_mhr,
        HRV_COMPUTE_frequency_domain_total_power,
        HRV_COMPUTE_frequency_domain_vlf,
        HRV_COMPUTE_frequency_domain_lf,
        HRV_COMPUTE_frequency_domain_hf,
        HRV_COMPUTE_frequency_domain_lf_hf,
        HRV_COMPUTE_frequency_domain_lfnu,
        HRV_COMPUTE_frequency_domain_hfnu,
        HRV_COMPUTE_non_linear_sd1,
        HRV_COMPUTE_non_linear_sd2
    }
};

static int id_cnt=0;
static inline int base_id_val(void)
{
    return id_cnt++;
}

static const struct{
    char *name;
    int num;
    char key[JSON_DATA_BASE_NUM][JSON_MAX_KEYLEN];
    enum VAL_TYPE val_type[JSON_DATA_BASE_NUM];
    fun_p val_func[JSON_DATA_BASE_NUM];
}JSON_data_base = {
    "base",
    JSON_DATA_BASE_NUM,
    {
        "rr",
        "hr",
        "time",
        "id"
    },
    {
        FLOAT,
        FLOAT,
        FLOAT,
        INT
    },
    {
        HEART_DETECTION_heart_RR,
        HEART_DETECTION_heart_HR,
        HEART_DETECTION_get_stable_clock,
        base_id_val
    }
};

static inline float status_connection_val(void){
    return HEART_DETECTION_disconnection()?0.0:1.0;
}

static const struct{
    char *name ;
    int num;
    char key[JSON_DATA_STATUS_NUM][JSON_MAX_KEYLEN];
    enum VAL_TYPE val_type[JSON_DATA_STATUS_NUM];
    fun_p val_func[JSON_DATA_STATUS_NUM];
}JSON_data_status = {
    "status",
    JSON_DATA_STATUS_NUM,
    {
        "connection"
    },
    {
        FLOAT
    },
    {
        status_connection_val
    }
};

int _JSON_create(char * buffer ,const char * dataname ,const int num ,const char key[][JSON_MAX_KEYLEN], const enum VAL_TYPE val_type[] ,const float_fun_void val_func[]);

#define JSON_create(buffer,datastruct)\
_JSON_create(buffer,##datastruct##.name,##datastruct##.num,##datastruct##.key,##datastruct##.val_type,##datastruct##.val_func)

#endif
