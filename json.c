#include <stdio.h>
#include <stdlib.h>
#include "json.h"

int _JSON_create(char * buffer ,const char * dataname ,const int num ,const char key[][JSON_MAX_KEYLEN], const enum VAL_TYPE val_type[] ,const float_fun_void val_func[])
{
    int buffer_len = 0;
    buffer_len += sprintf(buffer+buffer_len,"{\"protocol\":\"{");

    buffer_len += sprintf(buffer+buffer_len,"\\\"%s\\\":{",dataname);

    for(int i=0;i<num;i++)
    {
        if(i>0)buffer_len += sprintf(buffer+buffer_len,",");

        switch (val_type[i])
        {
        case INT:
            buffer_len += sprintf(buffer+buffer_len,"\\\"%s\\\":%d",key[i],((int_fun_void)val_func[i])());
            break;
        case FLOAT:
            float x = ((float_fun_void)val_func[i])();
            int f = (int)x;
            int b = (int)((x-f)*1000);
            buffer_len += sprintf(buffer+buffer_len,"\\\"%s\\\":%d.%d",key[i],f,b);
            break;
        case STR:
            buffer_len += sprintf(buffer+buffer_len,"\\\"%s\\\":%s",key[i],((str_fun_void)val_func[i])());
            break;
        default:
            break;
        }
    }

    buffer_len += sprintf(buffer+buffer_len,"}");

    buffer_len += sprintf(buffer+buffer_len,"}\"}\r");

    return buffer_len;
}

