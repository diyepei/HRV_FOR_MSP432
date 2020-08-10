/*---------------------------------UART2_C---------------------------------*/

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "led.h"
#include "oled.h"
#include "uart0.h"

volatile static char cache[2][1000];
volatile static int cache_tmp=0;
volatile static int cache_i=0;

static const eUSCI_UART_Config uartConfig = //UART_CONFIG
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,          //选用SMCLK（1M）时钟源
    26,                                      // BRDIV = 26 ，clockPrescalar时钟分频系数
    0,                                       // UCxBRF = 8  firstModReg （BRDIV、UCxBRF、 UCxBRS和SMCLK，用于设置串口波特率）
    111,                                       // UCxBRS = 17 secondModReg
    EUSCI_A_UART_NO_PARITY,                  // 校验位None
    EUSCI_A_UART_LSB_FIRST,                  // 低位优先，小端模式
    EUSCI_A_UART_ONE_STOP_BIT,               // 停止位1位
    EUSCI_A_UART_MODE,                       // UART mode
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // 设置为过采样，该数值为1
};

void UART2_init(void)
{
  /* P1.2和P1.3选用UART模式并分别作为UCA0_RXD和UCA0_TXD */
  MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
  /* 使用uartConfig配置UART_A0 */
  MAP_UART_initModule(EUSCI_A2_BASE, &uartConfig);
  /* 使能UART_A0 */
  MAP_UART_enableModule(EUSCI_A2_BASE);
}

void UART2_send(char *string){
	for(uint32_t i = 0;string[i]; i++){
		MAP_UART_transmitData(EUSCI_A2_BASE, string[i]);
	}
}

void EUSCIA2_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A2_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) 
    {
        cache[cache_tmp][cache_i++]=(char)MAP_UART_receiveData(EUSCI_A2_BASE);
    }
}

char * UART2_receive(void)
{
    cache[cache_tmp][cache_i]=0;
    char * res = cache[cache_tmp];
    cache_tmp^=1;
    cache_i=0;
    return res;
}

void UART2_test_send(char *str){
    UART2_send(str);
    UART0_send_str("send:");
    UART0_send_str(str);
    UART0_send_str("\n");
}

char * UART2_test_receive(void)
{
    cache[cache_tmp][cache_i]=0;
    char * res = cache[cache_tmp];
    cache_tmp^=1;
    cache_i=0;
    UART0_send_str("receive:");
    UART0_send_str(res);
    UART0_send_str("\n");
    return res;
}

